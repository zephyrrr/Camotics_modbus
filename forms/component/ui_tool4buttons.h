/********************************************************************************
** Form generated from reading UI file 'tool4buttons.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TOOL4BUTTONS_H
#define UI_TOOL4BUTTONS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Tool4ButtonsClass
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;

    void setupUi(QWidget *Tool4ButtonsClass)
    {
        if (Tool4ButtonsClass->objectName().isEmpty())
            Tool4ButtonsClass->setObjectName(QString::fromUtf8("Tool4ButtonsClass"));
        Tool4ButtonsClass->resize(400, 300);
        Tool4ButtonsClass->setStyleSheet(QString::fromUtf8(""));
        verticalLayout = new QVBoxLayout(Tool4ButtonsClass);
        verticalLayout->setSpacing(5);
        verticalLayout->setContentsMargins(5, 5, 5, 5);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(5);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));

        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(Tool4ButtonsClass);

        QMetaObject::connectSlotsByName(Tool4ButtonsClass);
    } // setupUi

    void retranslateUi(QWidget *Tool4ButtonsClass)
    {
        Tool4ButtonsClass->setWindowTitle(QCoreApplication::translate("Tool4ButtonsClass", "Tool4Buttons", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Tool4ButtonsClass: public Ui_Tool4ButtonsClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TOOL4BUTTONS_H
