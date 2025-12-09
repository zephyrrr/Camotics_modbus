/********************************************************************************
** Form generated from reading UI file 'duanmianform.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DUANMIANFORM_H
#define UI_DUANMIANFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "widgets/qlineedit4axisvalue.h"

QT_BEGIN_NAMESPACE

class Ui_DuanMianForm
{
public:
    QHBoxLayout *horizontalLayout_9;
    QVBoxLayout *verticalLayout_2;
    QSpacerItem *verticalSpacer1;
    QHBoxLayout *horizontalLayout_10;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *btnX;
    QPushButton *btnX_P;
    QPushButton *btnX_N;
    QLabel *label_3;
    QHBoxLayout *horizontalLayout_8;
    QPushButton *btnZ;
    QPushButton *btnZ_P;
    QPushButton *btnZ_N;
    QLabel *label_1;
    QHBoxLayout *horizontalLayout;
    QPushButton *inSetToZero;
    QPushButton *pushButton_6;
    QHBoxLayout *horizontalLayout_7;
    QPushButton *btnY;
    QPushButton *btnY_P;
    QPushButton *btnY_N;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *btnU;
    QPushButton *btnU_P;
    QPushButton *btnU_N;
    QLabel *label_4;
    QLineEdit4AxisValue *inBack;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *verticalSpacer2;
    QVBoxLayout *verticalLayout;
    QLabel *labelPix;
    QTextBrowser *textBrowser;
    QButtonGroup *buttonGroup;

    void setupUi(QWidget *DuanMianForm)
    {
        if (DuanMianForm->objectName().isEmpty())
            DuanMianForm->setObjectName(QString::fromUtf8("DuanMianForm"));
        DuanMianForm->resize(900, 600);
        horizontalLayout_9 = new QHBoxLayout(DuanMianForm);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalSpacer1 = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer1);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        btnX = new QPushButton(DuanMianForm);
        btnX->setObjectName(QString::fromUtf8("btnX"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(btnX->sizePolicy().hasHeightForWidth());
        btnX->setSizePolicy(sizePolicy);
        QFont font;
        font.setPointSize(20);
        btnX->setFont(font);
        btnX->setCheckable(true);

        horizontalLayout_4->addWidget(btnX);

        btnX_P = new QPushButton(DuanMianForm);
        btnX_P->setObjectName(QString::fromUtf8("btnX_P"));
        btnX_P->setEnabled(false);
        sizePolicy.setHeightForWidth(btnX_P->sizePolicy().hasHeightForWidth());
        btnX_P->setSizePolicy(sizePolicy);
        QFont font1;
        font1.setPointSize(36);
        btnX_P->setFont(font1);
        btnX_P->setCheckable(true);

        horizontalLayout_4->addWidget(btnX_P);

        btnX_N = new QPushButton(DuanMianForm);
        btnX_N->setObjectName(QString::fromUtf8("btnX_N"));
        btnX_N->setEnabled(false);
        sizePolicy.setHeightForWidth(btnX_N->sizePolicy().hasHeightForWidth());
        btnX_N->setSizePolicy(sizePolicy);
        btnX_N->setFont(font1);
        btnX_N->setCheckable(true);

        horizontalLayout_4->addWidget(btnX_N);

        horizontalLayout_4->setStretch(0, 1);
        horizontalLayout_4->setStretch(1, 6);
        horizontalLayout_4->setStretch(2, 6);

        gridLayout->addLayout(horizontalLayout_4, 0, 1, 1, 1);

        label_3 = new QLabel(DuanMianForm);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 5, 0, 1, 1);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        btnZ = new QPushButton(DuanMianForm);
        btnZ->setObjectName(QString::fromUtf8("btnZ"));
        sizePolicy.setHeightForWidth(btnZ->sizePolicy().hasHeightForWidth());
        btnZ->setSizePolicy(sizePolicy);
        btnZ->setFont(font);
        btnZ->setCheckable(true);

        horizontalLayout_8->addWidget(btnZ);

        btnZ_P = new QPushButton(DuanMianForm);
        btnZ_P->setObjectName(QString::fromUtf8("btnZ_P"));
        btnZ_P->setEnabled(false);
        sizePolicy.setHeightForWidth(btnZ_P->sizePolicy().hasHeightForWidth());
        btnZ_P->setSizePolicy(sizePolicy);
        btnZ_P->setFont(font1);
        btnZ_P->setCheckable(true);

        horizontalLayout_8->addWidget(btnZ_P);

        btnZ_N = new QPushButton(DuanMianForm);
        btnZ_N->setObjectName(QString::fromUtf8("btnZ_N"));
        btnZ_N->setEnabled(false);
        sizePolicy.setHeightForWidth(btnZ_N->sizePolicy().hasHeightForWidth());
        btnZ_N->setSizePolicy(sizePolicy);
        btnZ_N->setFont(font1);
        btnZ_N->setCheckable(true);

        horizontalLayout_8->addWidget(btnZ_N);

        horizontalLayout_8->setStretch(0, 1);
        horizontalLayout_8->setStretch(1, 6);
        horizontalLayout_8->setStretch(2, 6);

        gridLayout->addLayout(horizontalLayout_8, 2, 1, 1, 1);

        label_1 = new QLabel(DuanMianForm);
        label_1->setObjectName(QString::fromUtf8("label_1"));

        gridLayout->addWidget(label_1, 0, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        inSetToZero = new QPushButton(DuanMianForm);
        buttonGroup = new QButtonGroup(DuanMianForm);
        buttonGroup->setObjectName(QString::fromUtf8("buttonGroup"));
        buttonGroup->addButton(inSetToZero);
        inSetToZero->setObjectName(QString::fromUtf8("inSetToZero"));
        sizePolicy.setHeightForWidth(inSetToZero->sizePolicy().hasHeightForWidth());
        inSetToZero->setSizePolicy(sizePolicy);
        inSetToZero->setCheckable(true);
        inSetToZero->setChecked(true);

        horizontalLayout->addWidget(inSetToZero);

        pushButton_6 = new QPushButton(DuanMianForm);
        buttonGroup->addButton(pushButton_6);
        pushButton_6->setObjectName(QString::fromUtf8("pushButton_6"));
        sizePolicy.setHeightForWidth(pushButton_6->sizePolicy().hasHeightForWidth());
        pushButton_6->setSizePolicy(sizePolicy);
        pushButton_6->setCheckable(true);
        pushButton_6->setChecked(false);

        horizontalLayout->addWidget(pushButton_6);


        gridLayout->addLayout(horizontalLayout, 5, 1, 1, 1);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        btnY = new QPushButton(DuanMianForm);
        btnY->setObjectName(QString::fromUtf8("btnY"));
        sizePolicy.setHeightForWidth(btnY->sizePolicy().hasHeightForWidth());
        btnY->setSizePolicy(sizePolicy);
        btnY->setFont(font);
        btnY->setCheckable(true);

        horizontalLayout_7->addWidget(btnY);

        btnY_P = new QPushButton(DuanMianForm);
        btnY_P->setObjectName(QString::fromUtf8("btnY_P"));
        btnY_P->setEnabled(false);
        sizePolicy.setHeightForWidth(btnY_P->sizePolicy().hasHeightForWidth());
        btnY_P->setSizePolicy(sizePolicy);
        btnY_P->setFont(font1);
        btnY_P->setCheckable(true);

        horizontalLayout_7->addWidget(btnY_P);

        btnY_N = new QPushButton(DuanMianForm);
        btnY_N->setObjectName(QString::fromUtf8("btnY_N"));
        btnY_N->setEnabled(false);
        sizePolicy.setHeightForWidth(btnY_N->sizePolicy().hasHeightForWidth());
        btnY_N->setSizePolicy(sizePolicy);
        btnY_N->setFont(font1);
        btnY_N->setCheckable(true);

        horizontalLayout_7->addWidget(btnY_N);

        horizontalLayout_7->setStretch(0, 1);
        horizontalLayout_7->setStretch(1, 6);
        horizontalLayout_7->setStretch(2, 6);

        gridLayout->addLayout(horizontalLayout_7, 1, 1, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        btnU = new QPushButton(DuanMianForm);
        btnU->setObjectName(QString::fromUtf8("btnU"));
        sizePolicy.setHeightForWidth(btnU->sizePolicy().hasHeightForWidth());
        btnU->setSizePolicy(sizePolicy);
        btnU->setFont(font);
        btnU->setCheckable(true);

        horizontalLayout_3->addWidget(btnU);

        btnU_P = new QPushButton(DuanMianForm);
        btnU_P->setObjectName(QString::fromUtf8("btnU_P"));
        btnU_P->setEnabled(false);
        sizePolicy.setHeightForWidth(btnU_P->sizePolicy().hasHeightForWidth());
        btnU_P->setSizePolicy(sizePolicy);
        btnU_P->setFont(font1);
        btnU_P->setCheckable(true);

        horizontalLayout_3->addWidget(btnU_P);

        btnU_N = new QPushButton(DuanMianForm);
        btnU_N->setObjectName(QString::fromUtf8("btnU_N"));
        btnU_N->setEnabled(false);
        sizePolicy.setHeightForWidth(btnU_N->sizePolicy().hasHeightForWidth());
        btnU_N->setSizePolicy(sizePolicy);
        btnU_N->setFont(font1);
        btnU_N->setCheckable(true);

        horizontalLayout_3->addWidget(btnU_N);

        horizontalLayout_3->setStretch(0, 1);
        horizontalLayout_3->setStretch(1, 6);
        horizontalLayout_3->setStretch(2, 6);

        gridLayout->addLayout(horizontalLayout_3, 3, 1, 1, 1);

        label_4 = new QLabel(DuanMianForm);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 4, 0, 1, 1);

        inBack = new QLineEdit4AxisValue(DuanMianForm);
        inBack->setObjectName(QString::fromUtf8("inBack"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(inBack->sizePolicy().hasHeightForWidth());
        inBack->setSizePolicy(sizePolicy1);
        inBack->setFont(font);

        gridLayout->addWidget(inBack, 4, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 2, 1, 1);


        horizontalLayout_10->addLayout(gridLayout);


        verticalLayout_2->addLayout(horizontalLayout_10);

        verticalSpacer2 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer2);


        horizontalLayout_9->addLayout(verticalLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        labelPix = new QLabel(DuanMianForm);
        labelPix->setObjectName(QString::fromUtf8("labelPix"));

        verticalLayout->addWidget(labelPix);

        textBrowser = new QTextBrowser(DuanMianForm);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));

        verticalLayout->addWidget(textBrowser);

        verticalLayout->setStretch(0, 2);
        verticalLayout->setStretch(1, 3);

        horizontalLayout_9->addLayout(verticalLayout);

        horizontalLayout_9->setStretch(0, 3);
        horizontalLayout_9->setStretch(1, 1);

        retranslateUi(DuanMianForm);

        QMetaObject::connectSlotsByName(DuanMianForm);
    } // setupUi

    void retranslateUi(QWidget *DuanMianForm)
    {
        DuanMianForm->setWindowTitle(QCoreApplication::translate("DuanMianForm", "Form", nullptr));
        btnX->setText(QCoreApplication::translate("DuanMianForm", "X", nullptr));
        btnX_P->setText(QCoreApplication::translate("DuanMianForm", "+", nullptr));
        btnX_N->setText(QCoreApplication::translate("DuanMianForm", "-", nullptr));
        label_3->setText(QCoreApplication::translate("DuanMianForm", "\345\235\220\346\240\2070\350\256\276\345\256\232", nullptr));
        btnZ->setText(QCoreApplication::translate("DuanMianForm", "Z", nullptr));
        btnZ_P->setText(QCoreApplication::translate("DuanMianForm", "+", nullptr));
        btnZ_N->setText(QCoreApplication::translate("DuanMianForm", "-", nullptr));
        label_1->setText(QCoreApplication::translate("DuanMianForm", "\346\225\260\346\215\256\350\276\223\345\205\245", nullptr));
        inSetToZero->setText(QCoreApplication::translate("DuanMianForm", "ON", nullptr));
        pushButton_6->setText(QCoreApplication::translate("DuanMianForm", "OFF", nullptr));
        btnY->setText(QCoreApplication::translate("DuanMianForm", "Y", nullptr));
        btnY_P->setText(QCoreApplication::translate("DuanMianForm", "+", nullptr));
        btnY_N->setText(QCoreApplication::translate("DuanMianForm", "-", nullptr));
        btnU->setText(QCoreApplication::translate("DuanMianForm", "U", nullptr));
        btnU_P->setText(QCoreApplication::translate("DuanMianForm", "+", nullptr));
        btnU_N->setText(QCoreApplication::translate("DuanMianForm", "-", nullptr));
        label_4->setText(QCoreApplication::translate("DuanMianForm", "\346\216\245\350\247\246\346\204\237\347\237\245\345\233\236\351\200\200\345\200\274", nullptr));
        inBack->setText(QCoreApplication::translate("DuanMianForm", "0.5", nullptr));
        labelPix->setText(QString());
        textBrowser->setHtml(QCoreApplication::translate("DuanMianForm", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Microsoft YaHei UI'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'SimSun'; font-size:20pt; font-weight:696;\">[\350\275\264\345\217\267]</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'SimSun'; font-size:20pt; font-weight:696;\">\345\212\250\344\275\234\347\232\204\345\257\271\345\272\224\350\275\264\345\217\267\343"
                        "\200\202</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'SimSun'; font-size:20pt; font-weight:696;\">[\346\226\271\345\220\221]</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'SimSun'; font-size:20pt; font-weight:696;\">\345\212\250\344\275\234\347\232\204\346\226\271\345\220\221\357\274\214\342\200\234+\342\200\235\344\270\272\345\257\271\345\272\224\350\275\264\346\255\243\346\226\271\345\220\221\357\274\214\342\200\234-\342\200\235\344\270\272\345\257\271\345\272\224\350\275\264\347\232\204\350\264\237\346\226\271\345\220\221\343\200\202</span></p></body></html>", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DuanMianForm: public Ui_DuanMianForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DUANMIANFORM_H
