/********************************************************************************
** Form generated from reading UI file 'CAMotics_gcode_editor.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CAMOTICS_GCODE_EDITOR_H
#define UI_CAMOTICS_GCODE_EDITOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CAMotics_gcode_editorClass
{
public:
    QAction *actionParse;
    QAction *actionSimu;
    QAction *actionLoad;
    QAction *actionSave;
    QAction *actionRun;
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;

    void setupUi(QMainWindow *CAMotics_gcode_editorClass)
    {
        if (CAMotics_gcode_editorClass->objectName().isEmpty())
            CAMotics_gcode_editorClass->setObjectName(QString::fromUtf8("CAMotics_gcode_editorClass"));
        CAMotics_gcode_editorClass->resize(600, 400);
        actionParse = new QAction(CAMotics_gcode_editorClass);
        actionParse->setObjectName(QString::fromUtf8("actionParse"));
        actionSimu = new QAction(CAMotics_gcode_editorClass);
        actionSimu->setObjectName(QString::fromUtf8("actionSimu"));
        actionLoad = new QAction(CAMotics_gcode_editorClass);
        actionLoad->setObjectName(QString::fromUtf8("actionLoad"));
        actionSave = new QAction(CAMotics_gcode_editorClass);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));
        actionRun = new QAction(CAMotics_gcode_editorClass);
        actionRun->setObjectName(QString::fromUtf8("actionRun"));
        centralWidget = new QWidget(CAMotics_gcode_editorClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        CAMotics_gcode_editorClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(CAMotics_gcode_editorClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 22));
        CAMotics_gcode_editorClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(CAMotics_gcode_editorClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        CAMotics_gcode_editorClass->addToolBar(Qt::TopToolBarArea, mainToolBar);

        mainToolBar->addAction(actionLoad);
        mainToolBar->addAction(actionSave);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionParse);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionRun);

        retranslateUi(CAMotics_gcode_editorClass);

        QMetaObject::connectSlotsByName(CAMotics_gcode_editorClass);
    } // setupUi

    void retranslateUi(QMainWindow *CAMotics_gcode_editorClass)
    {
        CAMotics_gcode_editorClass->setWindowTitle(QCoreApplication::translate("CAMotics_gcode_editorClass", "CAMotics_gcode_editor", nullptr));
        actionParse->setText(QCoreApplication::translate("CAMotics_gcode_editorClass", "Parse", nullptr));
        actionSimu->setText(QCoreApplication::translate("CAMotics_gcode_editorClass", "Simu", nullptr));
        actionLoad->setText(QCoreApplication::translate("CAMotics_gcode_editorClass", "Load", nullptr));
        actionSave->setText(QCoreApplication::translate("CAMotics_gcode_editorClass", "Save", nullptr));
        actionRun->setText(QCoreApplication::translate("CAMotics_gcode_editorClass", "Run", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CAMotics_gcode_editorClass: public Ui_CAMotics_gcode_editorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CAMOTICS_GCODE_EDITOR_H
