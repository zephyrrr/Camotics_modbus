/********************************************************************************
** Form generated from reading UI file 'boollineedit.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BOOLLINEEDIT_H
#define UI_BOOLLINEEDIT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_BoolLineEditClass
{
public:
    QHBoxLayout *horizontalLayout;
    QPushButton *btnOn;
    QPushButton *btnOff;
    QButtonGroup *buttonGroup;

    void setupUi(QWidget *BoolLineEditClass)
    {
        if (BoolLineEditClass->objectName().isEmpty())
            BoolLineEditClass->setObjectName(QString::fromUtf8("BoolLineEditClass"));
        BoolLineEditClass->resize(173, 23);
        horizontalLayout = new QHBoxLayout(BoolLineEditClass);
        horizontalLayout->setSpacing(2);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        btnOn = new QPushButton(BoolLineEditClass);
        buttonGroup = new QButtonGroup(BoolLineEditClass);
        buttonGroup->setObjectName(QString::fromUtf8("buttonGroup"));
        buttonGroup->addButton(btnOn);
        btnOn->setObjectName(QString::fromUtf8("btnOn"));
        btnOn->setCheckable(true);

        horizontalLayout->addWidget(btnOn);

        btnOff = new QPushButton(BoolLineEditClass);
        buttonGroup->addButton(btnOff);
        btnOff->setObjectName(QString::fromUtf8("btnOff"));
        btnOff->setCheckable(true);

        horizontalLayout->addWidget(btnOff);


        retranslateUi(BoolLineEditClass);

        QMetaObject::connectSlotsByName(BoolLineEditClass);
    } // setupUi

    void retranslateUi(QWidget *BoolLineEditClass)
    {
        BoolLineEditClass->setWindowTitle(QCoreApplication::translate("BoolLineEditClass", "BoolLineEdit", nullptr));
        btnOn->setText(QCoreApplication::translate("BoolLineEditClass", "ON", nullptr));
        btnOff->setText(QCoreApplication::translate("BoolLineEditClass", "OFF", nullptr));
    } // retranslateUi

};

namespace Ui {
    class BoolLineEditClass: public Ui_BoolLineEditClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BOOLLINEEDIT_H
