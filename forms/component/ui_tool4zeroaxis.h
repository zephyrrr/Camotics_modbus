/********************************************************************************
** Form generated from reading UI file 'tool4zeroaxis.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TOOL4ZEROAXIS_H
#define UI_TOOL4ZEROAXIS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Tool4ZeroAxisClass
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *btnSetXZero;
    QPushButton *btnSetYZero;
    QPushButton *btnSetZZero;

    void setupUi(QWidget *Tool4ZeroAxisClass)
    {
        if (Tool4ZeroAxisClass->objectName().isEmpty())
            Tool4ZeroAxisClass->setObjectName(QString::fromUtf8("Tool4ZeroAxisClass"));
        Tool4ZeroAxisClass->resize(400, 300);
        Tool4ZeroAxisClass->setStyleSheet(QString::fromUtf8(""));
        verticalLayout = new QVBoxLayout(Tool4ZeroAxisClass);
        verticalLayout->setSpacing(5);
        verticalLayout->setContentsMargins(5, 5, 5, 5);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(5);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        btnSetXZero = new QPushButton(Tool4ZeroAxisClass);
        btnSetXZero->setObjectName(QString::fromUtf8("btnSetXZero"));

        horizontalLayout->addWidget(btnSetXZero);

        btnSetYZero = new QPushButton(Tool4ZeroAxisClass);
        btnSetYZero->setObjectName(QString::fromUtf8("btnSetYZero"));

        horizontalLayout->addWidget(btnSetYZero);

        btnSetZZero = new QPushButton(Tool4ZeroAxisClass);
        btnSetZZero->setObjectName(QString::fromUtf8("btnSetZZero"));

        horizontalLayout->addWidget(btnSetZZero);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(Tool4ZeroAxisClass);

        QMetaObject::connectSlotsByName(Tool4ZeroAxisClass);
    } // setupUi

    void retranslateUi(QWidget *Tool4ZeroAxisClass)
    {
        Tool4ZeroAxisClass->setWindowTitle(QCoreApplication::translate("Tool4ZeroAxisClass", "Tool4ZeroAxis", nullptr));
        btnSetXZero->setText(QCoreApplication::translate("Tool4ZeroAxisClass", "X = 0", nullptr));
        btnSetYZero->setText(QCoreApplication::translate("Tool4ZeroAxisClass", "Y = 0", nullptr));
        btnSetZZero->setText(QCoreApplication::translate("Tool4ZeroAxisClass", "Z = 0", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Tool4ZeroAxisClass: public Ui_Tool4ZeroAxisClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TOOL4ZEROAXIS_H
