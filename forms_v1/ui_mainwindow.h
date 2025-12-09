/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QtWin
{
public:
    QAction *actionOptions;
    QAction *actionRegWindow;
    QWidget *centralwidget;
    QMenuBar *menubar;
    QMenu *menuView;
    QDockWidget *dockWidgetLog;
    QWidget *dockWidgetContents;
    QVBoxLayout *verticalLayout;
    QPlainTextEdit *txtLog;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 468);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        actionOptions = new QAction(MainWindow);
        actionOptions->setObjectName(QString::fromUtf8("actionOptions"));
        actionRegWindow = new QAction(MainWindow);
        actionRegWindow->setObjectName(QString::fromUtf8("actionRegWindow"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        sizePolicy.setHeightForWidth(centralwidget->sizePolicy().hasHeightForWidth());
        centralwidget->setSizePolicy(sizePolicy);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 22));
        menuView = new QMenu(menubar);
        menuView->setObjectName(QString::fromUtf8("menuView"));
        MainWindow->setMenuBar(menubar);
        dockWidgetLog = new QDockWidget(MainWindow);
        dockWidgetLog->setObjectName(QString::fromUtf8("dockWidgetLog"));
        dockWidgetLog->setFeatures(QDockWidget::NoDockWidgetFeatures);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        verticalLayout = new QVBoxLayout(dockWidgetContents);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        txtLog = new QPlainTextEdit(dockWidgetContents);
        txtLog->setObjectName(QString::fromUtf8("txtLog"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(1);
        sizePolicy1.setVerticalStretch(1);
        sizePolicy1.setHeightForWidth(txtLog->sizePolicy().hasHeightForWidth());
        txtLog->setSizePolicy(sizePolicy1);
        txtLog->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        txtLog->setSizeAdjustPolicy(QAbstractScrollArea::AdjustIgnored);
        txtLog->setLineWrapMode(QPlainTextEdit::NoWrap);

        verticalLayout->addWidget(txtLog);

        dockWidgetLog->setWidget(dockWidgetContents);
        MainWindow->addDockWidget(Qt::BottomDockWidgetArea, dockWidgetLog);

        menubar->addAction(menuView->menuAction());
        menuView->addAction(actionOptions);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("QtWin", "MainWindow2", nullptr));
        actionOptions->setText(QCoreApplication::translate("QtWin", "Options", nullptr));
        actionRegWindow->setText(QCoreApplication::translate("QtWin", "RegWindow", nullptr));
        menuView->setTitle(QCoreApplication::translate("QtWin", "View", nullptr));
        dockWidgetLog->setWindowTitle(QCoreApplication::translate("QtWin", "RZ", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QtWin: public Ui_QtWin {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
