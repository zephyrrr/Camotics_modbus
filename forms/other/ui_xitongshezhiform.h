/********************************************************************************
** Form generated from reading UI file 'xitongshezhiform.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_XITONGSHEZHIFORM_H
#define UI_XITONGSHEZHIFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_xitongshezhiformClass
{
public:
    QWidget *centralWidget;

    void setupUi(QWidget *xitongshezhiformClass)
    {
        if (xitongshezhiformClass->objectName().isEmpty())
            xitongshezhiformClass->setObjectName(QString::fromUtf8("xitongshezhiformClass"));
        xitongshezhiformClass->resize(600, 400);
        centralWidget = new QWidget(xitongshezhiformClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));

        retranslateUi(xitongshezhiformClass);

        QMetaObject::connectSlotsByName(xitongshezhiformClass);
    } // setupUi

    void retranslateUi(QWidget *xitongshezhiformClass)
    {
        xitongshezhiformClass->setWindowTitle(QCoreApplication::translate("xitongshezhiformClass", "xitongshezhiform", nullptr));
    } // retranslateUi

};

namespace Ui {
    class xitongshezhiformClass: public Ui_xitongshezhiformClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_XITONGSHEZHIFORM_H
