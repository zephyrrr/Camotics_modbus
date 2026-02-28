#include "LocaleUtils.h"
#include <QFile>
#include <QFileInfo>
#include <QLocale>
#include <QDir>
QString LocaleUtils::FindFileByLocale(QString filePath)
{
	QFileInfo fileInfo(filePath);
	QString path = fileInfo.path();
	QString baseName = fileInfo.baseName();
	QString completeSuffix = fileInfo.completeSuffix();

	//QLocale systemLocale = QLocale::system();
	QLocale curLocale;

	QString localeSuffix = curLocale.name(); // e.g., "en_US"
	QString newFilePath = path + "/" + baseName + "_" + localeSuffix + "." + completeSuffix;
	if (QFile::exists(newFilePath))
	{
		return QDir::toNativeSeparators(newFilePath);
	}

	localeSuffix = localeSuffix.split("_")[0];
	newFilePath = path + "/" + baseName + "_" + localeSuffix + "." + completeSuffix;
	if (QFile::exists(newFilePath))
	{
		return QDir::toNativeSeparators(newFilePath);;
	}

	newFilePath = filePath;
	if (QFile::exists(newFilePath))
	{
		return QDir::toNativeSeparators(newFilePath);;
	}

	return QString();
}
