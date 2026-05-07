/********************************************************************************
** Form generated from reading UI file 'runautoparamform.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RUNAUTOPARAMFORM_H
#define UI_RUNAUTOPARAMFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "widgets/qlineedit4axisvalue.h"

QT_BEGIN_NAMESPACE

class Ui_RunAutoParamForm
{
public:
    QHBoxLayout *horizontalLayout_9;
    QVBoxLayout *verticalLayout_2;
    QSpacerItem *verticalSpacer1;
    QHBoxLayout *horizontalLayout_10;
    QGridLayout *gridLayout;
    QLabel *label_15;
    QLineEdit4AxisValue *inDbhhw;
    QHBoxLayout *horizontalLayout_7;
    NComboBox *inDjxz;
    QLabel *label_19;
    QLabel *label_13;
    QLabel *label_5;
    QSpacerItem *horizontalSpacer;
    QLabel *label_12;
    NComboBox *inCcd;
    NComboBox *inJgcz;
    QLabel *label_14;
    QLabel *label_1;
    QHBoxLayout *horizontalLayout_8;
    NComboBox *inJgz;
    QHBoxLayout *horizontalLayout;
    QLineEdit4AxisValue *inCksd;
    QLabel *label_18;
    NComboBox *inYdxz;
    QLabel *label_16;
    NComboBox *inJgmj;
    QLabel *label_11;
    NComboBox *inYdms;
    QHBoxLayout *horizontalLayout_4;
    NComboBox *inClzh;
    QHBoxLayout *horizontalLayout_3;
    QLineEdit4AxisValue *inJgsd;
    QLineEdit4AxisValue *inJgsd2;
    QLineEdit4AxisValue *inJgsd3;
    QLabel *label_17;
    QSpacerItem *horizontalSpacer_2;
    QLineEdit4Int *inYdxzSpecial;
    QSpacerItem *verticalSpacer2;
    QVBoxLayout *verticalLayout;
    QLabel *labelPix;
    QTextBrowser *textBrowser;

    void setupUi(QWidget *RunAutoParamForm)
    {
        if (RunAutoParamForm->objectName().isEmpty())
            RunAutoParamForm->setObjectName(QString::fromUtf8("RunAutoParamForm"));
        RunAutoParamForm->resize(900, 600);
        horizontalLayout_9 = new QHBoxLayout(RunAutoParamForm);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalSpacer1 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        verticalLayout_2->addItem(verticalSpacer1);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_15 = new QLabel(RunAutoParamForm);
        label_15->setObjectName(QString::fromUtf8("label_15"));

        gridLayout->addWidget(label_15, 6, 0, 1, 1);

        inDbhhw = new QLineEdit4AxisValue(RunAutoParamForm);
        inDbhhw->setObjectName(QString::fromUtf8("inDbhhw"));

        gridLayout->addWidget(inDbhhw, 10, 1, 1, 1);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        inDjxz = new NComboBox(RunAutoParamForm);
        inDjxz->addItem(QString());
        inDjxz->setObjectName(QString::fromUtf8("inDjxz"));

        horizontalLayout_7->addWidget(inDjxz);


        gridLayout->addLayout(horizontalLayout_7, 1, 1, 1, 1);

        label_19 = new QLabel(RunAutoParamForm);
        label_19->setObjectName(QString::fromUtf8("label_19"));

        gridLayout->addWidget(label_19, 10, 0, 1, 1);

        label_13 = new QLabel(RunAutoParamForm);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        gridLayout->addWidget(label_13, 4, 0, 1, 1);

        label_5 = new QLabel(RunAutoParamForm);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout->addWidget(label_5, 3, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 3, 1, 1);

        label_12 = new QLabel(RunAutoParamForm);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        gridLayout->addWidget(label_12, 2, 0, 1, 1);

        inCcd = new NComboBox(RunAutoParamForm);
        inCcd->setObjectName(QString::fromUtf8("inCcd"));

        gridLayout->addWidget(inCcd, 6, 1, 1, 1);

        inJgcz = new NComboBox(RunAutoParamForm);
        inJgcz->setObjectName(QString::fromUtf8("inJgcz"));

        gridLayout->addWidget(inJgcz, 9, 1, 1, 1);

        label_14 = new QLabel(RunAutoParamForm);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        gridLayout->addWidget(label_14, 5, 0, 1, 1);

        label_1 = new QLabel(RunAutoParamForm);
        label_1->setObjectName(QString::fromUtf8("label_1"));

        gridLayout->addWidget(label_1, 0, 0, 1, 1);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        inJgz = new NComboBox(RunAutoParamForm);
        inJgz->setObjectName(QString::fromUtf8("inJgz"));

        horizontalLayout_8->addWidget(inJgz);


        gridLayout->addLayout(horizontalLayout_8, 2, 1, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        inCksd = new QLineEdit4AxisValue(RunAutoParamForm);
        inCksd->setObjectName(QString::fromUtf8("inCksd"));

        horizontalLayout->addWidget(inCksd);


        gridLayout->addLayout(horizontalLayout, 5, 1, 1, 1);

        label_18 = new QLabel(RunAutoParamForm);
        label_18->setObjectName(QString::fromUtf8("label_18"));

        gridLayout->addWidget(label_18, 9, 0, 1, 1);

        inYdxz = new NComboBox(RunAutoParamForm);
        inYdxz->setObjectName(QString::fromUtf8("inYdxz"));

        gridLayout->addWidget(inYdxz, 7, 1, 1, 1);

        label_16 = new QLabel(RunAutoParamForm);
        label_16->setObjectName(QString::fromUtf8("label_16"));

        gridLayout->addWidget(label_16, 7, 0, 1, 1);

        inJgmj = new NComboBox(RunAutoParamForm);
        inJgmj->setObjectName(QString::fromUtf8("inJgmj"));

        gridLayout->addWidget(inJgmj, 4, 1, 1, 1);

        label_11 = new QLabel(RunAutoParamForm);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        gridLayout->addWidget(label_11, 1, 0, 1, 1);

        inYdms = new NComboBox(RunAutoParamForm);
        inYdms->setObjectName(QString::fromUtf8("inYdms"));

        gridLayout->addWidget(inYdms, 8, 1, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        inClzh = new NComboBox(RunAutoParamForm);
        inClzh->addItem(QString());
        inClzh->setObjectName(QString::fromUtf8("inClzh"));

        horizontalLayout_4->addWidget(inClzh);


        gridLayout->addLayout(horizontalLayout_4, 0, 1, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        inJgsd = new QLineEdit4AxisValue(RunAutoParamForm);
        inJgsd->setObjectName(QString::fromUtf8("inJgsd"));

        horizontalLayout_3->addWidget(inJgsd);

        inJgsd2 = new QLineEdit4AxisValue(RunAutoParamForm);
        inJgsd2->setObjectName(QString::fromUtf8("inJgsd2"));

        horizontalLayout_3->addWidget(inJgsd2);

        inJgsd3 = new QLineEdit4AxisValue(RunAutoParamForm);
        inJgsd3->setObjectName(QString::fromUtf8("inJgsd3"));

        horizontalLayout_3->addWidget(inJgsd3);


        gridLayout->addLayout(horizontalLayout_3, 3, 1, 1, 1);

        label_17 = new QLabel(RunAutoParamForm);
        label_17->setObjectName(QString::fromUtf8("label_17"));

        gridLayout->addWidget(label_17, 8, 0, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 0, 2, 1, 1);

        inYdxzSpecial = new QLineEdit4Int(RunAutoParamForm);
        inYdxzSpecial->setObjectName(QString::fromUtf8("inYdxzSpecial"));

        gridLayout->addWidget(inYdxzSpecial, 7, 2, 1, 1);


        horizontalLayout_10->addLayout(gridLayout);


        verticalLayout_2->addLayout(horizontalLayout_10);

        verticalSpacer2 = new QSpacerItem(20, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);

        verticalLayout_2->addItem(verticalSpacer2);


        horizontalLayout_9->addLayout(verticalLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        labelPix = new QLabel(RunAutoParamForm);
        labelPix->setObjectName(QString::fromUtf8("labelPix"));

        verticalLayout->addWidget(labelPix);

        textBrowser = new QTextBrowser(RunAutoParamForm);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));

        verticalLayout->addWidget(textBrowser);

        verticalLayout->setStretch(0, 2);
        verticalLayout->setStretch(1, 3);

        horizontalLayout_9->addLayout(verticalLayout);

        horizontalLayout_9->setStretch(0, 3);
        horizontalLayout_9->setStretch(1, 1);

        retranslateUi(RunAutoParamForm);

        QMetaObject::connectSlotsByName(RunAutoParamForm);
    } // setupUi

    void retranslateUi(QWidget *RunAutoParamForm)
    {
        RunAutoParamForm->setWindowTitle(QCoreApplication::translate("RunAutoParamForm", "Form", nullptr));
        label_15->setText(QCoreApplication::translate("RunAutoParamForm", "CCD", nullptr));
        inDbhhw->setText(QCoreApplication::translate("RunAutoParamForm", "0.350", nullptr));
        inDjxz->setItemText(0, QCoreApplication::translate("RunAutoParamForm", "TY", nullptr));

        label_19->setText(QCoreApplication::translate("RunAutoParamForm", "DBHHW", nullptr));
        label_13->setText(QCoreApplication::translate("RunAutoParamForm", "JGMJ", nullptr));
        label_5->setText(QCoreApplication::translate("RunAutoParamForm", "JGSD", nullptr));
        label_12->setText(QCoreApplication::translate("RunAutoParamForm", "JGZ", nullptr));
        label_14->setText(QCoreApplication::translate("RunAutoParamForm", "CKSD", nullptr));
        label_1->setText(QCoreApplication::translate("RunAutoParamForm", "CLZH", nullptr));
        inCksd->setText(QCoreApplication::translate("RunAutoParamForm", "0.000", nullptr));
        label_18->setText(QCoreApplication::translate("RunAutoParamForm", "JGZS", nullptr));
        label_16->setText(QCoreApplication::translate("RunAutoParamForm", "YDXZ", nullptr));
        label_11->setText(QCoreApplication::translate("RunAutoParamForm", "XZXZ", nullptr));
        inClzh->setItemText(0, QCoreApplication::translate("RunAutoParamForm", "SM G", nullptr));

        inJgsd->setText(QCoreApplication::translate("RunAutoParamForm", "0.000", nullptr));
        inJgsd2->setText(QCoreApplication::translate("RunAutoParamForm", "0.000", nullptr));
        inJgsd3->setText(QCoreApplication::translate("RunAutoParamForm", "0.000", nullptr));
        label_17->setText(QCoreApplication::translate("RunAutoParamForm", "YDMS", nullptr));
        labelPix->setText(QString());
        textBrowser->setHtml(QString());
    } // retranslateUi

};

namespace Ui {
    class RunAutoParamForm: public Ui_RunAutoParamForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RUNAUTOPARAMFORM_H
