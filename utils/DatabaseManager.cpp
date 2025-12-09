#include "DatabaseManager.h"
#include <QTableView>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>




DatabaseManager::~DatabaseManager() {
    QMutexLocker locker(&mutex);
    for (auto it = databaseConnections.begin(); it != databaseConnections.end(); ++it) {
        if (it.value().isOpen()) {
            it.value().close();
        }
        QSqlDatabase::removeDatabase(it.key());
    }
    databaseConnections.clear();
}

bool DatabaseManager::addDatabase(const QString& type, const QString& connectionName, const QVariantMap& connectionOptions) {
    QMutexLocker locker(&mutex);
    if (databaseConnections.contains(connectionName)) {
        qDebug() << "Connection with name '" << connectionName << "' already exists.";
        return false;
    }

    QSqlDatabase db = QSqlDatabase::addDatabase(type, connectionName);

    // Set specific options based on the database type
    if (type == "QSQLITE") {
        db.setDatabaseName(connectionOptions.value("databaseName").toString());
    }
    else if (type == "QPSQL") {
        db.setHostName(connectionOptions.value("hostName").toString());
        db.setDatabaseName(connectionOptions.value("databaseName").toString());
        db.setUserName(connectionOptions.value("userName").toString());
        db.setPassword(connectionOptions.value("password").toString());
        // ... other PostgreSQL options
    }
    // ... other database types

    if (!db.open()) {
        qDebug() << "Error opening database '" << connectionName << "':" << db.lastError().text();
        QSqlDatabase::removeDatabase(connectionName);
        return false;
    }
    databaseConnections[connectionName] = db;
    return true;
}

QSqlDatabase DatabaseManager::getDatabase(const QString& connectionName) const {
    QMutexLocker locker(const_cast<QMutex*>(&mutex)); // Lock for read access
    if (databaseConnections.contains(connectionName)) {
        return databaseConnections[connectionName];
    }
    qDebug() << "Connection with name '" << connectionName << "' not found.";
    return QSqlDatabase(); // Return an invalid QSqlDatabase
}

bool DatabaseManager::executeQuery(const QString& connectionName, const QString& queryStr) {
    QSqlDatabase db = getDatabase(connectionName);
    if (db.isValid() && db.isOpen()) {
        QSqlQuery query(db);
        if (!query.exec(queryStr)) {
            qDebug() << "Error executing query on database '" << connectionName << "':" << query.lastError().text();
            return false;
        }
        return true;
    }
    qDebug() << "Database '" << connectionName << "' is not open or valid.";
    return false;
}
