#pragma once

#include <QMainWindow>
#include "ui_fangdiancheshiform.h"

#include "BaseChildWindow.h"
#include "../widgets/dynamicform.h"

class FangDianCanShuSingleForm : public QWidget
{
	Q_OBJECT

public:
	FangDianCanShuSingleForm(QWidget *parent = nullptr);
	~FangDianCanShuSingleForm();

public:
	void RunGCode(NCMachine* ncMachine);

	QString GetValuesAsCCode();
	void SetValuesAsCCode(QString cCode);

	DynamicForm* GetDynamicForm(int idx = 1) { return (idx == 0) ? m_dynamicForm0 : m_dynamicForm; }
	QHBoxLayout* GetLayout(int idx = 1) { return (idx == 0) ? m_layout0 : m_layout1; }
public:
	void LoadFromParametersC(int cIndex = -1);

private:
	Ui::fangdiancheshiformClass ui;
	DynamicForm* m_dynamicForm;
	QStringList m_pNames;
	DynamicForm* m_dynamicForm0;
	QHBoxLayout* m_layout0;
	QHBoxLayout* m_layout1;
};

class FangDianCheShiForm : public BaseChildWindow
{
	Q_OBJECT

public:
	FangDianCheShiForm(QWidget* parent = nullptr);
	~FangDianCheShiForm();

public:
	void RunGCode() override;
	void LoadData(QString filePath = NULL) override;
	void SaveData(QString filePath = NULL) override;

	bool LoadFromFile = true;
private:
	FangDianCanShuSingleForm* m_form;
};