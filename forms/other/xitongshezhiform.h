#pragma once

#include <QMainWindow>
#include "ui_xitongshezhiform.h"

#include "BaseChildWindow.h"
#include "../widgets/dynamicform.h"

class XiTongSheZhiForm : public BaseChildWindow
{
	Q_OBJECT

public:
	XiTongSheZhiForm(QWidget *parent = nullptr);
	~XiTongSheZhiForm();

public:
	void RunGCode() override;
	void LoadData(QString filePath = NULL) override;
	void SaveData(QString filePath = NULL) override;

private:
	Ui::xitongshezhiformClass ui;
	std::vector<DynamicForm*> m_dynamicForms;
};
