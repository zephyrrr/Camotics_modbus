#include <QDir>
#include <QFileInfo>
#include <QSettings>
#include <QFile>
#include "SystemSettings.h"
#include "utils/nfile.h"

void SystemSettings::SetProjectName(const QString& projectName)
{
	if (projectName.isEmpty())
		return;
	if (m_projectName == projectName)
		return;

	QString projectDir = GetUserDataDir() + "/" + projectName;
	QDir dir(projectDir);
	bool dirExist = false;
	if (!dir.exists())
	{
		dirExist = dir.mkpath(projectDir);
	}
	else
	{
		dirExist = true;
	}
	if (dirExist)
	{
		m_projectName = projectName;
	}
}

QString SystemSettings::GetPath(const QString& fileName, DataDirType dirFlags)
{
	// Priority order: Project > User > System
	QList<DataDirFlag> priorityOrder = { ProjectFlag, UserFlag, SystemFlag };

	for (DataDirFlag flag : priorityOrder) {
		if (dirFlags & flag) {
			QString baseDir;
			switch (flag) {
				case ProjectFlag:
					baseDir = SystemSettings::instance().GetProjectDir();
					break;
				case UserFlag:
					baseDir = SystemSettings::instance().GetUserDataDir();
					break;
				case SystemFlag:
					baseDir = SystemSettings::instance().GetSystemDataDir();
					break;
			}
			QString filePath = CombinePath(baseDir, fileName);
			// If only one flag, return path directly
			if (dirFlags == flag) {
				return filePath;
			}
			// If file exists, return it
			if (QFile::exists(filePath)) {
				return filePath;
			}
		}
	}

	// If no existing file found, return the first valid path
	for (DataDirFlag flag : priorityOrder) {
		if (dirFlags & flag) {
			QString baseDir;
			switch (flag) {
				case ProjectFlag:
					baseDir = SystemSettings::instance().GetProjectDir();
					break;
				case UserFlag:
					baseDir = SystemSettings::instance().GetUserDataDir();
					break;
				case SystemFlag:
					baseDir = SystemSettings::instance().GetSystemDataDir();
					break;
			}
			return CombinePath(baseDir, fileName);
		}
	}

	return fileName;
}

QString SystemSettings::GetDataFilePath(const QString& objectName, DataDirType dirFlags)
{
	QString fileName = QString("%1_data.json").arg(objectName);
	return GetPath(fileName, dirFlags);
}

QString SystemSettings::AppendDataFilePath(const QString& filePath, const QString& append)
{
	int dotIndex = filePath.lastIndexOf('.');
	if (dotIndex >= 5 && filePath.mid(dotIndex - 5, 5) == "_data")
		dotIndex -= 5;
	QString newFilePath = filePath;
	newFilePath.insert(dotIndex, "_s_" + append);
	return newFilePath;
}

void SystemSettings::LoadFromFile(const QString& filePath)
{
	if (filePath.endsWith(".ini")) {
		QSettings settings(filePath, QSettings::IniFormat);
		for (const QString& key : settings.allKeys()) {
			SetValue(key, settings.value(key).toString());
		}
	}
}

// Helper function for file migration (supports cross-drive)
static bool migrateFile(const QString& oldPath, const QString& newPath)
{
	if (QFile::copy(oldPath, newPath)) {
		QFile::remove(oldPath);
		return true;
	}
	return false;
}

// Helper function for directory migration
static bool migrateDirectory(const QString& oldDir, const QString& newDir)
{
	if (!QDir().mkpath(newDir))
		return false;
	QDir dir(oldDir);
	QStringList entries = dir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
	bool allMigrated = true;
	for (const QString& entry : entries) {
		QString oldPath = oldDir + "/" + entry;
		QString newPath = newDir + "/" + entry;
		QFileInfo info(oldPath);
		if (info.isDir()) {
			if (!migrateDirectory(oldPath, newPath))
				allMigrated = false;
		} else if (!QFile::exists(newPath)) {
			if (!migrateFile(oldPath, newPath))
				allMigrated = false;
		}
	}
	if (allMigrated) {
		QDir().rmdir(oldDir);
	}
	return true;
}

void SystemSettings::MigrateUserDataDir()
{
	NFile::cleanStaleTempFiles(GetPath("", UserFlag));
	NFile::cleanStaleTempFiles(GetPath("", ProjectFlag));
	NFile::cleanStaleTempFiles(GetPath("nc", UserFlag));

	QString dataDir = GetPath("", UserFlag);
	if (dataDir == "data")
		return;

	QDir dir(dataDir);
	if (dir.exists())
		return;

	// Create new directory
	if (!dir.mkpath(dataDir))
		return;

	// Files and directories to migrate
	QStringList itemsToMigrate = {
		"ncmachine.json",
		"settings.json",
		"work.db",
		"qcnc.dat",
		"nc",
		"default",
		"logs"
	};
	for (const QString& item : itemsToMigrate) {
		QString oldPath = "data/" + item;
		QString newPath = dataDir + "/" + item;
		if (!QFile::exists(oldPath))
			continue;
		QFileInfo info(oldPath);
		if (info.isDir()) {
			migrateDirectory(oldPath, newPath);
		} else if (!QFile::exists(newPath)) {
			migrateFile(oldPath, newPath);
		}
	}

	// Copy system config files to UserData if not exist
	QString systemDir = GetSystemDataDir();
	QStringList systemConfigFiles = {
		"xitongshezhi_data.json",
		"xitongshezhi2_data.json",
		"xitongshezhi3_data.json",
		"xitongshezhi4_data.json",
		"xitongshezhi5_data.json",
		"fangdiancanshu_global_data.json",
		"luojubuchang1_data.json",
		"luojubuchang2_data.json"
	};
	for (const QString& file : systemConfigFiles) {
		QString srcPath = systemDir + "/" + file;
		QString destPath = dataDir + "/" + file;
		if (QFile::exists(srcPath) && !QFile::exists(destPath)) {
			QFile::copy(srcPath, destPath);
		}
	}
}