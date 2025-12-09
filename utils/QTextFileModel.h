#ifndef TEXTFILEMODEL_H
#define TEXTFILEMODEL_H

#include <QStringListModel>
#include <QObject>
#include <QString>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

class QTextFileModel : public QStringListModel
{
    Q_OBJECT
public:
    explicit QTextFileModel(QObject* parent = nullptr, const QString& filePath = nullptr);

    // New methods to handle file operations
    bool loadFromFile(const QString& filePath);
    bool saveToFile(const QString& filePath) const;

private:
    QString m_filePath;
};


/**
 * @brief An editable list model that stores and manages data using an SQLite database
 * but maintains data in memory (via QStringListModel) while open for performance.
 *
 * This class mimics the behavior of QTextFileModel, replacing file I/O with database I/O.
 */
class QSqliteModel : public QStringListModel
{
    Q_OBJECT
public:
    // Takes the database path instead of the file path.
    explicit QSqliteModel(QObject* parent = nullptr, const QString& dbPath = nullptr, const QString& sqlQuery = nullptr);
    ~QSqliteModel();

    bool loadFromDatabase();

private:
    
    QSqlDatabase m_db;
    QString m_dbPath;
    QString m_sqlQuery;
};

#endif // TEXTFILEMODEL_H

