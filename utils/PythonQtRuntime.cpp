#include "PythonQtRuntime.h"
#include <QFileInfo>
#include <QDir>
#include <QMessageBox>
#include <QProcessEnvironment>
#include <QCoreApplication>
#include <cbang/log/Logger.h>

PythonQtRuntime PythonQtRuntime::instance;

#ifdef PYTHON_SUPPORT

#include "../pythonqt/src/PythonQt.h"
#include "../pythonqt/extensions/PythonQt_QtAll/PythonQt_QtAll.h"
#include "../pythonqt/src/gui/PythonQtScriptingConsole.h"
#include "utils/gcodeutils.h"
#include "utils/MyPythonQtScriptingConsole.h"

#ifdef _WIN64
#ifdef _DEBUG
#pragma comment(lib, "../pythonqt/lib/x64/PythonQt-Qt5-Python3.8_d.lib")
#pragma comment(lib, "../pythonqt/lib/x64/PythonQt_QtAll-Qt5-Python3.8_d.lib")
#else
#pragma comment(lib, "../pythonqt/lib/x64/PythonQt-Qt5-Python3.8.lib")
#pragma comment(lib, "../pythonqt/lib/x64/PythonQt_QtAll-Qt5-Python3.8.lib")
#endif
#elif _WIN32
#ifdef _DEBUG
#pragma comment(lib, "../pythonqt/lib/PythonQt-Qt5-Python3.8_d.lib")
#pragma comment(lib, "../pythonqt/lib/PythonQt_QtAll-Qt5-Python3.8_d.lib")
#else
#pragma comment(lib, "../pythonqt/lib/PythonQt-Qt5-Python3.8.lib")
#pragma comment(lib, "../pythonqt/lib/PythonQt_QtAll-Qt5-Python3.8.lib")
#endif
#else
// Non-Windows code
#endif

//#include "forms/run/runmanualmulti.h"




PythonQtRuntime* PythonQtRuntime::getInstance() {
	return &instance;
}

PythonQtRuntime::PythonQtRuntime()
	: console(NULL)
{
	QProcessEnvironment env = QProcessEnvironment::systemEnvironment();

	// Get the current PATH
	QString pythonHome = env.value("PYTHONHOME");
	if (pythonHome.isEmpty()) {
		pythonHome = QDir::currentPath() + "/python";
		env.insert("PYTHONHOME", pythonHome);
		env.insert("PYTHONPATH", pythonHome); 

		wchar_t wstr[2048];
		int length = pythonHome.toWCharArray(wstr);
		wstr[length] = '\0';
		Py_SetPythonHome(wstr);

		QString path = env.value("PATH");
		if (!path.contains(pythonHome)) {
			QString new_path = path + ";" + pythonHome;
			env.insert("PATH", new_path);
		}
	}
	QString pythonPath = pythonHome + "/python.exe";
	if (QFile::exists(pythonPath)) {
		m_isPythonInstalled = true;
	}
}
PythonQtRuntime::~PythonQtRuntime() {
	if (console != NULL) {
		delete console;
	}
}

bool PythonQtRuntime::Init()
{
	if (!m_isPythonInstalled)
		return false;

	try {
		PythonQt::init(PythonQt::IgnoreSiteModule | PythonQt::RedirectStdOut);
		//PythonQt::init(PythonQt::RedirectStdOut);
		PythonQt_QtAll::init();
		PythonQtObjectPtr  mainContext = PythonQt::self()->getMainModule();
		QString path = QDir::currentPath(); //"C:/OneDrive/doing/cnc";
		//QString script = QString("this_file = '%1/python/edm/Scripts/activate_this.py'\nexec(open(this_file).read(), {'__file__': this_file})\n").arg(path);
		QString script = QString(R"(this_file = '%1/python/edm/Scripts/activate_this.py'
with open(this_file, 'r', encoding='utf-8') as file:
	exec(file.read(), {'__file__': this_file})
)").arg(path);
		mainContext.evalScript(script);
	}
	catch (const std::exception& e) {
		LOG_WARNING("Python: exception: " << e.what());
		m_isPythonInstalled = false;
	}
	catch (...) {
		m_isPythonInstalled = false;
	}
	return m_isPythonInstalled;
}

void PythonQtRuntime::ShowConsole(QObject* window)
{
	if (!m_isPythonInstalled)
		return;

	//qRegisterMetaType<RunManualMulti>("RunManualMulti");
	PythonQt::self()->registerCPPClass("GCodeUtils", "", "", PythonQtCreateObject<GCodeUtilsWrapper>);
	//PythonQt::self()->registerClass(&RunManualMulti::staticMetaObject, "app");

	PythonQtObjectPtr  mainContext = PythonQt::self()->getMainModule();
	if (console == NULL) {
		console = new MyPythonQtScriptingConsole(NULL, mainContext);
		console->setWindowTitle("Python Console");
	}
	//// add a QObject to the namespace of the main python context
	//PyExampleObject example;
	mainContext.addObject("window", window);

	//mainContext.evalFile(":example.py");

	console->resize(800, 600);
	console->show();
}

QVariant PythonQtRuntime::RunScript(const QString& script)
{
	if (!m_isPythonInstalled)
		return QVariant::Invalid;
	PythonQtObjectPtr mainContext = PythonQt::self()->getMainModule();
	//mainContext.addObject("window", window);
	return mainContext.evalScript(script);
}

QVariant PythonQtRuntime::RunFile(const QString& fileName, const QHash<QString, QObject*>& params)
{
	if (!m_isPythonInstalled)
		return QVariant::Invalid;
	try {
		//if (fileName.endsWith(".py")) {
		//	// Create .pyc auto and .pyc.hash
		//}

		PythonQtObjectPtr mainContext = PythonQt::self()->getMainModule();
		QFileInfo fileInfo(fileName);
		QString path = fileInfo.path();
		// 这个会改变系统当前目录
		//QString script = QString("import os\nos.chdir('%1')\n").arg(path);
		//mainContext.evalScript(script);
		QString script = QString("__file__ = '%1'").arg(fileName);
		mainContext.evalScript(script);
		script = QString("__name__ = '__main__'");
		mainContext.evalScript(script);

		for (const auto& pName : params.keys()) {
			mainContext.addObject(pName, params[pName]);
		}
		
		//mainContext.evalFile(":example.py");
		mainContext.evalFile(fileInfo.absoluteFilePath());

		return mainContext.getVariable("result");
	}
	catch (const std::exception& e) {
		LOG_WARNING("EDM: exception: " << e.what());
	}
	catch (...) {
	}
	return QVariant::Invalid;
}

QVariant PythonQtRuntime::CallMethod(const QString& callable, const QVariantList& args, const QVariantMap& kwargs)
{
	if (!m_isPythonInstalled)
		return QVariant::Invalid;
	PythonQtObjectPtr mainContext = PythonQt::self()->getMainModule();
	return mainContext.call(callable, args, kwargs);
}
#endif // PYTHON_SUPPORT


