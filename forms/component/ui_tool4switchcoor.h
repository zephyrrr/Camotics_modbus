/********************************************************************************
** Form generated from reading UI file 'tool4switchcoor.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TOOL4SWITCHCOOR_H
#define UI_TOOL4SWITCHCOOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Tool4SwitchCoorClass
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *btnSwitchCoorLeft;
    QPushButton *btnCurrentCoor;
    QPushButton *btnSwitchCoorRight;

    void setupUi(QWidget *Tool4SwitchCoorClass)
    {
        if (Tool4SwitchCoorClass->objectName().isEmpty())
            Tool4SwitchCoorClass->setObjectName(QString::fromUtf8("Tool4SwitchCoorClass"));
        Tool4SwitchCoorClass->resize(400, 200);
        Tool4SwitchCoorClass->setStyleSheet(QString::fromUtf8(""));
        verticalLayout = new QVBoxLayout(Tool4SwitchCoorClass);
        verticalLayout->setSpacing(5);
        verticalLayout->setContentsMargins(5, 5, 5, 5);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(5);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        btnSwitchCoorLeft = new QPushButton(Tool4SwitchCoorClass);
        btnSwitchCoorLeft->setObjectName(QString::fromUtf8("btnSwitchCoorLeft"));

        horizontalLayout->addWidget(btnSwitchCoorLeft);

        btnCurrentCoor = new QPushButton(Tool4SwitchCoorClass);
        btnCurrentCoor->setObjectName(QString::fromUtf8("btnCurrentCoor"));
        btnCurrentCoor->setEnabled(false);

        horizontalLayout->addWidget(btnCurrentCoor);

        btnSwitchCoorRight = new QPushButton(Tool4SwitchCoorClass);
        btnSwitchCoorRight->setObjectName(QString::fromUtf8("btnSwitchCoorRight"));

        horizontalLayout->addWidget(btnSwitchCoorRight);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(Tool4SwitchCoorClass);

        QMetaObject::connectSlotsByName(Tool4SwitchCoorClass);
    } // setupUi

    void retranslateUi(QWidget *Tool4SwitchCoorClass)
    {
        Tool4SwitchCoorClass->setWindowTitle(QCoreApplication::translate("Tool4SwitchCoorClass", "Tool4SwitchCoor", nullptr));
        btnSwitchCoorLeft->setText(QCoreApplication::translate("Tool4SwitchCoorClass", "<", nullptr));
        btnCurrentCoor->setText(QCoreApplication::translate("Tool4SwitchCoorClass", "G54", nullptr));
        btnSwitchCoorRight->setText(QCoreApplication::translate("Tool4SwitchCoorClass", ">", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Tool4SwitchCoorClass: public Ui_Tool4SwitchCoorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TOOL4SWITCHCOOR_H
