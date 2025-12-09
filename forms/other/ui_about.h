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
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_aboutClass
{
public:
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QLabel *label;
    QLabel *lblSoftVersion;
    QSpacerItem *horizontalSpacer;
    QLabel *label_1;
    QLabel *lblHardwareVersion3;
    QLabel *label_2;
    QLabel *lblHardwareVersion4;
    QLabel *label_3;
    QLabel *lblHardwareVersion2;
    QLabel *label_4;
    QLabel *lblHardwareVersion1;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *aboutClass)
    {
        if (aboutClass->objectName().isEmpty())
            aboutClass->setObjectName(QString::fromUtf8("aboutClass"));
        aboutClass->resize(549, 375);
        verticalLayout = new QVBoxLayout(aboutClass);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(aboutClass);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        lblSoftVersion = new QLabel(aboutClass);
        lblSoftVersion->setObjectName(QString::fromUtf8("lblSoftVersion"));

        gridLayout->addWidget(lblSoftVersion, 0, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 2, 1, 1);

        label_1 = new QLabel(aboutClass);
        label_1->setObjectName(QString::fromUtf8("label_1"));

        gridLayout->addWidget(label_1, 1, 0, 1, 1);

        lblHardwareVersion3 = new QLabel(aboutClass);
        lblHardwareVersion3->setObjectName(QString::fromUtf8("lblHardwareVersion3"));

        gridLayout->addWidget(lblHardwareVersion3, 1, 1, 1, 1);

        label_2 = new QLabel(aboutClass);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 2, 0, 1, 1);

        lblHardwareVersion4 = new QLabel(aboutClass);
        lblHardwareVersion4->setObjectName(QString::fromUtf8("lblHardwareVersion4"));

        gridLayout->addWidget(lblHardwareVersion4, 2, 1, 1, 1);

        label_3 = new QLabel(aboutClass);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 3, 0, 1, 1);

        lblHardwareVersion2 = new QLabel(aboutClass);
        lblHardwareVersion2->setObjectName(QString::fromUtf8("lblHardwareVersion2"));

        gridLayout->addWidget(lblHardwareVersion2, 3, 1, 1, 1);

        label_4 = new QLabel(aboutClass);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 4, 0, 1, 1);

        lblHardwareVersion1 = new QLabel(aboutClass);
        lblHardwareVersion1->setObjectName(QString::fromUtf8("lblHardwareVersion1"));

        gridLayout->addWidget(lblHardwareVersion1, 4, 1, 1, 1);


        verticalLayout->addLayout(gridLayout);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Minimum);

        verticalLayout->addItem(verticalSpacer);


        retranslateUi(aboutClass);

        QMetaObject::connectSlotsByName(aboutClass);
    } // setupUi

    void retranslateUi(QWidget *aboutClass)
    {
        aboutClass->setWindowTitle(QCoreApplication::translate("aboutClass", "about", nullptr));
        label->setText(QCoreApplication::translate("aboutClass", "SWJBB\357\274\232", nullptr));
        lblSoftVersion->setText(QString());
        label_1->setText(QCoreApplication::translate("aboutClass", "XWJBB\357\274\232", nullptr));
        lblHardwareVersion3->setText(QString());
        label_2->setText(QCoreApplication::translate("aboutClass", "XWJNHBB\357\274\232", nullptr));
        lblHardwareVersion4->setText(QString());
        label_3->setText(QCoreApplication::translate("aboutClass", "XSKZBB\357\274\232", nullptr));
        lblHardwareVersion2->setText(QString());
        label_4->setText(QCoreApplication::translate("aboutClass", "SKHBB\357\274\232", nullptr));
        lblHardwareVersion1->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class aboutClass: public Ui_aboutClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUT_H
