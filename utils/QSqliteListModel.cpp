#include "QSQLiteListModel.h"
#include <QDebug>

QSqliteListModel::QSqliteListModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

bool QSqliteListModel::loadFromDatabase(const QString& dbPath, const QString& tableName, const QString& columnName)
{
    // Clean up previous data
    beginResetModel();
    m_data.clear();
    endResetModel();

    // 1. Set up the database connection
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(dbPath);

    if (!m_db.open()) {
        qCritical() << "Error: Could not open database:" << m_db.lastError().text();
        return false;
    }

    // 2. Prepare and execute the query
    QString queryStr = QString("SELECT %1 FROM %2").arg(columnName).arg(tableName);
    QSqlQuery query(queryStr, m_db);

    if (!query.exec()) {
        qCritical() << "Error executing query:" << query.lastError().text();
        m_db.close();
        return false;
    }

    // 3. Fetch data into the model's internal list
    beginInsertRows(QModelIndex(), 0, 0); // Prepare the model for changes
    while (query.next()) {
        // The first column (index 0) of the result set is our target column
        m_data.append(query.value(0));
    }
    endInsertRows();

    m_db.close(); // Close connection after fetching

    return true;
}

int QSqliteListModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return m_data.count();
}

QVariant QSqliteListModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= m_data.count())
        return QVariant();

    if (role == Qt::DisplayRole) {
        return m_data.at(index.row());
    }
    // Handle other roles (like EditRole, DecorationRole) here if needed

    return QVariant();
}
