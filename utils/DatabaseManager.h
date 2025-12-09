#pragma once
#include <QSqlDatabase>
#include <QMutex>
#include <QMutexLocker>
#include <QVariantMap>

class DatabaseManager
{
public:
	// Static instance accessor
	static DatabaseManager& instance() {
		static DatabaseManager manager; // Static initialization, thread-safe in C++11 and later
		return manager;
	}

	~DatabaseManager();

	bool addDatabase(const QString& type, const QString& connectionName, const QVariantMap& connectionOptions);

	QSqlDatabase getDatabase(const QString& connectionName) const;

	bool executeQuery(const QString& connectionName, const QString& queryStr);

private:
	DatabaseManager() {} // Private constructor to prevent external instantiation
	DatabaseManager(const DatabaseManager&) = delete;
	DatabaseManager& operator=(const DatabaseManager&) = delete;

	QMap<QString, QSqlDatabase> databaseConnections;
	mutable QMutex mutex; // Mutex for thread-safe access to the map
};

