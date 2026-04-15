#pragma once

#include <QTableWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include "qlineedit4axisvalue.h"
#include "utils/DataTable.h"

class ButtonEditTableWidget  : public QTableWidget, public IDataTable
{
	Q_OBJECT

public:
	ButtonEditTableWidget(QWidget* parent = NULL);
	~ButtonEditTableWidget();

	void createForm(int rows, int columns, std::function<ILineEdit* (int, QWidget*)> function4CreateLineEdit = NULL);

	void ScrollToRow(int row);
	void setReadOnly(bool readonly);

	// row是原始index
	void insertNewRow(int row = -1);
	void deleteRow(int row = -1);
	void copyRow(int row = -1);
	void pasteRow(int row = -1);

	void setReadOnlyOnRow(int row, bool readonly);
	void setRowSelection(int row, bool selected);
	void setPropertyOnRow(int row, const char* name, const QVariant& value);
	//void enableAddNewRow(bool b);
	//void enableDeleteRow(bool b);

	static QJsonArray GetData(ButtonEditTableWidget* parent, bool asMap = false);
	bool serialize(const QString& filePathName);
	bool deserialize(const QString& filePathName);

	void setReadOnlyOnCell(int row, int col, bool readonly);
	// row是数据Index，=原始Index+1
	QString getValue(int row, int col) const override;
	void setValue(int row, int col, const QString& value) override;
	
	// 最上面有一行QLabel，用于显示列名
	int getDataCount()  const override { return this->rowCount() - 1; }
	// 最左边有一列QPushButton，用于显示行号和选择
	int getDataYCount() override { return this->columnCount() - 1; }

	bool isReadOnly() { return m_isReadOnly; }

	int getRowOfEdit(int col, ILineEdit* edit);
private:
	ILineEdit* createEdit(int col, QWidget* parent);
	std::function<ILineEdit* (int, QWidget*)> m_function4CreateEdit;
	bool m_isReadOnly = false;

private slots:

signals:
	void rowButtonClicked(int row, int checked=false);
	void rowInserted(int row);
	//void currentRowChanged(int row, bool selected);

private:
	QButtonGroup* buttonGroup;
	QList<QLabel*> inLabels;
	QList<QPushButton*> inButtons;
	QList<QList<ILineEdit*>> inEdits;
	//int m_currentRow = -1;
	bool m_enableAddNew = false;
	bool m_enableDelete = false;
	bool m_enableMultiSelection = true;
	QList<QString> m_copyInfos;
	int m_initialRowCount = 0; // 用于记录初始行数，避免在deserialize时删除行
};

class ButtonEditTableWidgetWrapper : public QObject {

	Q_OBJECT

public Q_SLOTS:
	//// add a constructor
	//CustomObject* new_CustomObject(const QString& first, const QString& last) { return new CustomObject(first, last); }

	//// add a destructor
	//void delete_CustomObject(CustomObject* o) { delete o; }

	void insertNewRow(ButtonEditTableWidget* o, int row = -1) { o->insertNewRow(row); }
	void deleteRow(ButtonEditTableWidget* o, int row = -1) { o->deleteRow(row); }

	QString getValue(ButtonEditTableWidget* o, int row, int col) { return o->getValue(row, col); }
	void setValue(ButtonEditTableWidget* o, int row, int col, QString value) { o->setValue(row, col, value); }

	int getDataCount(ButtonEditTableWidget* o) { return o->getDataCount(); }
	int getDataYCount(ButtonEditTableWidget* o) { return o->getDataYCount(); }
};