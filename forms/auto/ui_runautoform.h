/********************************************************************************
** Form generated from reading UI file 'runautoform.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RUNAUTOFORM_H
#define UI_RUNAUTOFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RunAutoForm
{
public:
    QHBoxLayout *horizontalLayout_9;
    QTabWidget *tabWidget;

    void setupUi(QWidget *RunAutoForm)
    {
        if (RunAutoForm->objectName().isEmpty())
            RunAutoForm->setObjectName(QString::fromUtf8("RunAutoForm"));
        RunAutoForm->resize(900, 600);
        horizontalLayout_9 = new QHBoxLayout(RunAutoForm);
        horizontalLayout_9->setSpacing(0);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        horizontalLayout_9->setContentsMargins(0, 0, 0, 0);
        tabWidget = new QTabWidget(RunAutoForm);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setTabPosition(QTabWidget::TabPosition::South);

        horizontalLayout_9->addWidget(tabWidget);


        retranslateUi(RunAutoForm);

        tabWidget->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(RunAutoForm);
    } // setupUi

    void retranslateUi(QWidget *RunAutoForm)
    {
        RunAutoForm->setWindowTitle(QCoreApplication::translate("RunAutoForm", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RunAutoForm: public Ui_RunAutoForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RUNAUTOFORM_H
