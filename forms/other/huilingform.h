#pragma once

#include <QMainWindow>
#include "ui_huilingform.h"
#include "BaseChildWindow.h"

class HuiLingForm : public BaseChildWindow
{
	Q_OBJECT

public:
	HuiLingForm(QWidget *parent = nullptr);
	~HuiLingForm();

private:
	Ui::HuiLingFormClass ui;

public:
	void RunGCode() override;

private slots:
	void on_btnX_clicked();
	void on_btnY_clicked();
	void on_btnZ_clicked();
	void on_btnU_clicked();
};
