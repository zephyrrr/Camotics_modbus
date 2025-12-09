#include "jiagongjiluform.h"
#include <QTableView>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
#include <QDateTime>
#include "utils/DatabaseManager.h"

JiaGongJiLuForm::JiaGongJiLuForm(QWidget *parent)
	: BaseChildWindow(parent)
{
	ui.setupUi(this);

	//createDb();

    model = new QSqlQueryModel(this);
    ui.tableView->setModel(model);

	modelDetail = new QSqlQueryModel(this);
	ui.tableViewDetail->setModel(modelDetail);

    // Disable buttons initially
    ui.prevPageButton->setEnabled(false);
    ui.nextPageButton->setEnabled(false);

    QItemSelectionModel* selectionModelA = ui.tableView->selectionModel();
    connect(selectionModelA, &QItemSelectionModel::selectionChanged,
        this, &JiaGongJiLuForm::handleTableViewMasterChanged);

    openDatabase();
    ui.tableView->hideColumn(4);

}

void JiaGongJiLuForm::handleTableViewMasterChanged(const QItemSelection& selected, const QItemSelection& deselected)
{
    if (!selected.indexes().isEmpty()) {
        QModelIndex selectedIndex = selected.indexes().first();
        
        QAbstractItemModel* modelA = ui.tableView->model();
        if (modelA) {
            //QVariant dataFromA = modelA->data(selectedIndex, Qt::DisplayRole); // Or another role

            QModelIndex columnTwoIndex = modelA->index(selectedIndex.row(), 4, selectedIndex.parent());
            QVariant columnTwoData = modelA->data(columnTwoIndex, Qt::DisplayRole);

            loadTableDataDetail(columnTwoData.toInt());
        }
    }
    else {
        loadTableDataDetail(-1);
    }
}

void JiaGongJiLuForm::showEvent(QShowEvent* event)
{
    openDatabase();

    BaseChildWindow::showEvent(event);
}

JiaGongJiLuForm::~JiaGongJiLuForm()
{
}


void JiaGongJiLuForm::openDatabase()
{
    DatabaseManager& dbManager = DatabaseManager::instance();
    QSqlDatabase db = dbManager.getDatabase(WORK_DB_PATH);

    //if (db.isOpen()) {
    //    db.close();
    //}

    //if (!db.open()) {
    //    QMessageBox::critical(this, "Error", "Could not open database: " + db.lastError().text());
    //    return;
    //}

    currentTable = "WORK_MASTER";
    currentPage = 1;
    loadTableData();
    updatePaginationControls();
}


void JiaGongJiLuForm::loadTableData()
{
    if (currentTable.isEmpty()) {
        return;
    }

    DatabaseManager& dbManager = DatabaseManager::instance();
    QSqlDatabase db = dbManager.getDatabase(WORK_DB_PATH);

    int offset = (currentPage - 1) * rowsPerPage;
    // strftime('%H:%M:%S', datetime((julianday(end_date) - julianday(start_date)) * 86400, 'unixepoch')) AS 总时间
    QString query = QStringLiteral("SELECT name AS 名称, strftime('%Y-%m-%d %H:%M:%S', start_date) AS 开始时间, strftime('%Y-%m-%d %H:%M:%S', end_date) AS 结束时间, strftime('%H:%M:%S', datetime(time, 'unixepoch')) AS 总时间, id FROM %1 ORDER BY ID DESC LIMIT %2 OFFSET %3")
        .arg(currentTable)
        .arg(rowsPerPage)
        .arg(offset); 

    model->setQuery(query, db);

    //if (model->lastError().isValid()) {
    //    QMessageBox::critical(this, "Error", "Query failed: " + model->lastError().text());
    //    return;
    //}
    ui.tableView->resizeColumnsToContents();
    ui.tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void JiaGongJiLuForm::loadTableDataDetail(int parentId)
{
    DatabaseManager& dbManager = DatabaseManager::instance();
    QSqlDatabase db = dbManager.getDatabase(WORK_DB_PATH);

    QString query = QStringLiteral("SELECT cNo AS cNo, strftime('%H:%M:%S', datetime(time, 'unixepoch')) AS 时间 FROM %1 WHERE parent = %2 ORDER BY ID ASC")
        .arg("WORK_DETAIL").arg(parentId);

    modelDetail->setQuery(query, db);
    //if (modelDetail->lastError().isValid()) {
    //    QMessageBox::critical(this, "Error", "Query failed: " + modelDetail->lastError().text());
    //    return;
    //}

    ui.tableViewDetail->resizeColumnsToContents();
    ui.tableViewDetail->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

int JiaGongJiLuForm::getTotalRowCount(const QString& tableName)
{
    DatabaseManager& dbManager = DatabaseManager::instance();
    QSqlDatabase db = dbManager.getDatabase(WORK_DB_PATH);

    QSqlQuery query(db);
    query.prepare(QString("SELECT COUNT(*) FROM %1").arg(tableName));

    if (!query.exec()) {
        QMessageBox::critical(this, "Error", "Count query failed: " + query.lastError().text());
        return 0;
    }

    if (query.next()) {
        return query.value(0).toInt();
    }

    return 0;
}

bool JiaGongJiLuForm::createDb()
{
	//if (WORK_DB_PATH == ":memory:") {
	//	return true;
	//}
    bool dbShouldInit = WORK_DB_PATH == ":memory:" || !QFile::exists(WORK_DB_PATH);

    DatabaseManager& dbManager = DatabaseManager::instance();

    // Add database connections
    QVariantMap sqliteOptions;
    sqliteOptions["databaseName"] = WORK_DB_PATH;
    dbManager.addDatabase("QSQLITE", WORK_DB_PATH, sqliteOptions);

    //if (QFile::exists(WORK_DB_PATH))
    //    return true;

	QSqlDatabase db = dbManager.getDatabase(WORK_DB_PATH);

    if (!db.open()) {
        //QMessageBox::critical(this, "Error", "Could not open database: " + db.lastError().text());
        return false;
    };

    try 
    {
       
        if (dbShouldInit)
        {
            QSqlQuery query(db);
            bool b = query.exec("CREATE TABLE WORK_MASTER (id INTEGER PRIMARY KEY, name TEXT NOT NULL, start_date TEXT NOT NULL, end_date TEXT NOT NULL, time INTEGER NOT NULL)");
            if (!b) {
                QMessageBox::critical(NULL, "Error", "Could not create db table: " + db.lastError().text());
            }

            b = query.exec("CREATE TABLE WORK_DETAIL (id INTEGER PRIMARY KEY, cNo TEXT NOT NULL, start_date TEXT NOT NULL, end_date TEXT NOT NULL, parent INTEGER NOT NULL, time INTEGER NOT NULL)");
            if (!b) {
                QMessageBox::critical(NULL, "Error", "Could not create db table: " + db.lastError().text());
            }
        }

        if (WORK_DB_PATH == ":memory:") {
            QSqlQuery query(db);
            query.exec("INSERT INTO WORK_MASTER (id, name, start_date, end_date) VALUES (1, 'Alice', '2024-10-01 00:00:00.000', '2024-10-01 01:00:01.000')");
            query.exec("INSERT INTO WORK_MASTER (id, name, start_date, end_date) VALUES (2, 'Bob', '2024-10-01 00:00:00.000', '2024-11-01 01:00:02.000')");
            query.exec("INSERT INTO WORK_MASTER (id, name, start_date, end_date) VALUES (3, 'Alice', '2024-10-01 00:00:00.000', '2024-10-01 01:00:03.000')");
            query.exec("INSERT INTO WORK_MASTER (id, name, start_date, end_date) VALUES (4, 'Bob', '2024-10-01 00:00:00.000', '2024-10-01 01:00:04.000')");
            query.exec("INSERT INTO WORK_MASTER (id, name, start_date, end_date) VALUES (5, 'Alice', '2024-10-01 00:00:00.000', '2024-10-01 01:00:05.000')");
            query.exec("INSERT INTO WORK_MASTER (id, name, start_date, end_date) VALUES (6, 'Bob', '2024-10-01 00:00:00.000', '2024-10-01 01:00:06.000')");
            query.exec("INSERT INTO WORK_MASTER (id, name, start_date, end_date) VALUES (7, 'Alice', '2024-10-01 00:00:00.000', '2024-10-01 01:00:07.000')");
            query.exec("INSERT INTO WORK_MASTER (id, name, start_date, end_date) VALUES (8, 'Bob', '2024-10-01 00:00:00.000', '2024-10-01 01:00:08.000')");

            query.exec("INSERT INTO WORK_DETAIL (cNo, start_date, end_date, parent) VALUES ('001', '2024-10-01 00:00:00.000', '2024-10-01 01:00:01.000', 1)");
            query.exec("INSERT INTO WORK_DETAIL (cNo, start_date, end_date, parent) VALUES ('002', '2024-10-01 01:00:00.000', '2024-10-01 02:00:02.000', 1)");

            query.exec("INSERT INTO WORK_DETAIL (cNo, start_date, end_date, parent) VALUES ('003', '2024-10-02 00:00:00.000', '2024-10-02 01:00:01.000', 2)");
            query.exec("INSERT INTO WORK_DETAIL (cNo, start_date, end_date, parent) VALUES ('004', '2024-10-02 01:00:00.000', '2024-10-02 02:00:02.000', 2)");

        }
    }
	catch (const std::exception& e) {
		return false;
	}
    //db.close();

    return true;
}

qint64 JiaGongJiLuForm::addRecordWork(QString name, QDateTime startDate, QDateTime endDate, int workSeconds)
{
    DatabaseManager& dbManager = DatabaseManager::instance();
    QSqlDatabase db = dbManager.getDatabase(WORK_DB_PATH);
    if (!db.isValid()) {
        return -1;
    }
    QSqlQuery query(db);
    QString sql = QString("INSERT INTO WORK_MASTER (name, start_date, end_date, time) VALUES ('%1', '%2', '%3', %4)")
        .arg(name).arg(startDate.toString("yyyy-MM-dd hh:mm:ss")).arg(endDate.toString("yyyy-MM-dd hh:mm:ss")).arg(workSeconds);
    bool b = query.exec(sql);
    if (!b) {
        QSqlError error = query.lastError();
        qDebug() << "SQL Error:";
        qDebug() << "  Type:" << error.type();
        qDebug() << "  Number:" << error.number();
        qDebug() << "  Database Text:" << error.databaseText();
        qDebug() << "  Driver Text:" << error.driverText();

        return -1;
    }
    QVariant lastId = query.lastInsertId();

    if (lastId.isValid()) {
        qint64 newRecordId = lastId.toLongLong();
        return newRecordId;
    }
    else {
        return -1;
    }
}

void JiaGongJiLuForm::addRecordWorkDetail(QString cNo, QDateTime startDate, QDateTime endDate, int workSeconds, qint64 parentId)
{
    DatabaseManager& dbManager = DatabaseManager::instance();
    QSqlDatabase db = dbManager.getDatabase(WORK_DB_PATH);

    QSqlQuery query(db);
    QString sql = QString("INSERT INTO WORK_DETAIL (cNo, start_date, end_date, time, parent) VALUES ('%1', '%2', '%3', %4, %5)")
        .arg(cNo).arg(startDate.toString("yyyy-MM-dd hh:mm:ss")).arg(endDate.toString("yyyy-MM-dd hh:mm:ss")).arg(workSeconds).arg(parentId);
    query.exec(sql);
}

void JiaGongJiLuForm::on_prevPageButton_clicked()
{
    if (currentPage > 1) {
        currentPage--;
        loadTableData();
        updatePaginationControls();
    }
}

void JiaGongJiLuForm::on_nextPageButton_clicked()
{
    int totalRows = getTotalRowCount(currentTable);
    int maxPages = (totalRows + rowsPerPage - 1) / rowsPerPage;

    if (currentPage < maxPages) {
        currentPage++;
        loadTableData();
        updatePaginationControls();
    }
}

void JiaGongJiLuForm::updatePaginationControls()
{
    if (currentTable.isEmpty()) {
        ui.prevPageButton->setEnabled(false);
        ui.nextPageButton->setEnabled(false);
        ui.pageLabel->setText("No table selected");
        return;
    }

    int totalRows = getTotalRowCount(currentTable);
    int maxPages = (totalRows + rowsPerPage - 1) / rowsPerPage;

    ui.prevPageButton->setEnabled(currentPage > 1);
    ui.nextPageButton->setEnabled(currentPage < maxPages);

    ui.pageLabel->setText(QStringLiteral("第%1/%2页 (总共记录数: %3)")
        .arg(currentPage)
        .arg(maxPages)
        .arg(totalRows));
}