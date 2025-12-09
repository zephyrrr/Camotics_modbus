#pragma once

#include <QWidget>
#include <QStackedWidget>
#include "ui_runmanual.h"
#include "BaseChildWindow.h"
#include "widgets/spreadsheet.h"
#include "widgets/buttonedittablewidget.h"
#include "../component/tool4zeroandhalfaxis.h"
#include "../other/fangdiancheshiform.h"

class MultiButtonsWidget;

class RunManual : public BaseChildWindowWithTools
{
	Q_OBJECT

public:
	RunManual(QWidget* parent = nullptr, QString objectName = "sdjg");
	~RunManual();

	void RunGCode() override;
	QString GetGCode() override;
	void LoadData(QString filePath = NULL) override;
	void SaveData(QString filePath = NULL) override;
	void UpdateState() override;

	int SetCurrentRunLine(bool initialize);
	//void LoadDataFromCCode(QString cCode);
	static QString GetGCodeOSub803(IDataTable* table2);
	//static QString GetGCodeOne(bool isAbsolutePosition, double toAxisLength[], double startAxisPositions[]);

	ButtonEditTableWidget* table1;
	ButtonEditTableWidget* table2;

protected:
	Ui::RunManualClass ui;
	
	MultiButtonsWidget* mbw;
private:
	//QButtonGroup buttonGroup4Detail;

	//SpreadSheet* table1;
	//SpreadSheet* table2;
	FangDianCanShuSingleForm* m_fangDianCanShuForm;
	int m_fangDianCanShuFormRow = -1;
	QStackedWidget* stackedChildWidget;
	QHash<int, int> mapLine2Row;
	int lastLine4DoneOne = -1;
private:
	void ShowDetailForm(int row);
	QString GetGCodeV2();
	QString GetGCodeV1();
	void SaveFanDianCanShuForm(bool invisible = true);
	bool m_bToolsInitialized = false;

private slots:
	void HideDetailForm();

protected:
	void showEvent(QShowEvent* event) override;
	void hideEvent(QHideEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
};
