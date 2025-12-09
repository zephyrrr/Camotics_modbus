#pragma once

#include <QDialog>
#include <QListView>
#include <QLineEdit>
#include "BaseChildWindow.h"
#include <QFileDialog>

class NFileDialog : public QDialog // public SoftKeyboardWidget // 
{
	Q_OBJECT

public:
	NFileDialog(QWidget* parent = nullptr, const QString& caption = QString(), const QString& directory = QString(), const QString& filter = QString(), const bool canInputNewName = false);
	~NFileDialog();

	QString getSelectedFilePath() { return m_filePath; }
private:
    QListView* listView;
    QLineEdit* lineEdit;

	QString m_filePath;
    void fileSelected(const QModelIndex& index);

	QStringList m_filters;
	QStringList m_filterExtentions;
	SoftKeyboardWidget* m_keyboardWidget;

public:
	static QString getOpenFileName(QWidget* parent = nullptr, const QString& caption = QString(), const QString& dir = QString(), const QString& filter = QString());
	static QString getSaveFileName(QWidget* parent = nullptr, const QString& caption = QString(), const QString& dir = QString(), const QString& filter = QString());
	static QString findFirstRemovableDisk();
	static bool ejectUsbDrive(char driveLetter, bool force = false);
	static QString getExistingDirectory(QWidget* parent = nullptr, const QString& caption = QString(), const QString& dir = QString(), QFileDialog::Options options = QFileDialog::ShowDirsOnly);
};
