#pragma once

#include <QWidget>
#include "ui_about.h"
#include "BaseChildWindow.h"

class AboutForm : public BaseChildWindow
{
	Q_OBJECT

public:
	AboutForm(QWidget *parent = nullptr);
	~AboutForm();

public:
	void RunGCode() override {};

protected:
	bool eventFilter(QObject* obj, QEvent* event) override;

private:
	Ui::aboutClass ui;
	int m_adminClickCount = 0;
};
