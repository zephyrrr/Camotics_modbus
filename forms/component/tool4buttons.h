#pragma once

#include <QWidget>
#include <QPushButton.h>
#include "ui_tool4buttons.h"
#include "tool4base.h"

#define MAX_BUTTONS_IN_TOOL 3
class Tool4Buttons : public QToolWidget
{
	Q_OBJECT

public:
	Tool4Buttons(QWidget *parent = nullptr, QStringList btnCaptions = QStringList());
	~Tool4Buttons();

	QPushButton* addButton(const QString& caption);
	QPushButton* getButton(int index) { return m_buttons[index]; }
	int getCount() { return m_buttons.count(); }
private:
	Ui::Tool4ButtonsClass ui;
	QList<QPushButton*> m_buttons;
	QHBoxLayout* horizontalLayout;
};

class BaseChildWindow;

class Tool4Plugins : public QObject
{
	Q_OBJECT

private:
	static QString getPluginButtonName(const QString& pyFileName);
public:
	static QList<Tool4Buttons*> create(BaseChildWindow* childWindow, QWidget* parent = nullptr);
};