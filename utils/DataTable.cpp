#include "DataTable.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include "utils/nfile.h"

DataTable::DataTable(int rows, int columns)
    : m_data(rows, QVector<QString>(columns)) {}

QString DataTable::getValue(int row, int col) const {
    if (row < 0 || row >= m_data.size() - 1 || col < -1 || col >= m_data[row].size() - 1) {
		return QString::null;
	}
    return m_data[row + 1][col + 1];
}

void DataTable::setValue(int row, int col, const QString& value) {
    if (row < 0 || row >= m_data.size() - 1 || col < -1 || col >= m_data[row].size() - 1) {
        return;
    }
    m_data[row + 1][col + 1] = value;
}

bool DataTable::serialize(const QString& fileName) const {
    QJsonArray data;
    for (const QVector<QString>& row : m_data) {
        QJsonArray rowData;
        for (const QString& cell : row) {
            rowData.append(cell);
        }
        data.append(rowData);
    }

    QJsonDocument doc(data);
    QString json = doc.toJson();

    NFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }
    file.write(json.toUtf8());
    file.close();

    return true;
}

bool DataTable::deserialize(const QString& fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }
    QString json = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
    QJsonArray data = doc.array();

    m_data.clear();
    for (const QJsonValue& rowDataValue : data) {
        QJsonArray rowData = rowDataValue.toArray();
        QVector<QString> row;
        for (const QJsonValue& cellValue : rowData) {
            row.append(cellValue.toString());
        }
        m_data.append(row);
    }

    return true;
}
