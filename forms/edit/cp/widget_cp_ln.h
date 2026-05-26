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
	// 通过 QOptionWidget 继承
	QString getOption() const override;

private:
	Ui::CPLNWidget ui;

private slots:
	void on_btnOk_clicked();
};
