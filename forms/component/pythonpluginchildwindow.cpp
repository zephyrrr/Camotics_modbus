// Workaround for boost/cbang header conflict with MSVC
#ifdef _MSC_VER
#define _CRT_NONSTDC_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "pythonpluginchildwindow.h"
#include "modbus/NCMachine.h"  // Include NCMachine.h before PythonQt headers
#include "utils/PythonQtRuntime.h"
#include <QHBoxLayout>

PythonPluginChildWindow::PythonPluginChildWindow(QWidget* parent)
	: BaseChildWindow(parent)
{
}

PythonPluginChildWindow::~PythonPluginChildWindow()
{
}

void PythonPluginChildWindow::SetPythonWidget(QWidget* widget)
{
	m_pythonWidget = widget;
	if (widget) {
		QHBoxLayout* layout = new QHBoxLayout(this);
		layout->setContentsMargins(0, 0, 0, 0);
		layout->addWidget(widget);
	}
}

void PythonPluginChildWindow::RunGCode()
{
	// Try Python callback first
	QVariant result = CallPythonFunction("run_gcode");
	if (result.isValid() && result.type() == QVariant::String) {
		QString gcode = result.toString();
		if (!gcode.isEmpty() && m_ncMachine) {
			m_ncMachine->RunGCode(gcode);
		}
	}
}

void PythonPluginChildWindow::showEvent(QShowEvent* event)
{
	BaseChildWindow::showEvent(event);
	CallPythonFunction("on_show");
}

void PythonPluginChildWindow::hideEvent(QHideEvent* event)
{
	BaseChildWindow::hideEvent(event);
	CallPythonFunction("on_hide");
}

QVariant PythonPluginChildWindow::CallPythonFunction(const QString& funcName)
{
	if (m_pluginFile.isEmpty()) {
		return QVariant();
	}

#ifdef PYTHON_SUPPORT
	if (!PythonQtRuntime::getInstance()->IsInit()) {
		return QVariant();
	}

	try {
		// Build the call: module_name.function_name(window)
		QFileInfo fileInfo(m_pluginFile);
		QString moduleName = fileInfo.baseName();

		// Get the module from sys.modules or import it
		QString script = QString(
			"import sys\n"
			"import importlib\n"
			"module_name = '%1'\n"
			"if module_name in sys.modules:\n"
			"    mod = sys.modules[module_name]\n"
			"else:\n"
			"    spec = importlib.util.spec_from_file_location(module_name, '%2')\n"
			"    mod = importlib.util.module_from_spec(spec)\n"
			"    sys.modules[module_name] = mod\n"
			"    spec.loader.exec_module(mod)\n"
			"func = getattr(mod, '%3', None)\n"
			"if func and callable(func):\n"
			"    result = func(_plugin_window)\n"
			"else:\n"
			"    result = None\n"
		).arg(moduleName, QString(m_pluginFile).replace("\\", "/"), funcName);

		// Set the window reference
		PythonQtObjectPtr mainContext = PythonQt::self()->getMainModule();
		mainContext.addObject("_plugin_window", this);

		QVariant result = mainContext.evalScript(script);
		return result;
	}
	catch (...) {
		return QVariant();
	}
#endif

	return QVariant();
}