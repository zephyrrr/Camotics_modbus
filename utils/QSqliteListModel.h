#ifndef SQLITELISTMODEL_H
#define SQLITELISTMODEL_H

#include <QAbstractListModel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QList>
#include <QVariant>
class QSqliteListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit QSqliteListModel(QObject* parent = nullptr);

    // Custom method to connect and fetch data
    bool loadFromDatabase(const QString& dbPath, const QString& tableName, const QString& columnName);

    // QAbstractListModel Overrides
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

private:
    QSqlDatabase m_db;
    QList<QVariant> m_data;
    QString m_columnName;
};


#endif // SQLITELISTMODEL_H
