#pragma once

#include "BaseChildWindow.h"
#include "../plugins/iplugin.h"

class ContainerChildWindow  : public BaseChildWindow
{
	Q_OBJECT

public:
	ContainerChildWindow(QWidget *parent);
	~ContainerChildWindow();

public:
	void RunGCode() override;
	void SetPlugin(IDoPlugin* plugin) { m_plugin = plugin; }

	void SetContent(QWidget* widget);
private:
	IDoPlugin* m_plugin;
};
