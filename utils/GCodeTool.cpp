#include "GCodeTool.h"
#include <gcode/machine/MachineState.h>
#include <gcode/machine/MachineLinearizer.h>
#include <gcode/machine/MachineUnitAdapter.h>
#include <gcode/machine/GCodeMachine.h>
#include <gcode/machine/JSONMachineWriter.h>

#include <cbang/config/MinConstraint.h>
#include <cbang/os/SystemUtilities.h>
#include <cbang/json/JSON.h>
#include <cbang/SmartPointer.h>
#include <cbang/io/InputSource.h>

#include <regex>
#include <string>

#include <QFile>
#include <QDataStream>
#include <QJsonObject>
#include <QJsonDocument>

#include "../modbus/eutils.h"
#include "gcode/RealtimeJsonMachine.h"
#include "utils/nfile.h"
#include "../modbus/NCMachineDefines.h"

#define CURRENT_DATA_VERSION 1

//GCodeTool::GCodeTool(Inaccessible)
GCodeTool::GCodeTool()
{
	//m_jsonMachineWriter = new JSONMachineWriter(oss, GCode::Units::METRIC, true, 0, false, 2, 3);
	m_machineState = new MachineState();
	//m_pipeline = new MachinePipeline();
	//m_pipeline->add(m_jsonMachineWriter);
	//m_pipeline->add(m_machineState);
	
	//m_pipeline->set("_max_arc_error", 0.01, GCode::Units::METRIC);

	m_controller = new ControllerImpl(*m_machineState);
	//m_controller = new ControllerImpl(*m_pipeline);

	//machineState->set("test", 2, GCode::Units::METRIC);
	//SaveData();
	deserialize();
}

GCodeTool::~GCodeTool()
{
	//delete controller;
	//delete machineState;
	//SaveData();
}

const char GCodeTool::axis[] = { 'X', 'Y', 'Z', 'U' };
const int GCodeTool::axisInt[] = { 1, 2, 4, 8 };
const char* GCodeTool::csNames[] = { "54", "55", "56", "57", "58", "59" };

//cb::JSON::ValuePtr GCodeTool::ParseGCode2Json(const InputSource& source)
//{
//	auto json2 = ParseCCode(source);
//
//	m_pipeline->start();
//	Interpreter(*m_controller).read(source);
//	m_pipeline->end();
//	stream->flush();
//	oss.flush();
//	std::string s = oss.str();
//	oss.clear();
//	if (!s.empty()) {
//		cb::JSON::ValuePtr json = cb::JSON::Reader::parseString(s);
//		json2->appendFrom(*json);
//	}
//	return json2;
//}

//cb::JSON::ValuePtr GCodeTool::ParseCCode(const InputSource& source)
//{
//	std::vector<std::string> cCommandStrings;
//	std::regex pattern("C(\\d{3})\\s*=\\s*(.*)");
//	while (true) {
//		std::istream& iss = source.getStream();
//		std::streampos oldpos = iss.tellg();
//		std::string line = source.getLine();
//		std::smatch match;
//		if (std::regex_search(line, match, pattern)) {
//			cCommandStrings.push_back(match[0].str());
//		}
//		else {
//			iss.seekg(oldpos);
//			break;
//		}
//	}
//	cb::JSON::ValuePtr json2 = cb::JSON::Factory().createList();
//	if (cCommandStrings.size() > 0) {
//		for (size_t i = 0; i < cCommandStrings.size(); ++i) {
//			SmartPointer<JSON::Value> value = JSON::Factory().createDict();
//			value->insert("type", "C_command");
//			value->insert("value", cCommandStrings[i]);
//			json2->append(value);
//		}
//	}
//	return json2;
//}


// not used now. run gcode at once, not line by line
//cb::JSON::ValuePtr GCodeTool::EvaluateGCode(QString gcode, bool outputJson)
//{
//	SmartPointer<ControllerImpl> controller;
//	SmartPointer<MachineState> machineState;
//	machineState = new MachineState();
//
//	std::string stringGCode = EUtils::QString2StdString(gcode);
//	const InputSource& source = cb::StringInputSource(stringGCode);
//
//	auto json2 = ParseCCode(source);
//
//	std::ostringstream oss;
//	cb::SmartPointer<std::ostream> stream = SmartPointer<std::ostream>::Phony(&oss);
//	SmartPointer<MachineInterface> machine;
//	if (outputJson) {
//		machine = new JSONMachineWriter(*stream, GCode::Units::METRIC, true, 0, false, 2, 3);
//	}
//	else {
//		machine = new GCodeMachine(stream, GCode::Units::METRIC);
//	}
//	MachinePipeline pipeline = MachinePipeline();
//	pipeline.add(machine);
//	pipeline.add(machineState);
//	pipeline.set("_max_arc_error", 0.01, GCode::Units::METRIC);
//
//	controller = new ControllerImpl(pipeline);
//
//	pipeline.start();
//	Interpreter(*controller).read(source);
//	pipeline.end();
//
//	stream->flush();
//	std::string s = oss.str();
//	if (!s.empty()) {
//		if (outputJson) {
//			cb::JSON::ValuePtr json = cb::JSON::Reader::parseString(s);
//			json2->appendFrom(*json);
//		}
//		else {
//			cb::JSON::ValuePtr json = cb::JSON::Factory().createList();
//			std::istringstream f(s);
//			std::string s1;
//			while (std::getline(f, s1, '\n')) {
//				SmartPointer<JSON::Value> value = JSON::Factory().createDict();
//				value->insert("type", "GCode");
//				value->insert("value", s1);
//				json->append(value);
//			}
//			json2->appendFrom(*json);
//		}
//	}
//	//delete jsonMachine;
//	return json2;
//}

QString GCodeTool::ParseGCode(QString gcode)
{
	std::string stringGCode = EUtils::QString2StdString(gcode);
	const InputSource& source = cb::InputSource(stringGCode);

	std::ostringstream oss;
	cb::SmartPointer<std::ostream> stream = SmartPointer<std::ostream>::Phony(&oss);

	//ParseCCode(source);

	GCode::Printer printer(*stream, false);
	Parser(source).parse(printer, 0);

	stream->flush();
	std::string s = oss.str();
	// Here is the modified gcode which is synatax ok for gcode

	// todo: put sub code to the beginning of the gcode
	s = moveAllSubBlocksToFront(s);
	return QString::fromStdString(s);
}

/**
 * @brief Finds all blocks matching "O<number> sub" ... "O<number> endsub",
 * cuts them, and prepends their content to the beginning of the text.
 * @param inputText The original string to process.
 * @return A new string with all sub-blocks moved to the front in the order they appeared.
 */
std::string GCodeTool::moveAllSubBlocksToFront(const std::string& inputText) {
	// The regex remains the same, designed to find one complete block.
	// O(\\d+) sub  -> Matches "O" + digits (group 1) + " sub".
	// ([\\s\\S]*?) -> Captures the multi-line content inside (group 2).
	// O\\1 endsub  -> Matches the corresponding "endsub" using a backreference to the digits.
	std::regex block_regex("O(\\d+) sub([\\s\\S]*?)O\\1 endsub");

	// Vector to store the content of all found blocks in the order they appear.
	std::vector<std::string> all_block_contents;

	// Use sregex_iterator to find all non-overlapping matches in the text.
	auto matches_begin = std::sregex_iterator(inputText.begin(), inputText.end(), block_regex);
	auto matches_end = std::sregex_iterator();

	// Loop through all the matches found by the iterator.
	for (std::sregex_iterator i = matches_begin; i != matches_end; ++i) {
		std::smatch match = *i;
		// match[2] is the inner content of the block (our second capture group).
		all_block_contents.push_back(match[0].str());
	}

	// If no blocks were found, just return the original text.
	if (all_block_contents.empty()) {
		//std::cout << "No matching 'O... sub' / 'O... endsub' blocks found." << std::endl;
		return inputText;
	}

	// After collecting the content, use regex_replace to remove ALL occurrences
	// of the blocks from the original text in one go.
	std::string text_without_blocks = std::regex_replace(inputText, block_regex, "");

	// Use a stringstream for efficient concatenation.
	std::stringstream result_stream;

	// First, add all the collected block contents to the start.
	for (const auto& content : all_block_contents) {
		result_stream << content << "\n";
	}

	// Then, append the rest of the text (which has the blocks removed).
	result_stream << text_without_blocks;

	return result_stream.str();
}

bool GCodeTool::RunGCode(SmartPointer<MachineInterface> machine, std::string gcode)
{
	LOG_INFO(8, "EDM: RunGCode \n" << gcode);
	if (m_isRunning) {
		LOG_WARNING("EDM: GCodeTool is already running.");
		return false;
	}

	bool ret = true;
	m_isRunning = true;
	std::string stringGCode = gcode;
	const InputSource& source = cb::InputSource(stringGCode);

	// dont' run in it. in NCMachine::RunCCode()
	//auto json2 = ParseCCode(source);

	std::ostringstream oss;
	cb::SmartPointer<std::ostream> stream = SmartPointer<std::ostream>::Phony(&oss);
	//SmartPointer<MachineInterface> machine;

	MachinePipeline pipeline = MachinePipeline();
	if (machine != nullptr) {
		pipeline.add(machine);
	}
	pipeline.add(m_machineState);
	pipeline.set("_max_arc_error", 0.01, GCode::Units::METRIC);

	m_controller->setMachine(pipeline);

	pipeline.start();
	auto interpreter = Interpreter(*m_controller); // .read(source);
	interpreter.push(source);
	try {
		interpreter.run(0);
	}
	catch (const std::exception& e) {
		LOG_WARNING("EDM: exception: " << e.what());
		ret = false;
	}
	catch (...) {
		LOG_WARNING("EDM: exception: Unknown");
		ret = false;
	}
	pipeline.end();

	m_controller->setMachine(*m_machineState);
	m_isRunning = false;

	return ret;
}

void GCodeTool::deserialize()
{
	QString fileName = GetDataFileName();
	//QString bakfileName = fileName + ".bak";

	bool loadOk = true;

	int cnt = 0;
	while (true)
	{
		if (!QFile::exists(fileName))
			break;
		if (cnt > 0)
			break;
		cnt++;

		QFile file(fileName);

		//if (!loadOk)
		//{
		//	NFile bakfile(bakfileName);
		//	if (bakfile.exists()) {
		//		if (file.exists()) {
		//			file.remove();
		//		}
		//		bakfile.copy(fileName);
		//		bakfile.remove();
		//	}
		//	else {
		//		break;
		//	}
		//}


		bool b = file.open(QIODevice::ReadOnly);
		if (!b) {
			loadOk = false;
			continue;
		}

		QDataStream out(&file);

		if (out.version() != QDataStream::Qt_5_15) {
			loadOk = false;
			continue;
		}
		int version;
		out >> version;
		if (version != CURRENT_DATA_VERSION) {
			loadOk = false;
			continue;
		}

		double v;

		for (int i = GLOBAL_OFFSETS_ENABLED; i < CS60_ROTATION + 1; ++i)
		{
			out >> v;
			m_machineState->set(static_cast<GCode::address_t>(i), v, GCode::Units::METRIC);
		}
		for (int idx = 0; idx < POS_MEMORY_LEN; ++idx)
			for (int col = 0; col < POS_AXIS_LEN + 1; ++col)
			{
				out >> v;
				m_machineState->set(GetPosMomoryName(idx, col), v, GCode::Units::METRIC);
			}
		//QString s;
		//while (!out.atEnd()) {
		//	out >> s;
		//	out >> v;
		//	m_machineState->set(s.toUtf8().constData(), v, GCode::Units::METRIC);
		//}

		file.close();

		loadOk = true;
		break;
	}
}

std::string GCodeTool::GetPosMomoryName(int idx, int col)
{
	std::stringstream ss;
	ss << "Memory Pos " << idx << "_" << col;
	std::string str = ss.str();
	return str;
}

QString GCodeTool::FormatPos(double v)
{
	QString numString;

	if (v < 0) {
		numString = "-";
		v = -v;
	}
	else
	{
		numString = "+";
	}

	numString += QString::number(v, 'f', 3).rightJustified(9, '0');
	return numString;
}


void GCodeTool::serialize()
{
	QString fileName = GetDataFileName();
	//QString bakfileName = fileName + ".bak";

	NFile file(fileName);
	//NFile bakfile(bakfileName);
	//if (file.exists()) {
	//	if (bakfile.exists()) {
	//		bakfile.remove();
	//	}
	//	QFile::copy(fileName, bakfileName);
	//}
	file.open(QIODevice::WriteOnly);

	QDataStream in(&file);
	in.setVersion(QDataStream::Qt_5_15);

	in << CURRENT_DATA_VERSION;

	for (int i = GLOBAL_OFFSETS_ENABLED; i < CS60_ROTATION + 1; ++i)
	{
		double v = m_machineState->get(static_cast<GCode::address_t>(i), GCode::Units::METRIC);
		in << v;
	}

	for (int idx = 0; idx < POS_MEMORY_LEN; ++idx)
		for (int col = 0; col < POS_AXIS_LEN + 1; ++col)
		{
			double v = m_machineState->get(GetPosMomoryName(idx, col), GCode::Units::METRIC);
			in << v;
		}
	//for (auto it = m_machineState->named.begin(); it != m_machineState->named.end(); ++it) {
	//	in << QString::fromStdString(it->first) << it->second.get(GCode::Units::METRIC);
	//}

	file.close();

	//QJsonObject obj = QJsonObject::fromVariantMap(myClass.toVariantMap());
	//QJsonDocument doc(obj);
	//file.write(doc.toJson());

	//if (bakfile.exists()) {
	//	bakfile.remove();
	//}
	//QFile::copy(fileName, bakfileName);
}


