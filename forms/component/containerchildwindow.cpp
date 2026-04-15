#include "containerchildwindow.h"
#include "../modbus/NCMachine.h"

ContainerChildWindow::ContainerChildWindow(QWidget *parent)
	: BaseChildWindow(parent), m_plugin(nullptr)
{}

ContainerChildWindow::~ContainerChildWindow()
{}

void ContainerChildWindow::RunGCode()
{
	if (m_plugin) {
		QString gcode = m_plugin->doIt().value<QString>();
		if (!gcode.isEmpty()) {
			m_ncMachine->RunGCode(gcode);
		}
	}
}

void ContainerChildWindow::SetContent(QWidget* widget)
{
	QHBoxLayout* layout = new QHBoxLayout(this);
	layout->addWidget(widget);
}
