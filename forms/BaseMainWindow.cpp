#include "BaseMainWindow.h"
#include <QDir>
#include <QToolButton>

BaseMainWindow::BaseMainWindow(QWidget* parent)
	:QMainWindow(parent)
{
	SystemSettings::instance().SetProjectDir(QDir::currentPath() + "/data/default");
	SystemSettings::instance().LoadFromFile(QDir::currentPath() + "/data/qGlobal.ini");

}

BaseMainWindow::~BaseMainWindow()
{
	
}

void BaseMainWindow::showChildWindow(QString windowTitle)
{
	auto buttons = this->findChildren<QToolButton*>();
	for (QToolButton* button : buttons) {
		if (button->text() == windowTitle) {
			button->click();
			break;
		}
	}
}