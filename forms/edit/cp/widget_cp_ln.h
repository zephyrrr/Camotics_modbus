#pragma once

#include <QMainWindow>
#include "ui_widget_cp_ln.h"

#include "widgets/qlineedit4optionwidget.h"

class WidgetCPLN : public QOptionWidget
{
	Q_OBJECT

public:
	WidgetCPLN(QWidget *parent = nullptr);
	~WidgetCPLN();

protected:

public:
	// Í¨¹ý QOptionWidget ¼Ì³Ð
	QString getOption() const override;

private:
	Ui::CPLNWidget ui;

private slots:
	void on_btnOk_clicked();
};
