#pragma once

#include <QWidget>
#include <QLabel>
#include <QLineEdit>

class EditableTextWidget : public QWidget
{
	Q_OBJECT

public:
	EditableTextWidget(const QString& text, QWidget* parent = nullptr);
	~EditableTextWidget();

	QString getParamValue(const QString& name) const { return m_paramValues.value(name); }
	static QString removeATags(QString& text);

private slots:
	void handleLinkActivated(const QString& link);
	void handleEditingFinished();

private:
	QLabel* label;
	QLineEdit* lineEdit = NULL;
	QHash<QString, QString> m_paramValues;
	QString m_nowEditName, m_nowEditValue;
	void setupUI(const QString& text);
};
