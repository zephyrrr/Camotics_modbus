#ifndef DATATABLE_H
#define DATATABLE_H

#include <QVector>
#include <QString>

class IDataTable {
public:
    IDataTable() {}

    // Ensures to invoke actual object destructor
    virtual ~IDataTable() {}

    virtual QString getValue(int row, int col) const = 0;
    virtual void setValue(int row, int col, const QString& value) = 0;

    virtual int getDataCount() const = 0;
    virtual int getDataYCount() = 0;
};

class DataTable : public IDataTable {
public:
    DataTable(int rows = 0, int columns = 0);

    QString getValue(int row, int col) const override;
    void setValue(int row, int col, const QString& value) override;
    bool serialize(const QString& fileName) const;
    bool deserialize(const QString& fileName);

    int getDataCount() const override { return m_data.length() - 1; }
    int getDataYCount() override { return m_data[0].length() - 1; }
private:
    QVector<QVector<QString>> m_data;
};

#endif // DATATABLE_H



