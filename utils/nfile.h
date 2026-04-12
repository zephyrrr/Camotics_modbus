#pragma once

#include <QObject>
#include <QSaveFile>

class NFile  : public QSaveFile
{
	Q_OBJECT

public:
	explicit NFile(const QString& name) : QSaveFile(name) {};
#ifndef QT_NO_QOBJECT
	explicit NFile(QObject* parent = nullptr) : QSaveFile(parent) {};
	explicit NFile(const QString& name, QObject* parent) : QSaveFile(name, parent) {};
#endif

	~NFile() {};

public:
	void close();

	// 新增：静态清理函数，一键清理残留临时文件
	static void cleanStaleTempFiles(const QString& dirPath);
};
