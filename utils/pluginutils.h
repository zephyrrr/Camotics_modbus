#pragma once

#include <QList>
#include <QHash>
#include <QMap>
#include <QLayout>
#include "PythonQtRuntime.h"
#include "../utils/simplecrypt.h"

#ifdef _DEBUG
#define PLUGIN_PATH "plugins_src/"
#else
#define PLUGIN_PATH "plugins/"  //"dist/plugins/"
#endif // _DEBUG

// Python plugin info structure
struct PythonPluginInfo {
	QString name;           // Display name in menu
	QString filePath;       // Full path to .py file
	int pageIndex;          // Menu page index (0-4)
	QVariantMap metadata;   // Additional metadata from plugin
};

class PluginUtils
{
public:
	template <typename T>
	static QList<T*> loadPlugin(const QString &pluginPath) {
        QList<T*> ret;

        QDir pluginsDir(QCoreApplication::applicationDirPath());
#if defined(Q_OS_WIN)
        if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
            pluginsDir.cdUp();
#elif defined(Q_OS_MAC)
        if (pluginsDir.dirName() == "MacOS") {
            pluginsDir.cdUp();
            pluginsDir.cdUp();
            pluginsDir.cdUp();
        }
#endif
        pluginsDir.cd("plugins");
        pluginsDir.cd(pluginPath);
        const QStringList entries = pluginsDir.entryList(QStringList() << "*.dll", QDir::Files, QDir::Name);
        for (const QString& fileName : entries) {
            QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
            QObject* plugin = pluginLoader.instance();
            if (plugin) {
                T* echoInterface = qobject_cast<T*>(plugin);
                if (echoInterface)
                {
                    ret.append(echoInterface);
                }
                else
                {
                    pluginLoader.unload();
                }
            }
        }

        return ret;
	}

    static QMap<QString, QString> loadPythonScripts(const QString& pluginPath);

    // Load Python plugins with menu support
    // Returns list of plugin info with name, file path, and page index
    static QList<PythonPluginInfo> loadPythonMenuPlugins(const QString& pluginPath, int defaultPageIndex = 1);

    // Create a widget from Python plugin
    // Python plugin should define: create_widget(window) -> QWidget
    static QWidget* createPythonWidget(const QString& filePath, QObject* window);

    //static void AddPluginsToLayout(const QString& pluginPath, QLayout* layout);
    static QList<QWidget*> CreateScripts(const QString& pluginPath, QObject* parent);

    static bool verifyFile(const QString& filePath);
	static QString encryptFile(const QString& filePath);
	static QString decryptFile(const QString& filePath);
    static QVariant RunFile(const QString& fileName, QObject* window);
private:
    static SimpleCrypt crypto;
};


