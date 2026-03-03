#pragma once

#include <QWidget>
#include <QMessageBox>
#include "ui_runmanualmulti.h"
#include "BaseChildWindow.h"
#include "widgets/spreadsheet.h"
#include "widgets/buttonedittablewidget.h"
#include "utils/DataTable.h"
#include "../component/tool4zeroandhalfaxis.h"

class MultiButtonsWidget;

class RunManualMulti : public BaseChildWindowWithTools
{
	Q_OBJECT

public:
	RunManualMulti(QWidget* parent = nullptr, QString objectName = "ddjg");
	~RunManualMulti();
	void DoSomething() { QMessageBox::information(this, "title", "text"); };

	QString GetGCode(bool forRun = true) override;
	QString GetGCodeV1();
	QString GetGCodeV2();
	void RunGCode() override;

	void LoadData(QString filePath = NULL) override;
	void SaveData(QString filePath = NULL) override;
	void UpdateState() override;

protected:
	Ui::RunManualMulti ui;

	ButtonEditTableWidget* table3;
	MultiButtonsWidget* mbw;

	QString m_name4RunAutoOne = "sdjg";
private:
	
	DataTable* table1;
	DataTable* table2;
	//QHash<int, int> mapLine2Row;
	int lastLine4DoneMulti = -1;
	int lastLine4DoneOne = -1;
private slots:
	void on_btnJghs_clicked();
	//void on_btnShortcutGrid1_clicked();

};


// add a decorator that allows to access the CustomObject from PythonQt
class RunManualMultiWrapper : public QObject {

	Q_OBJECT

public Q_SLOTS:
	//// add a constructor
	//CustomObject* new_CustomObject(const QString& first, const QString& last) { return new CustomObject(first, last); }

	//// add a destructor
	//void delete_CustomObject(CustomObject* o) { delete o; }

	//// add access methods
	//QString firstName(CustomObject* o) { return o->_firstName; }

	//QString lastName(CustomObject* o) { return o->_lastName; }

	void DoSomething(RunManualMulti* o) { o->DoSomething(); }
};
