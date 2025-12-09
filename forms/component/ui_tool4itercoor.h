/********************************************************************************
** Form generated from reading UI file 'tool4itercoor.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TOOL4ITERCOOR_H
#define UI_TOOL4ITERCOOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Tool4IterCoorClass
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *btnStartCoor;
    QPushButton *btnCurrentCoor;
    QPushButton *btnRunIterCoor;

    void setupUi(QWidget *Tool4IterCoorClass)
    {
        if (Tool4IterCoorClass->objectName().isEmpty())
            Tool4IterCoorClass->setObjectName(QString::fromUtf8("Tool4IterCoorClass"));
        Tool4IterCoorClass->resize(400, 200);
        Tool4IterCoorClass->setStyleSheet(QString::fromUtf8(""));
        verticalLayout = new QVBoxLayout(Tool4IterCoorClass);
        verticalLayout->setSpacing(5);
        verticalLayout->setContentsMargins(5, 5, 5, 5);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(5);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        btnStartCoor = new QPushButton(Tool4IterCoorClass);
        btnStartCoor->setObjectName(QString::fromUtf8("btnStartCoor"));
        btnStartCoor->setEnabled(false);

        horizontalLayout->addWidget(btnStartCoor);

        btnCurrentCoor = new QPushButton(Tool4IterCoorClass);
        btnCurrentCoor->setObjectName(QString::fromUtf8("btnCurrentCoor"));
        btnCurrentCoor->setEnabled(false);

        horizontalLayout->addWidget(btnCurrentCoor);

        btnRunIterCoor = new QPushButton(Tool4IterCoorClass);
        btnRunIterCoor->setObjectName(QString::fromUtf8("btnRunIterCoor"));

        horizontalLayout->addWidget(btnRunIterCoor);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(Tool4IterCoorClass);

        QMetaObject::connectSlotsByName(Tool4IterCoorClass);
    } // setupUi

    void retranslateUi(QWidget *Tool4IterCoorClass)
    {
        Tool4IterCoorClass->setWindowTitle(QCoreApplication::translate("Tool4IterCoorClass", "Tool4IterCoor", nullptr));
        btnStartCoor->setText(QCoreApplication::translate("Tool4IterCoorClass", "G054", nullptr));
        btnCurrentCoor->setText(QCoreApplication::translate("Tool4IterCoorClass", "G54", nullptr));
        btnRunIterCoor->setText(QCoreApplication::translate("Tool4IterCoorClass", "\346\211\271\351\207\217\350\277\220\350\241\214", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Tool4IterCoorClass: public Ui_Tool4IterCoorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TOOL4ITERCOOR_H
