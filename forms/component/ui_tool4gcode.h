/********************************************************************************
** Form generated from reading UI file 'tool4gcode.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TOOL4GCODE_H
#define UI_TOOL4GCODE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Tool4GCodeClass
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *btnExport;

    void setupUi(QWidget *Tool4GCodeClass)
    {
        if (Tool4GCodeClass->objectName().isEmpty())
            Tool4GCodeClass->setObjectName(QString::fromUtf8("Tool4GCodeClass"));
        Tool4GCodeClass->resize(360, 180);
        Tool4GCodeClass->setStyleSheet(QString::fromUtf8(""));
        verticalLayout = new QVBoxLayout(Tool4GCodeClass);
        verticalLayout->setSpacing(5);
        verticalLayout->setContentsMargins(5, 5, 5, 5);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(5);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        btnExport = new QPushButton(Tool4GCodeClass);
        btnExport->setObjectName(QString::fromUtf8("btnExport"));

        horizontalLayout->addWidget(btnExport);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(Tool4GCodeClass);

        QMetaObject::connectSlotsByName(Tool4GCodeClass);
    } // setupUi

    void retranslateUi(QWidget *Tool4GCodeClass)
    {
        Tool4GCodeClass->setWindowTitle(QCoreApplication::translate("Tool4GCodeClass", "Tool4GCode", nullptr));
        btnExport->setText(QCoreApplication::translate("Tool4GCodeClass", "GDMBC", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Tool4GCodeClass: public Ui_Tool4GCodeClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TOOL4GCODE_H
