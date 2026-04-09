#include "pluginutils.h"
#include <QObject>
#include <QDir>
#include <QCoreApplication>
#include <QCryptographicHash>
#include <QPushButton>
#include <QDebug>
#include "PythonQtRuntime.h"

#ifdef PYTHON_SUPPORT
#include "../pythonqt/src/PythonQt.h"
#endif


SimpleCrypt PluginUtils::crypto(Q_UINT64_C(0x0c2ad3a3acb9f023));

QMap<QString, QString> PluginUtils::loadPythonScripts(const QString& pluginPath)
{
    if (!PythonQtRuntime::getInstance()->IsInit()) {
        return QMap<QString, QString>();
    }

    //PluginUtils::decryptFile(tr("ZGZ2"));

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

QList<PythonPluginInfo> PluginUtils::loadPythonMenuPlugins(const QString& pluginPath, int defaultPageIndex)
{
    QList<PythonPluginInfo> result;

#ifdef PYTHON_SUPPORT
    if (!PythonQtRuntime::getInstance()->IsInit()) {
        return result;
    }

    // Get plugin directory
    QString appPath = QCoreApplication::applicationDirPath();
    QDir pluginsDir(appPath);
#if defined(Q_OS_WIN)
    if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
        pluginsDir.cdUp();
    if (pluginsDir.dirName().toLower() == "x64")
        pluginsDir.cdUp();
#elif defined(Q_OS_MAC)
    if (pluginsDir.dirName() == "MacOS") {
        pluginsDir.cdUp();
        pluginsDir.cdUp();
        pluginsDir.cdUp();
    }
#endif
    pluginsDir.cd(PLUGIN_PATH);
    pluginsDir.cd(pluginPath);

    // List Python files
    QStringList filters;
    filters << "*.py";
    const QStringList entries = pluginsDir.entryList(filters, QDir::Files, QDir::Name);

    for (const QString& fileName : entries) {
        // Skip init files and private files
        if (fileName == "init.py" || fileName.startsWith("_") || fileName.startsWith("P_")) {
            continue;
        }

        QString filePath = pluginsDir.absoluteFilePath(fileName);
        QFileInfo fileInfo(fileName);
        QString baseName = fileInfo.baseName();

        // Read plugin metadata by executing and getting variables
        PythonPluginInfo info;
        info.filePath = filePath;
        info.pageIndex = defaultPageIndex;

        try {
            PythonQtObjectPtr mainContext = PythonQt::self()->getMainModule();

            // Execute the plugin file to get metadata
            QString script = QString(
                "import sys\n"
                "import importlib.util\n"
                "_temp_plugin_path = r'%1'\n"
                "_temp_spec = importlib.util.spec_from_file_location('_temp_plugin', _temp_plugin_path)\n"
                "_temp_mod = importlib.util.module_from_spec(_temp_spec)\n"
                "_temp_spec.loader.exec_module(_temp_mod)\n"
                "_temp_name = getattr(_temp_mod, 'PLUGIN_NAME', '%2')\n"
                "_temp_page = getattr(_temp_mod, 'PLUGIN_PAGE', %3)\n"
                "_temp_result = {'name': _temp_name, 'page': _temp_page}\n"
            ).arg(QString(filePath).replace("\\", "/"), baseName, QString::number(defaultPageIndex));

            mainContext.evalScript(script);
            QVariantMap pluginData = mainContext.getVariable("_temp_result").toMap();

            info.name = pluginData.value("name", baseName).toString();
            info.pageIndex = pluginData.value("page", defaultPageIndex).toInt();

            // Validate page index
            if (info.pageIndex < 0 || info.pageIndex > 4) {
                info.pageIndex = defaultPageIndex;
            }

            result.append(info);
        }
        catch (...) {
            // On error, use defaults
            info.name = baseName;
            info.pageIndex = defaultPageIndex;
            result.append(info);
        }
    }
#endif

    return result;
}

QWidget* PluginUtils::createPythonWidget(const QString& filePath, QObject* window)
{
#ifdef PYTHON_SUPPORT
    if (!PythonQtRuntime::getInstance()->IsInit()) {
        qDebug("PluginUtils::createPythonWidget: Python not initialized");
        return nullptr;
    }

    try {
        PythonQtObjectPtr mainContext = PythonQt::self()->getMainModule();

        // Add window to context
        mainContext.addObject("_create_window", window);

        // Execute the plugin and call create_widget
        QString script = QString(
            "import sys\n"
            "import importlib.util\n"
            "_plugin_path = r'%1'\n"
            "_plugin_spec = importlib.util.spec_from_file_location('_create_plugin', _plugin_path)\n"
            "_plugin_mod = importlib.util.module_from_spec(_plugin_spec)\n"
            "_plugin_spec.loader.exec_module(_plugin_mod)\n"
            "_plugin_widget = None\n"
            "_plugin_error = None\n"
            "try:\n"
            "    if hasattr(_plugin_mod, 'create_widget'):\n"
            "        _plugin_widget = _plugin_mod.create_widget(_create_window)\n"
            "    else:\n"
            "        _plugin_error = 'create_widget function not found'\n"
            "except Exception as e:\n"
            "    _plugin_error = str(e)\n"
            "    print(f'[Python Error] {e}')\n"
        ).arg(QString(filePath).replace("\\", "/"));

        mainContext.evalScript(script);

        // Check for errors
        QVariant errorVar = mainContext.getVariable("_plugin_error");
        if (errorVar.isValid() && !errorVar.toString().isEmpty()) {
            qDebug("PluginUtils::createPythonWidget: Python error: %s", qPrintable(errorVar.toString()));
            return nullptr;
        }

        // Get the widget
        QVariant widgetVar = mainContext.getVariable("_plugin_widget");
        if (!widgetVar.isValid()) {
            qDebug("PluginUtils::createPythonWidget: _plugin_widget is not valid");
            return nullptr;
        }

        // PythonQt returns QWidget* as QObject*
        QObject* obj = widgetVar.value<QObject*>();
        if (!obj) {
            qDebug("PluginUtils::createPythonWidget: Failed to get QObject from variant, type=%d", widgetVar.type());
            return nullptr;
        }

        QWidget* widget = qobject_cast<QWidget*>(obj);
        if (!widget) {
            qDebug("PluginUtils::createPythonWidget: Object is not a QWidget: %s", obj->metaObject()->className());
            return nullptr;
        }

        qDebug("PluginUtils::createPythonWidget: Successfully created widget: %s", widget->objectName().toUtf8().constData());
        return widget;
    }
    catch (const std::exception& e) {
        qDebug("PluginUtils::createPythonWidget: Exception: %s", e.what());
        return nullptr;
    }
    catch (...) {
        qDebug("PluginUtils::createPythonWidget: Unknown exception");
        return nullptr;
    }
#else
    return nullptr;
#endif
}