/********************************************************************************
** Form generated from reading UI file 'about.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ABOUT_H
#define UI_ABOUT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "widgets/qlineedit4axisvalue.h"

QT_BEGIN_NAMESPACE

class Ui_aboutClass
{
public:
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QLabel *lblSoftVersion;
    QLabel *lblXjzrqTitle;
    QLabel *lblHardwareVersion4;
    QLabel *lblHardwareVersion2;
    QLabel *lblReg87Date;
    QLabel *label_3;
    QLabel *lblHardwareVersion1;
    QLabel *label;
    QLineEdit4Keyboard *txtReg87Date;
    QLabel *lblXlhTitle;
    QLabel *lblSqmmTitle;
    QLineEdit4Keyboard *txtDecryptPassword;
    QLabel *lblHardwareVersion3;
    QLabel *lblSqjzrqTitle;
    QLabel *label_2;
    QLabel *label_4;
    QLabel *label_1;
    QLabel *lblSerialNumber;
    QPushButton *btnDecrypt;

    void setupUi(QWidget *aboutClass)
    {
        if (aboutClass->objectName().isEmpty())
            aboutClass->setObjectName(QString::fromUtf8("aboutClass"));
        aboutClass->resize(583, 327);
        verticalLayout = new QVBoxLayout(aboutClass);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SizeConstraint::SetDefaultConstraint);
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        lblSoftVersion = new QLabel(aboutClass);
        lblSoftVersion->setObjectName(QString::fromUtf8("lblSoftVersion"));

        gridLayout->addWidget(lblSoftVersion, 0, 1, 1, 1, Qt::AlignmentFlag::AlignLeft);

        lblXjzrqTitle = new QLabel(aboutClass);
        lblXjzrqTitle->setObjectName(QString::fromUtf8("lblXjzrqTitle"));

        gridLayout->addWidget(lblXjzrqTitle, 2, 2, 1, 1);

        lblHardwareVersion4 = new QLabel(aboutClass);
        lblHardwareVersion4->setObjectName(QString::fromUtf8("lblHardwareVersion4"));

        gridLayout->addWidget(lblHardwareVersion4, 2, 1, 1, 1, Qt::AlignmentFlag::AlignLeft);

        lblHardwareVersion2 = new QLabel(aboutClass);
        lblHardwareVersion2->setObjectName(QString::fromUtf8("lblHardwareVersion2"));

        gridLayout->addWidget(lblHardwareVersion2, 3, 1, 1, 1, Qt::AlignmentFlag::AlignLeft);

        lblReg87Date = new QLabel(aboutClass);
        lblReg87Date->setObjectName(QString::fromUtf8("lblReg87Date"));

        gridLayout->addWidget(lblReg87Date, 1, 3, 1, 1, Qt::AlignmentFlag::AlignLeft);

        label_3 = new QLabel(aboutClass);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 3, 0, 1, 1);

        lblHardwareVersion1 = new QLabel(aboutClass);
        lblHardwareVersion1->setObjectName(QString::fromUtf8("lblHardwareVersion1"));

        gridLayout->addWidget(lblHardwareVersion1, 4, 1, 1, 1, Qt::AlignmentFlag::AlignLeft);

        label = new QLabel(aboutClass);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        txtReg87Date = new QLineEdit4Keyboard(aboutClass);
        txtReg87Date->setObjectName(QString::fromUtf8("txtReg87Date"));
        txtReg87Date->setFrame(true);

        gridLayout->addWidget(txtReg87Date, 2, 3, 1, 1, Qt::AlignmentFlag::AlignVCenter);

        lblXlhTitle = new QLabel(aboutClass);
        lblXlhTitle->setObjectName(QString::fromUtf8("lblXlhTitle"));

        gridLayout->addWidget(lblXlhTitle, 0, 2, 1, 1);

        lblSqmmTitle = new QLabel(aboutClass);
        lblSqmmTitle->setObjectName(QString::fromUtf8("lblSqmmTitle"));

        gridLayout->addWidget(lblSqmmTitle, 3, 2, 1, 1);

        txtDecryptPassword = new QLineEdit4Keyboard(aboutClass);
        txtDecryptPassword->setObjectName(QString::fromUtf8("txtDecryptPassword"));

        gridLayout->addWidget(txtDecryptPassword, 3, 3, 1, 1, Qt::AlignmentFlag::AlignVCenter);

        lblHardwareVersion3 = new QLabel(aboutClass);
        lblHardwareVersion3->setObjectName(QString::fromUtf8("lblHardwareVersion3"));

        gridLayout->addWidget(lblHardwareVersion3, 1, 1, 1, 1, Qt::AlignmentFlag::AlignLeft);

        lblSqjzrqTitle = new QLabel(aboutClass);
        lblSqjzrqTitle->setObjectName(QString::fromUtf8("lblSqjzrqTitle"));

        gridLayout->addWidget(lblSqjzrqTitle, 1, 2, 1, 1);

        label_2 = new QLabel(aboutClass);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 2, 0, 1, 1);

        label_4 = new QLabel(aboutClass);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 4, 0, 1, 1);

        label_1 = new QLabel(aboutClass);
        label_1->setObjectName(QString::fromUtf8("label_1"));

        gridLayout->addWidget(label_1, 1, 0, 1, 1);

        lblSerialNumber = new QLabel(aboutClass);
        lblSerialNumber->setObjectName(QString::fromUtf8("lblSerialNumber"));

        gridLayout->addWidget(lblSerialNumber, 0, 3, 1, 1, Qt::AlignmentFlag::AlignLeft);

        btnDecrypt = new QPushButton(aboutClass);
        btnDecrypt->setObjectName(QString::fromUtf8("btnDecrypt"));

        gridLayout->addWidget(btnDecrypt, 3, 4, 1, 1, Qt::AlignmentFlag::AlignVCenter);


        verticalLayout->addLayout(gridLayout);


        retranslateUi(aboutClass);

        QMetaObject::connectSlotsByName(aboutClass);
    } // setupUi

    void retranslateUi(QWidget *aboutClass)
    {
        aboutClass->setWindowTitle(QCoreApplication::translate("aboutClass", "about", nullptr));
        lblSoftVersion->setText(QString());
        lblXjzrqTitle->setText(QCoreApplication::translate("aboutClass", "XJZRQ", nullptr));
        lblHardwareVersion4->setText(QString());
        lblHardwareVersion2->setText(QString());
        lblReg87Date->setText(QString());
        label_3->setText(QCoreApplication::translate("aboutClass", "XSKZBB\357\274\232", nullptr));
        lblHardwareVersion1->setText(QString());
        label->setText(QCoreApplication::translate("aboutClass", "SWJBB\357\274\232", nullptr));
        txtReg87Date->setText(QString());
        lblXlhTitle->setText(QCoreApplication::translate("aboutClass", "XLH", nullptr));
        lblSqmmTitle->setText(QCoreApplication::translate("aboutClass", "SQMM", nullptr));
        txtDecryptPassword->setText(QString());
        lblHardwareVersion3->setText(QString());
        lblSqjzrqTitle->setText(QCoreApplication::translate("aboutClass", "SQJZRQ", nullptr));
        label_2->setText(QCoreApplication::translate("aboutClass", "XWJNHBB\357\274\232", nullptr));
        label_4->setText(QCoreApplication::translate("aboutClass", "SKHBB\357\274\232", nullptr));
        label_1->setText(QCoreApplication::translate("aboutClass", "XWJBB\357\274\232", nullptr));
        lblSerialNumber->setText(QString());
        btnDecrypt->setText(QCoreApplication::translate("aboutClass", "SQ", nullptr));
    } // retranslateUi

};

namespace Ui {
    class aboutClass: public Ui_aboutClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUT_H
