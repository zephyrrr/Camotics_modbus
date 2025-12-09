#include "QTextFileModel.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

QTextFileModel::QTextFileModel(QObject* parent, const QString& filePath)
    : QStringListModel(parent)
{
	loadFromFile(filePath);
}

// ----------------------------------------
// LOAD (Read content from disk)
// ----------------------------------------
bool QTextFileModel::loadFromFile(const QString& filePath)
{
    if (filePath.isEmpty()) {
        qWarning() << "Error: File path is empty.";
        return false;
	}
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Error: Could not open file for reading:" << filePath << file.errorString();
        return false;
    }

    QStringList list;
    QTextStream in(&file);
    in.setCodec("UTF-8");

    // Read the file line by line
    while (!in.atEnd()) {
        QString s = tr(in.readLine().toUtf8().constData());
        //QT_TR_NOOP("Network Error");

        if (!s.isEmpty()) {
            list << s;
        }
    }

    file.close();
    m_filePath = filePath; // Store the path for saving

    // Set the internal QStringList, which automatically updates the view
    setStringList(list);

    return true;
}

// ----------------------------------------
// SAVE (Write current data to disk)
// ----------------------------------------
bool QTextFileModel::saveToFile(const QString& filePath) const
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        qWarning() << "Error: Could not open file for writing:" << filePath << file.errorString();
        return false;
    }

    QTextStream out(&file);
    out.setCodec("UTF-8");
    QStringList list = stringList(); // Get the current data from the model

    // Write each string (line) to the file
    for (const QString& line : list) {
        out << line << "\n";
    }

    file.close();
    return true;
}



// Note: QSqlTableModel handles all the abstract model methods (rowCount, data, setData, etc.)
// We only need to implement the database connection and table setup logic.

QSqliteModel::QSqliteModel(QObject* parent, const QString& dbPath, const QString& sqlQuery)
	: QStringListModel(parent), m_dbPath(dbPath), m_sqlQuery(sqlQuery)
{
    loadFromDatabase();
}

QSqliteModel::~QSqliteModel()
{
    // Ensure the database connection is cleanly closed and removed when the model is destroyed.
    if (m_db.isOpen()) {
        QString connName = m_db.connectionName();
        m_db.close();
        QSqlDatabase::removeDatabase(connName);
    }
}

/**
 * @brief Loads the data from the database table into the model.
 * @return True if the selection was successful.
 */
bool QSqliteModel::loadFromDatabase()
{
    QStringList list;
    
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(m_dbPath); // Replace with your file path

    if (!db.open()) {
        qDebug() << "Error opening database:" << db.lastError().text();
        return false;
    }

    QSqlQuery query;
    query.prepare(m_sqlQuery);
    if (!query.exec()) {
        m_db.close();
        qDebug() << "Select query failed:" << query.lastError().text();
        return false;
    }

    // query.next() moves the internal pointer to the next record and returns true if successful.
    while (query.next()) {
        // You can get the value by column index (0 for the first column) or column name.
        // Get by index (0 in this case, since we only selected 'name')
        QString s = query.value(0).toString();

        list << s;
    }
    db.close();

    // Set the internal QStringList, which automatically updates the view
    setStringList(list);

    return true;
}

