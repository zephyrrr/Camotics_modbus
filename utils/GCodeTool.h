#ifndef GCODETOOL_H
#define GCODETOOL_H

#include <gcode/ControllerImpl.h>
#include <gcode/Printer.h>
#include <gcode/parse/Parser.h>
#include <gcode/interp/Interpreter.h>
#include <gcode/machine/MachinePipeline.h>
#include <gcode/machine/MachineState.h>
#include <gcode/machine/JSONMachineWriter.h>

#include <cbang/Exception.h>
#include <cbang/ApplicationMain.h>
#include <cbang/util/Singleton.h>

#include <iostream>
#include <QString>

#define DATA_FILE_NAME "data/qcnc.dat"

using namespace cb;
using namespace GCode;

class GCodeTool// : public Singleton<GCodeTool>
{
public:
	//GCodeTool(Inaccessible);
	GCodeTool();
	~GCodeTool();

	//static cb::JSON::ValuePtr EvaluateGCode(QString gcode, bool outputJson);
	static QString ParseGCode(QString gcode);

	bool RunGCode(SmartPointer<MachineInterface> machine, std::string gcode);
	SmartPointer<ControllerImpl> GetController() { return m_controller; }
	const bool IsRunning() { return m_isRuning; }

	void deserialize();
	void serialize();

	static const char axis[4];
	static const int axisInt[4];
	static const char* csNames[6];
	static std::string GetPosMomoryName(int idx, int col);
	static QString FormatPos(double v);

private:
	static std::string moveAllSubBlocksToFront(const std::string& inputText);
	//static cb::JSON::ValuePtr ParseCCode(const InputSource& source);

	SmartPointer<ControllerImpl> m_controller;
	SmartPointer<MachineState> m_machineState;
	bool m_isRuning = false;
	//SmartPointer<MachinePipeline> m_pipeline;
	//SmartPointer<JSONMachineWriter> m_jsonMachineWriter;

	//cb::SmartPointer<std::ostream> stream;
	//cb::SmartPointer<std::ostringstream> oss;
	//std::ostringstream oss;
	//cb::SmartPointer<std::ostream> stream = SmartPointer<ostream>::Phony(&oss);
};

#endif

