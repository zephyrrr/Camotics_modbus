#pragma once

#include "BaseChildWindow.h"
#include <QVariant>

/**
 * PythonPluginChildWindow - Container for Python-based plugins
 *
 * Wraps a QWidget created from Python and provides lifecycle hooks.
 * Python plugins can define optional callback functions:
 *   - on_show(window) - Called when widget is shown
 *   - on_hide(window) - Called when widget is hidden
 *   - run_gcode(window) - Called when RunGCode is triggered
 */
class PythonPluginChildWindow : public BaseChildWindow
{
	Q_OBJECT

public:
	PythonPluginChildWindow(QWidget* parent);
	~PythonPluginChildWindow();

	// Set the Python-created widget
	void SetPythonWidget(QWidget* widget);

	// Set the plugin file path (for callbacks)
	void SetPluginFile(const QString& filePath) { m_pluginFile = filePath; }

	// Set the plugin name
	void SetPluginName(const QString& name) { m_pluginName = name; }
	QString GetPluginName() const { return m_pluginName; }

	void RunGCode() override;

protected:
	void showEvent(QShowEvent* event) override;
	void hideEvent(QHideEvent* event) override;

private:
	QWidget* m_pythonWidget = nullptr;
	QString m_pluginFile;
	QString m_pluginName;

	// Call a Python function with (window) parameter
	QVariant CallPythonFunction(const QString& funcName);
};