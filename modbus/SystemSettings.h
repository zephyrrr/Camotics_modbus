#ifndef SYSTEMSETTINGS_H
#define SYSTEMSETTINGS_H

#include <cbang/util/Singleton.h>
#include <QObject>
#include <QHash>
#include <QDir>

using namespace cb;

class SystemSettings : public Singleton<SystemSettings>
{
public:
	enum DataDirFlag {
		ProjectFlag = 1,
		UserFlag = 2,
		SystemFlag = 4
	};
	using DataDirType = int;

	SystemSettings(Inaccessible) {};

	QString GetProjectName() { return m_projectName; }
	void SetProjectName(const QString& projectName);

	static QString GetPath(const QString& fileName, DataDirType dirFlags = ProjectFlag);
	static QString GetDataFilePath(const QString& objectName, DataDirType dirFlags = ProjectFlag);
	static QString AppendDataFilePath(const QString& filePath, const QString& append);

	void SetValue(const QString& key, QString value) { m_values[key] = value; }
	QString GetValue(const QString& key) { return m_values.contains(key) ? m_values[key] : ""; }
	void DeleteValue(const QString& key) { m_values.remove(key); }

	void LoadFromFile(const QString& filePath);
	void MigrateUserDataDir();

private:
	static QString CombinePath(const QString& base, const QString& relative) {
		return QDir::cleanPath(base + "/" + relative);
	}

	QString GetProjectDir() { return GetUserDataDir() + "/" + m_projectName; }
	QString GetUserDataDir() { QString v = GetValue("System/UserDataDir"); return v.isEmpty() ? "data" : v; }
	QString GetSystemDataDir() { return QDir::currentPath() + "/data"; }

public:
	QString LastRunNCFileName;

private:
	QString m_projectName;
	QHash<QString, QString> m_values;
};

#endif