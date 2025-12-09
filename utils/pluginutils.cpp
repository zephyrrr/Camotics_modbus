#include "pluginutils.h"
#include <QObject>
#include <QDir>
#include <QCoreApplication>
#include <QCryptographicHash>
#include <QPushButton>
#include "PythonQtRuntime.h"


SimpleCrypt PluginUtils::crypto(Q_UINT64_C(0x0c2ad3a3acb9f023));

QMap<QString, QString> PluginUtils::loadPythonScripts(const QString& pluginPath)
{
    if (!PythonQtRuntime::getInstance()->IsInit()) {
        return QMap<QString, QString>();
    }

    //PluginUtils::decryptFile(QStringLiteral("z:\\tools_Иёзг2.pyc.en"));

    QMap<QString, QString> ret;

    QString appPath = QCoreApplication::applicationDirPath();
    QDir pluginsDir(appPath);
    bool bDirExist = true;
#if defined(Q_OS_WIN)
    if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
        bDirExist = pluginsDir.cdUp();
    if (pluginsDir.dirName().toLower() == "x64" )
        bDirExist = pluginsDir.cdUp();
#elif defined(Q_OS_MAC)
    if (pluginsDir.dirName() == "MacOS") {
        bDirExist = pluginsDir.cdUp();
        bDirExist = pluginsDir.cdUp();
        bDirExist = pluginsDir.cdUp();
    }
#endif
    bDirExist = pluginsDir.cd(PLUGIN_PATH);
    bDirExist = pluginsDir.cd(pluginPath);
    QStringList filters;
    // QStringList() << "*.py" << "*.pyc" << "*.pycb"

    const bool usePyarmorPy = true;
    if (usePyarmorPy) {
        filters << "*.py" << "*.pyc";
    }
    else {
#ifdef _DEBUG
        filters << "*.py";
        for (const QString& fileName : pluginsDir.entryList(QStringList() << "*.pyc", QDir::Files, QDir::Name)) {
            QString enFileName = PluginUtils::encryptFile(pluginsDir.absoluteFilePath(fileName));
            QFile::rename(enFileName, pluginsDir.absoluteFilePath(fileName + "b"));
        }
#else:
        filters << "*.pycb";
        for (const QString& fileName : pluginsDir.entryList(QStringList() << "*.py", QDir::Files, QDir::Name)) {
            QFile::remove(pluginsDir.absoluteFilePath(fileName));
        }
#endif // _DEBUG
    }

    const QStringList entries = pluginsDir.entryList(filters, QDir::Files, QDir::Name);
    for (const QString& fileName : entries) {
        if (fileName.startsWith("P_"))
            continue;

        QFileInfo fileInfo(fileName);
        QString baseName = fileInfo.baseName();

        //// Generate the hash of the baseName
        //QByteArray hash = QCryptographicHash::hash(baseName.toUtf8(), QCryptographicHash::Md5);
        //QString hashedBaseName = hash.toHex();
        //QString hashedFileName = "P_" + hashedBaseName + "." + fileInfo.completeSuffix();
        //QFile::copy(pluginsDir.absoluteFilePath(fileName), pluginsDir.absoluteFilePath(hashedFileName));

        QString hashedFileName = fileName;
        // prefer .py file
        if (!ret.contains(baseName) || fileInfo.suffix() == "py") {
            ret[baseName] = pluginsDir.absoluteFilePath(hashedFileName);
        }
    }

    return ret;
}

//void PluginUtils::AddPluginsToLayout(const QString& pluginPath, QLayout* layout)
//{
//
//}
QList<QWidget*> PluginUtils::CreateScripts(const QString& pluginPath, QObject* parent)
{
    QList<QWidget*> ret;
#ifdef PYTHON_SUPPORT
    QMap<QString, QString> pyFiles = PluginUtils::loadPythonScripts(pluginPath);
    for (const auto& pyFileName : pyFiles.keys()) {
        QPushButton* btn = new QPushButton(pyFileName);
        ret.append(btn);

        QString value = pyFiles.value(pyFileName);
        QObject::connect(btn, &QPushButton::clicked, [value, parent]() {
            PluginUtils::RunFile(value, parent);
            });
    }
#endif
    return ret;
}

bool PluginUtils::verifyFile(const QString& filePath)
{
	QString newFilePath = filePath;
    if (filePath.endsWith(".py")) {
        newFilePath = newFilePath + "c";
        //return true;
    }

    QFile file(newFilePath);
    if (!file.open(QIODevice::ReadOnly)) {
#ifdef _DEBUG
        return true;
#else
        return false;
#endif // DEBUG
    }

    QCryptographicHash hash(QCryptographicHash::Sha256);
    hash.addData(file.readAll());
    file.close();

    QByteArray hash1 = hash.result();

    QFile file2(newFilePath + ".md5");

#ifdef _DEBUG
    QByteArray md5 = crypto.encryptToByteArray(hash1);
    if (file2.open(QIODevice::WriteOnly)) {
        file2.write(md5);
        file2.close();
    }
    return true;
#else
    if (!file2.exists()) {
        return false;
    }
#endif // DEBUG

    if (!file2.open(QIODevice::ReadOnly))
        return false;

    QByteArray hash2 = crypto.decryptToByteArray(file2.readAll());

	return hash1 == hash2;
}

QString PluginUtils::encryptFile(const QString& filePath)
{
    QFile file(filePath);

    QByteArray fileBytes;
    if (file.open(QIODevice::ReadOnly)) {
        fileBytes = file.readAll();
        file.close();
    }
    
    int a = fileBytes.length();

    QByteArray fileBytes2 = crypto.encryptToByteArray(fileBytes);
    int b = fileBytes2.length();
    QFile file2(filePath + ".en");
    if (file2.open(QIODevice::WriteOnly)) {
        file2.write(fileBytes2);
        file2.close();
    }

    return file2.fileName();
}

QString PluginUtils::decryptFile(const QString& filePath)
{
    QFile file(filePath);

    QByteArray fileBytes;
    if (file.open(QIODevice::ReadOnly)) {
        fileBytes = file.readAll();
        file.close();
    }

    QByteArray fileBytes2 = crypto.decryptToByteArray(fileBytes);

    QFile file2(filePath + ".de");
    if (file2.open(QIODevice::WriteOnly)) {
        file2.write(fileBytes2);
        file2.close();
    }

    return file2.fileName();
}

QVariant PluginUtils::RunFile(const QString& fileName, QObject* window)
{
    //bool b = PluginUtils::verifyFile(fileName);
    //if (!b) {
    //    return QVariant::Invalid;
    //}
    try {
        QString newFileName = fileName;
        if (fileName.endsWith(".pycb")) {
            newFileName = decryptFile(fileName);
        }

        QHash<QString, QObject*> params;
        params["window"] = window;
        QVariant ret = PythonQtRuntime::getInstance()->RunFile(newFileName, params);
        if (fileName.endsWith(".pycb")) {
            QFile::remove(newFileName);
        }
        return ret;
    }
    catch (...) {
        return QVariant::Invalid;
	}
}