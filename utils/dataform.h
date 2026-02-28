#ifndef DATAFORM_H
#define DATAFORM_H

#include <QString>
#include <QHash>


class IDataForm {
public:
	IDataForm() {}

	// Ensures to invoke actual object destructor
	virtual ~IDataForm() {}
	virtual QString getValue(const QString& caption) const = 0;
	virtual void setValue(const QString& caption, const QString& text) = 0;
};

class DataForm : public IDataForm
{
public:
	DataForm();
	~DataForm();

	QString objectName() const { return m_objectName; }
	void setObjectName(const QString& objectName) { m_objectName = objectName; }
	
	QString getValue(const QString& caption) const override;
	QString getValue(const QString& caption, bool raiseWarning) const;
	void setValue(const QString& caption, const QString& text) override;
	void deleteItem(const QString& caption);

	bool serialize(QString filePathName);
	bool deserialize(QString filePathName);

private:
	QString m_objectName;
	QHash<QString, QString> m_data;

	static QString m_emptyString;
};

class DataForms
{
private:
	static DataForms instance;
	DataForms();

public:
	static DataForms* getInstance();
	~DataForms();

	void removeDataForm(QString objectName);
	DataForm* getDataForm(QString objectName, QString projectDir=NULL);
	QString getValue(const QString& caption) const;

	void LoadData();
	void UnloadData();
private:
	QHash<QString, DataForm*> m_dataForms;
};

#endif