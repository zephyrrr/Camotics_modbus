#ifndef BASEMAINWINDOW_H
#define BASEMAINWINDOW_H

#include <QMainWindow>
#include "BaseChildWindow.h"

class BaseMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    BaseMainWindow(QWidget* parent);
    ~BaseMainWindow();

public:

    virtual BaseChildWindow* getChildWindow(QString windowTitle) { Q_UNUSED(windowTitle);  return NULL; }
    virtual void showChildWindow(QString windowTitle);
protected:
};

#endif

