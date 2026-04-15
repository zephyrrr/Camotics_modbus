#pragma once

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include "ui_jiagongjiluform.h"
#include "BaseChildWindow.h"
#include "modbus/NCMachineProperties.h"

// Work database path is now in DataDir (configured via qGlobal.ini)
inline QString GetWorkDbPath() { return SystemSettings::GetPath("work.db", SystemSettings::UserFlag); }

class JiaGongJiLuForm : public BaseChildWindow
{
	Q_OBJECT

public:
	JiaGongJiLuForm(QWidget *parent = nullptr);
	
	~JiaGongJiLuForm();

public:
	void RunGCode() override {};

private slots:
	void on_prevPageButton_clicked();
	void on_nextPageButton_clicked();
	void updatePaginationControls();
	void handleTableViewMasterChanged(const QItemSelection& selected, const QItemSelection& deselected);

protected:
	void showEvent(QShowEvent* event) override;

private:
	Ui::JiaGongJiLuFormClass ui;

	QSqlQueryModel* model;
	QSqlQueryModel* modelDetail;

	int currentPage = 1;
	int rowsPerPage = 10;
	QString currentTable;
	//QString m_workDbPath;

	void openDatabase();
	void loadTableData();
	void loadTableDataDetail(int parentId);
	int getTotalRowCount(const QString& tableName);

public:
	static bool createDb();

	static qint64 addRecordWork(QString name, QDateTime startDate, QDateTime endDate, int workSeconds);
	static void addRecordWorkDetail(QString cNo, QDateTime startDate, QDateTime endDate, int workSeconds, qint64 parentId);
};
