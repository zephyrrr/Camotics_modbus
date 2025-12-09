#pragma once

#include <QObject>
#include <QEvent>
#include <QDebug>
#include <QMouseEvent>
#include <QWidget>
#include <cbang/log/Logger.h>
#include "../BaseChildWindow.h"

class EventLogger : public QObject
{
    Q_OBJECT
public:
    explicit EventLogger(QObject* parent = nullptr) : QObject(parent) {}

protected:
    bool eventFilter(QObject* watched, QEvent* event) override {
        if (event->type() == QEvent::MouseButtonPress) {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            
            QString objectInfo = QString("Object '%1' (Class: %2)")
                .arg(watched->objectName())
                .arg(watched->metaObject()->className());

            QWidget* watchedWidget = qobject_cast<QWidget*>(watched);
            QString windowInfo = "Main Window";
            if (watchedWidget) {
                
                BaseChildWindow* parentWindow = BaseChildWindow::GetChildWindow(watchedWidget);
                //QWidget* parentWindow = watchedWidget;
                //while (parentWindow && !parentWindow->isWindow())
                //{
                //    parentWindow = parentWindow->parentWidget();
                //}
                if (parentWindow) {
                    windowInfo = QString("Window '%1' (Class: %2)")
                        .arg(parentWindow->objectName())
                        .arg(parentWindow->metaObject()->className());
                }
            }

            QString s = QString("UI: Mouse Press on %1 at pos(%2, %3), in %4")
                .arg(objectInfo)
                .arg(mouseEvent->pos().x())
                .arg(mouseEvent->pos().y())
                .arg(windowInfo);

            LOG_INFO(8, s.toUtf8().constData());
        }

        // Pass the event on to the real target
        return QObject::eventFilter(watched, event);
    }
};

