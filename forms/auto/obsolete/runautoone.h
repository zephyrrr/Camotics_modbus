#pragma once

#include <QWidget>
#include <QStackedWidget>
#include "ui_runautoone.h"
#include "BaseChildWindow.h"
#include "widgets/spreadsheet.h"
#include "widgets/buttonedittablewidget.h"
#include "../component/tool4zeroandhalfaxis.h"
#include "../other/fangdiancheshiform.h"

class RunAutoOne : public BaseChildWindow
{
	Q_OBJECT

public:
	RunAutoOne(QWidget* parent = nullptr);
	~RunAutoOne();

	void RunGCode() override;
	QString GetGCode() override;
	void LoadData(QString filePath = NULL) override;
	void SaveData(QString filePath = NULL) override;
	//void LoadDataFromCCode(QString cCode);
private:
	Ui::RunAutoOne ui;

	//SpreadSheet* table1;
	//SpreadSheet* table2;
	ButtonEditTableWidget* table1;
	ButtonEditTableWidget* table2;

	Tool4ZeroAndHalfAxis* m_tool4ZeroAndHalfAxis = NULL;
	FangDianCanShuSingleForm* m_fangDianCanShuForm;
	int m_fangDianCanShuFormRow = -1;
	QStackedWidget* stackedChildWidget;
	QHash<int, int> mapLine2Row;
private:
	void ShowDetail(int row);
	QString GetGCodeV2();
	QString GetGCodeV1();
	void SaveFanDianCanShuForm(bool invisible = true);
protected:
	void showEvent(QShowEvent* event) override;
	void hideEvent(QHideEvent* event) override;

};
