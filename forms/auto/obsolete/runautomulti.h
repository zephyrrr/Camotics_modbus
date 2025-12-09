#pragma once

#include <QWidget>
#include <QMessageBox>
#include "ui_runautomulti.h"
#include "BaseChildWindow.h"
#include "widgets/spreadsheet.h"
#include "widgets/buttonedittablewidget.h"
#include "utils/DataTable.h"
#include "plugins/ipointgenerator.h"
#include "../component/tool4zeroandhalfaxis.h"

class RunAutoMulti : public BaseChildWindow
{
	Q_OBJECT

public:
	RunAutoMulti(QWidget* parent = nullptr);
	~RunAutoMulti();

	QString GetGCode() override;
	void RunGCode() override;

	void LoadData(QString filePath = NULL) override;
	void SaveData(QString filePath = NULL) override;

private:
	Ui::RunAutoMulti ui;

	ButtonEditTableWidget* table3;
	DataTable* table1;
	DataTable* table2;

	Tool4ZeroAndHalfAxis* m_tool4ZeroAndHalfAxis = NULL;
	QHash<int, int> mapLine2Row;
	QString m_name4RunAutoOne = "runautoone";
protected:
	void showEvent(QShowEvent* event) override;

private slots:
	void on_btnJghs_clicked();
	//void on_btnShortcutGrid1_clicked();

};
