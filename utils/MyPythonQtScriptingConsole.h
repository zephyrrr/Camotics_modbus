#pragma once

#include "../pythonqt/src/gui/PythonQtScriptingConsole.h"

class MyPythonQtScriptingConsole : public PythonQtScriptingConsole
{
    Q_OBJECT
public:
    MyPythonQtScriptingConsole(QWidget* parent, const PythonQtObjectPtr& context, Qt::WindowFlags i = Qt::WindowFlags());
    void keyPressEvent(QKeyEvent* event) override;

    bool CheckSyntax(QString code);

protected:
    void insertFromMimeData(const QMimeData* source) override;
};

