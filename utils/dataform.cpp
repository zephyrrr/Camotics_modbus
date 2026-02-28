#include "dataform.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QDir>
#include "modbus/NCMachineProperties.h"
#include <cbang/log/Logger.h>
#include "utils/nfile.h"

DataForm::DataForm()
{}

DataForm::~DataForm()
{}

QString DataForm::m_emptyString = "";

QString DataForm::getValue(const QString& caption) const
{
	return getValue(caption, true);
}

QString DataForm::getValue(const QString& caption, bool raiseWarning) const
{
    if (m_data.contains(caption))
        return m_data[caption];
    else {
        if (raiseWarning) {
            LOG_WARNING("No data value in DataForm: " << caption.toUtf8().constData());
        }
        return m_emptyString;
    }
}

void DataForm::setValue(const QString& caption, const QString& text)
{
	m_data[caption] = text;
}

void DataForm::deleteItem(const QString& caption)
{
    if (m_data.contains(caption))
        m_data.remove(caption);
}

bool DataForm::serialize(QString filePathName)
{
    try {
        QJsonObject data;
        for each (QString caption in m_data.keys())
        {
            data[caption] = getValue(caption);
        }

        QJsonDocument doc(data);
        QString json = doc.toJson();

        NFile file(filePathName);
        file.open(QIODevice::WriteOnly);
        file.write(json.toUtf8());
        file.close();
    }
    catch (...) {
        return false;
    }
    return true;
}

bool DataForm::deserialize(QString filePathName)
{
    try {
        if (QFile::exists(filePathName)) {
            QFile file(filePathName);
            file.open(QIODevice::ReadOnly);
            QString json = file.readAll();
            file.close();

            QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
            QJsonObject data = doc.object();

            for each (QString caption in data.keys())
            {
                QJsonValue value = data[caption];
                if (value.isBool()) {
                    m_data[caption] = QVariant(value.toBool()).toString();
                }
                else {
                    m_data[caption] = value.toString();
                }
            }
        }
    }
    catch (...) {
        return false;
    }
    return true;
}


DataForms* DataForms::getInstance() {
	return &instance;
}

DataForms::DataForms()
{
    LoadData();
}
DataForms::~DataForms() {
    UnloadData();
}

void DataForms::removeDataForm(QString objectName)
{
    if (m_dataForms.contains(objectName)) {
        delete m_dataForms[objectName];
        m_dataForms.remove(objectName);
    }
}

DataForm* DataForms::getDataForm(QString objectName, QString projectDir)
{
    DataForm* ret;
    if (m_dataForms.contains(objectName)) {
        ret = m_dataForms[objectName];
    }
	else
    {
        ret = new DataForm();
        ret->setObjectName(objectName);
        m_dataForms[objectName] = ret;

        QString filePathName = SystemSettings::GetDataFilePath(ret->objectName(), projectDir);
        ret->deserialize(filePathName);
    }

    return ret;
}

void DataForms::LoadData()
{
    // #define DYNAMIC_FORMS_COUNT 5
    for (int i = 0; i < 5; ++i) {
        getDataForm(QString("xitongshezhi%01").arg(i == 0 ? "" : QString::number(i + 1)));
    }
}

void DataForms::UnloadData()
{
    for (const QString& key : m_dataForms.keys()) {
        delete m_dataForms[key];
    }
	m_dataForms.clear();
}

QString DataForms::getValue(const QString& caption) const
{
    QString ret;
    for (DataForm* form : m_dataForms.values())
    {
		ret = form->getValue(caption, false);
		if (!ret.isEmpty())
			return ret;
    }
    return "";
}

DataForms DataForms::instance;