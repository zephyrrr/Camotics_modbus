// http://www.wisol.ch/w/articles/2015-07-26-virtual-keyboard-qt
// https://github.com/wisoltech/qt-virt-keyboard
// modified 2016 Dzoka

#include <QCoreApplication>
#include <QGuiApplication>
#include <QKeyEvent>
#include <QWindow>
#include "keyemitter.h"
#include <cbang/log/Logger.h>

KeyEmitter::KeyEmitter()
{
}

KeyEmitter::~KeyEmitter()
{

}

void KeyEmitter::emitKey(Qt::Key key, QObject* receiver)
{
    if (receiver == NULL) {
        //receiver = QGuiApplication::focusObject();
    }

    if(!receiver)
    {
        return;
    }
    QKeyEvent pressEvent = QKeyEvent(QEvent::KeyPress, key, Qt::NoModifier, QKeySequence(key).toString());
    QCoreApplication::sendEvent(receiver, &pressEvent);

    QKeyEvent releaseEvent = QKeyEvent(QEvent::KeyRelease, key, Qt::NoModifier);
    QCoreApplication::sendEvent(receiver, &releaseEvent);

    QString objectInfo = QString("Object '%1' (Class: %2)")
        .arg(receiver->objectName())
        .arg(receiver->metaObject()->className());

    QString s = QString("UI: Key Press %1 on %2")
        .arg(QKeySequence(key).toString())
        .arg(objectInfo);
    LOG_INFO(8, s.toUtf8().constData());
}
