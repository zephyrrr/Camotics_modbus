#include "CAMotics_gcode_editor.h"
#include <iostream>
#include <windows.h>
#include <cbang/SmartPointer.h>
#include "camotics/qt/Highlighter.h"
#include "camotics/qt/GCodeHighlighter.h"

#include <cbang/log/Logger.h>
#include <cbang/io/LineBufferStream.h>

#include "utils/GCodeTool.h"

using namespace CAMotics;
using namespace cb;

#define DEFAULT_FILE_NAME "textbox.nc"
#define LDEFAULT_FILE_NAME L"textbox.nc"
#define LDEFAULT_FILE_OUTPUT_CMD L" > textbox.nc.output 2> textbox.nc.error"

QtWin4Editor::QtWin4Editor(QWidget* parent, CAMotics::ConsoleWriter* console, NCMachine* machine)
	: QMainWindow(parent), m_machine(machine)
{
	ui.setupUi(this);

	QVBoxLayout* layout = new QVBoxLayout(ui.centralWidget);
	ui.centralWidget->setLayout(layout);

	createNCEdit(layout);
	load(DEFAULT_FILE_NAME);

	if (console) {
		m_console = console;
	}
	else {
		m_console = new CAMotics::ConsoleWriter(this);
		cb::SmartPointer<cb::LineBufferStream<CAMotics::ConsoleWriter>> consoleStream = new cb::LineBufferStream<CAMotics::ConsoleWriter>(*m_console);
		cb::Logger::instance().setScreenStream(consoleStream);
		cb::Logger::instance().setLogCRLF(false);
		m_console->setStyleSheet("QTextEdit { background-color: #ffffff; }");

		layout->addWidget(m_console);

		LOG_INFO(1, "Log Test");
		m_console->writeToConsole();
	}
	
	//showMaximized();
}

QtWin4Editor::~QtWin4Editor()
{
	//if (editor)
	//	delete editor;
	//if (m_console)
	//	delete m_console;
}


void QtWin4Editor::createNCEdit(QVBoxLayout* layout)
{
	SmartPointer<Highlighter> highlighter;
	highlighter = new GCodeHighlighter;

	editor = new NCEdit(NULL, highlighter, NULL);

	editor->loadDarkScheme();
	editor->setWordWrapMode(QTextOption::NoWrap);
	editor->setPlainText("");

	layout->addWidget(editor);


	//{
	//    QDockWidget* dockWidget = new QDockWidget(tr("BJQ"), this);
	//    dockWidget->setObjectName("GCode Editor");
	//    dockWidget->setWidget(editor);
	//    dockWidget->widget()->setMinimumSize(QSize(100, 100));

	//    // Set the allowed areas for docking
	//    dockWidget->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::RightDockWidgetArea);

	//    // Add the dock widget to the main window
	//    addDockWidget(Qt::BottomDockWidgetArea, dockWidget);
	//}
}

void QtWin4Editor::load(QString filename)
{
	auto path = QDir::currentPath();
	QDir dir(path);
	QString filepath = dir.filePath(filename);

	QFile qFile(filepath);
	if (qFile.exists()) {
		qFile.open(QFile::ReadOnly);
		QString contents = qFile.readAll();
		qFile.close();
		contents.replace('\t', "  ");
		editor->setPlainText(contents);
	}
}

void QtWin4Editor::save(QString filename)
{
	auto path = QDir::currentPath();
	QDir dir(path);
	QString filepath = dir.filePath(filename);
	QString content = editor->toPlainText();
	QFile qFile(filepath);
	qFile.open(QFile::WriteOnly | QIODevice::Truncate);
	qFile.write(content.toUtf8());
	qFile.close();
}

void QtWin4Editor::RunCmd(WCHAR commandLine[MAX_PATH], bool showWindow)
{
	STARTUPINFO si;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW;
	if (!showWindow)
		si.wShowWindow = SW_HIDE;
	else
		si.wShowWindow = SW_SHOWNORMAL;
	//si.dwFlags = STARTF_USESTDHANDLES;
	//si.hStdOutput = hChildStdoutWr;

	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(pi));

	//SECURITY_ATTRIBUTES saAttr = { sizeof(SECURITY_ATTRIBUTES) };
	//saAttr.bInheritHandle = TRUE;
	//saAttr.lpSecurityDescriptor = NULL;

	//HANDLE hChildStdoutRd, hChildStdoutWr;
	//if (!CreatePipe(&hChildStdoutRd, &hChildStdoutWr, &saAttr, 0))
	//{
	//    std::cerr << "Failed to create pipe: " << GetLastError() << std::endl;
	//}

	//if (!SetHandleInformation(hChildStdoutRd, HANDLE_FLAG_INHERIT, 0))
	//{
	//    std::cerr << "Failed to set handle information: " << GetLastError() << std::endl;
	//}

	if (!CreateProcess(NULL, commandLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
		DWORD errorCode = GetLastError();
		LOG_INFO(0, "Error creating process: " << errorCode);
		m_console->writeToConsole();
		return;
	}

	WaitForSingleObject(pi.hProcess, 15 * 1000);

	/*char buffer[4096];
	DWORD bytesRead;
	while (ReadFile(hChildStdoutRd, buffer, sizeof(buffer), &bytesRead, NULL))
	{
			if (bytesRead == 0)
			{
					break;
			}
	}

	CloseHandle(hChildStdoutWr);
	CloseHandle(hChildStdoutRd);*/

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}

void QtWin4Editor::LogToConsole()
{
	auto path = QDir::currentPath();
	QDir dir(path);

	QString filename = DEFAULT_FILE_NAME;
	QString filename2 = filename + ".error";
	QFile qFileError(filename2);


	if (qFileError.exists() && qFileError.size() > 0) {
		qFileError.open(QFile::ReadOnly);
		QByteArray data = qFileError.readAll();
		qFileError.close();
		QString string2 = QString::fromUtf8(data);
		LOG_ERROR(string2.toUtf8().constData());
	}
	else {
		// read output
		QString filename3 = filename + ".output";
		QString filepath3 = dir.filePath(filename3);
		QFile qFileOutput(filepath3);

		if (qFileOutput.exists()) {
			qFileOutput.open(QFile::ReadOnly);
			QString contents = qFileOutput.readAll();
			qFileOutput.close();
			//contents = contents.replace(QRegExp("\r\n"), "\n");
			//LOG_INFO(0, contents.toUtf8().constData());
		}
	}

	m_console->writeToConsole();
}

void QtWin4Editor::on_actionLoad_triggered()
{
	QString filename = QFileDialog::getOpenFileName
	(this, tr("Open File"), ".",
		(
			tr("GCode Files") + QString("(*.nc *.gcode);;") +
			tr("All Files") + QString(" (*.*)")
			)
	);

	if (filename.isEmpty())
		return;

	load(filename);
}
void QtWin4Editor::on_actionSave_triggered()
{
	QString filename = QFileDialog::getSaveFileName
	(this, tr("Save File"), ".",
		(
			tr("GCode Files") + QString("(*.gcode);;") +
			tr("All Files") + QString(" (*.*)")
			)
	);

	if (filename.isEmpty())
		return;

	save(filename);
}

void QtWin4Editor::on_actionSimu_triggered()
{
	QString filename = DEFAULT_FILE_NAME;
	save(filename);

	QString filename2 = filename + ".error";
	QFile qFileError(filename2);
	if (qFileError.exists()) {
		qFileError.remove();
	}


	const WCHAR* cmdPath = L"C:\\Windows\\System32\\cmd.exe";
	WCHAR commandLine[MAX_PATH] = { 0 };
	wcscat_s(commandLine, MAX_PATH, cmdPath);
	wcscat_s(commandLine, MAX_PATH, L" /c ");
	wcscat_s(commandLine, MAX_PATH, L"Camotics_sim.exe");
	wcscat_s(commandLine, MAX_PATH, L" ");
	wcscat_s(commandLine, MAX_PATH, L" textbox.camotics");
	wcscat_s(commandLine, MAX_PATH, L" textbox.stl");
	wcscat_s(commandLine, MAX_PATH, LDEFAULT_FILE_OUTPUT_CMD);
	RunCmd(commandLine);

	LogToConsole();

	WCHAR commandLine2[MAX_PATH] = { 0 };
	wcscat_s(commandLine2, MAX_PATH, L"CAMotics_stl_viewer.exe");
	RunCmd(commandLine2, true);

	LogToConsole();
}

void QtWin4Editor::on_actionRun_triggered()
{
	//on_actionParse_triggered();

	save(DEFAULT_FILE_NAME);
	auto gcode = cb::InputSource::open(DEFAULT_FILE_NAME).toString();

	//LOG_INFO(0, gcode);
	m_console->writeToConsole();

	for(int i=0; i<1; ++i)
	m_machine->RunGCode(QString::fromStdString(gcode));
}

void QtWin4Editor::on_actionParse_triggered()
{
	QString filename = DEFAULT_FILE_NAME;
	save(filename);
	//return;

	QString filename2 = filename + ".error";
	QFile qFileError(filename2);
	if (qFileError.exists()) {
		qFileError.remove();
	}

	const WCHAR* cmdPath = L"C:\\Windows\\System32\\cmd.exe";
	WCHAR commandLine[MAX_PATH] = { 0 };
	wcscat_s(commandLine, MAX_PATH, cmdPath);
	wcscat_s(commandLine, MAX_PATH, L" /c ");
	wcscat_s(commandLine, MAX_PATH, L"Camotics_gcode.exe");
	//wcscat_s(commandLine, MAX_PATH, L"--annotate --parse");
	wcscat_s(commandLine, MAX_PATH, L" --json-out ");
	wcscat_s(commandLine, MAX_PATH, LDEFAULT_FILE_NAME);
	wcscat_s(commandLine, MAX_PATH, LDEFAULT_FILE_OUTPUT_CMD);
	RunCmd(commandLine);
	LogToConsole();
}
