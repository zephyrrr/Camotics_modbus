#pragma once
#include <QObject>

//#ifdef _DEBUG
#define PYTHON_SUPPORT
//#endif // DEBUG

#ifdef PYTHON_SUPPORT
#include "../pythonqt/src/PythonQt.h"

class PythonQtScriptingConsole;

class PythonQtRuntime
{
private:
    static PythonQtRuntime instance;
    PythonQtRuntime();

public:
    static PythonQtRuntime* getInstance();
    ~PythonQtRuntime();

public:
    bool IsInit() { return m_isPythonInstalled; }

	bool Init();
	void ShowConsole(QObject* window);
    QVariant RunScript(const QString& script);
    QVariant RunFile(const QString& fileName, const QHash<QString, QObject*>& params = {});
	QVariant CallMethod(const QString& callable, const QVariantList& args = QVariantList(), const QVariantMap& kwargs = QVariantMap());
    //template<class T>
    //static void registerCPPClass(const char* typeName, const char* parentTypeName = nullptr)
    //{
    //    if (!getInstance()->m_isPythonInstalled)
    //        return;
    //    PythonQt::self()->registerCPPClass(typeName, parentTypeName, "", PythonQtCreateObject<T>, NULL);
    //}

private:
    PythonQtScriptingConsole* console;
    bool m_isPythonInstalled = false;
};

#else
class PythonQtRuntime
{
private:
    static PythonQtRuntime instance;
    PythonQtRuntime() {}

public:
    static PythonQtRuntime* getInstance() { return &instance; }
    ~PythonQtRuntime() {}

public:
    void Init() {}
    QVariant ShowConsole(QObject* window) {}
    QVariant RunScript(const QString& script, QObject* window) {}
    void RunFile(const QString& fileName, QObject* window) {}
    //template<class T>
    //static void registerCPPClass(const char* typeName, const char* parentTypeName = nullptr) {}
};
#endif // PYTHON_SUPPORT

