/********************************************************************************
** Form generated from reading UI file 'tool4coorset.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TOOL4COORSET_H
#define UI_TOOL4COORSET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Tool4CoorSetClass
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *btnRecord;
    QPushButton *btnBatchSet;

    void setupUi(QWidget *Tool4CoorSetClass)
    {
        if (Tool4CoorSetClass->objectName().isEmpty())
            Tool4CoorSetClass->setObjectName(QString::fromUtf8("Tool4CoorSetClass"));
        Tool4CoorSetClass->resize(360, 180);
        Tool4CoorSetClass->setStyleSheet(QString::fromUtf8(""));
        verticalLayout = new QVBoxLayout(Tool4CoorSetClass);
        verticalLayout->setSpacing(5);
        verticalLayout->setContentsMargins(5, 5, 5, 5);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(5);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        btnRecord = new QPushButton(Tool4CoorSetClass);
        btnRecord->setObjectName(QString::fromUtf8("btnRecord"));

        horizontalLayout->addWidget(btnRecord);

        btnBatchSet = new QPushButton(Tool4CoorSetClass);
        btnBatchSet->setObjectName(QString::fromUtf8("btnBatchSet"));

        horizontalLayout->addWidget(btnBatchSet);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(Tool4CoorSetClass);

        QMetaObject::connectSlotsByName(Tool4CoorSetClass);
    } // setupUi

    void retranslateUi(QWidget *Tool4CoorSetClass)
    {
        Tool4CoorSetClass->setWindowTitle(QCoreApplication::translate("Tool4CoorSetClass", "Tool4CoorSet", nullptr));
        btnRecord->setText(QCoreApplication::translate("Tool4CoorSetClass", "JL", nullptr));
        btnBatchSet->setText(QCoreApplication::translate("Tool4CoorSetClass", "SZ", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Tool4CoorSetClass: public Ui_Tool4CoorSetClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TOOL4COORSET_H
