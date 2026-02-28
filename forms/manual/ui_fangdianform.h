/********************************************************************************
** Form generated from reading UI file 'fangdianform.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FANGDIANFORM_H
#define UI_FANGDIANFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
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

class Ui_FangDianForm
{
public:
    QHBoxLayout *horizontalLayout_9;
    QVBoxLayout *verticalLayout_2;
    QSpacerItem *verticalSpacer1;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *btnX;
    QPushButton *btnX_3_P;
    QPushButton *btnX_3_N;
    QLabel *label_4;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *btnU;
    QPushButton *btnU_3_P;
    QPushButton *btnU_3_N;
    QHBoxLayout *horizontalLayout_10;
    QLineEdit4Fdcs *inJgtj;
    QHBoxLayout *horizontalLayout_7;
    QPushButton *btnY;
    QPushButton *btnY_3_P;
    QPushButton *btnY_3_N;
    QHBoxLayout *horizontalLayout_8;
    QPushButton *btnZ;
    QPushButton *btnZ_3_P;
    QPushButton *btnZ_3_N;
    QLabel *label_1;
    QLabel *label;
    QLineEdit4AxisValue *inHtl;
    QHBoxLayout *inJgtjDetail;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer2;
    QVBoxLayout *verticalLayout;
    QLabel *labelPix;
    QTextBrowser *textBrowser;

    void setupUi(QWidget *FangDianForm)
    {
        if (FangDianForm->objectName().isEmpty())
            FangDianForm->setObjectName(QString::fromUtf8("FangDianForm"));
        FangDianForm->resize(1081, 610);
        horizontalLayout_9 = new QHBoxLayout(FangDianForm);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalSpacer1 = new QSpacerItem(20, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);

        verticalLayout_2->addItem(verticalSpacer1);

        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(30, 30, 30, 30);
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        btnX = new QPushButton(FangDianForm);
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

        btnX_3_P = new QPushButton(FangDianForm);
        btnX_3_P->setObjectName(QString::fromUtf8("btnX_3_P"));
        btnX_3_P->setEnabled(false);
        sizePolicy.setHeightForWidth(btnX_3_P->sizePolicy().hasHeightForWidth());
        btnX_3_P->setSizePolicy(sizePolicy);
        QFont font1;
        font1.setPointSize(30);
        btnX_3_P->setFont(font1);
        btnX_3_P->setCheckable(true);

        horizontalLayout_4->addWidget(btnX_3_P);

        btnX_3_N = new QPushButton(FangDianForm);
        btnX_3_N->setObjectName(QString::fromUtf8("btnX_3_N"));
        btnX_3_N->setEnabled(false);
        sizePolicy.setHeightForWidth(btnX_3_N->sizePolicy().hasHeightForWidth());
        btnX_3_N->setSizePolicy(sizePolicy);
        btnX_3_N->setFont(font1);
        btnX_3_N->setCheckable(true);

        horizontalLayout_4->addWidget(btnX_3_N);


        gridLayout->addLayout(horizontalLayout_4, 0, 1, 1, 1);

        label_4 = new QLabel(FangDianForm);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 4, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 1, 2, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        btnU = new QPushButton(FangDianForm);
        btnU->setObjectName(QString::fromUtf8("btnU"));
        sizePolicy.setHeightForWidth(btnU->sizePolicy().hasHeightForWidth());
        btnU->setSizePolicy(sizePolicy);
        btnU->setFont(font);
        btnU->setCheckable(true);

        horizontalLayout_3->addWidget(btnU);

        btnU_3_P = new QPushButton(FangDianForm);
        btnU_3_P->setObjectName(QString::fromUtf8("btnU_3_P"));
        btnU_3_P->setEnabled(false);
        sizePolicy.setHeightForWidth(btnU_3_P->sizePolicy().hasHeightForWidth());
        btnU_3_P->setSizePolicy(sizePolicy);
        btnU_3_P->setFont(font1);
        btnU_3_P->setCheckable(true);

        horizontalLayout_3->addWidget(btnU_3_P);

        btnU_3_N = new QPushButton(FangDianForm);
        btnU_3_N->setObjectName(QString::fromUtf8("btnU_3_N"));
        btnU_3_N->setEnabled(false);
        sizePolicy.setHeightForWidth(btnU_3_N->sizePolicy().hasHeightForWidth());
        btnU_3_N->setSizePolicy(sizePolicy);
        btnU_3_N->setFont(font1);
        btnU_3_N->setCheckable(true);

        horizontalLayout_3->addWidget(btnU_3_N);


        gridLayout->addLayout(horizontalLayout_3, 3, 1, 1, 1);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        inJgtj = new QLineEdit4Fdcs(FangDianForm);
        inJgtj->setObjectName(QString::fromUtf8("inJgtj"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(inJgtj->sizePolicy().hasHeightForWidth());
        inJgtj->setSizePolicy(sizePolicy1);
        inJgtj->setFont(font);

        horizontalLayout_10->addWidget(inJgtj);


        gridLayout->addLayout(horizontalLayout_10, 4, 1, 1, 1);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        btnY = new QPushButton(FangDianForm);
        btnY->setObjectName(QString::fromUtf8("btnY"));
        sizePolicy.setHeightForWidth(btnY->sizePolicy().hasHeightForWidth());
        btnY->setSizePolicy(sizePolicy);
        btnY->setFont(font);
        btnY->setCheckable(true);

        horizontalLayout_7->addWidget(btnY);

        btnY_3_P = new QPushButton(FangDianForm);
        btnY_3_P->setObjectName(QString::fromUtf8("btnY_3_P"));
        btnY_3_P->setEnabled(false);
        sizePolicy.setHeightForWidth(btnY_3_P->sizePolicy().hasHeightForWidth());
        btnY_3_P->setSizePolicy(sizePolicy);
        btnY_3_P->setFont(font1);
        btnY_3_P->setCheckable(true);

        horizontalLayout_7->addWidget(btnY_3_P);

        btnY_3_N = new QPushButton(FangDianForm);
        btnY_3_N->setObjectName(QString::fromUtf8("btnY_3_N"));
        btnY_3_N->setEnabled(false);
        sizePolicy.setHeightForWidth(btnY_3_N->sizePolicy().hasHeightForWidth());
        btnY_3_N->setSizePolicy(sizePolicy);
        btnY_3_N->setFont(font1);
        btnY_3_N->setCheckable(true);

        horizontalLayout_7->addWidget(btnY_3_N);


        gridLayout->addLayout(horizontalLayout_7, 1, 1, 1, 1);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        btnZ = new QPushButton(FangDianForm);
        btnZ->setObjectName(QString::fromUtf8("btnZ"));
        sizePolicy.setHeightForWidth(btnZ->sizePolicy().hasHeightForWidth());
        btnZ->setSizePolicy(sizePolicy);
        btnZ->setFont(font);
        btnZ->setCheckable(true);

        horizontalLayout_8->addWidget(btnZ);

        btnZ_3_P = new QPushButton(FangDianForm);
        btnZ_3_P->setObjectName(QString::fromUtf8("btnZ_3_P"));
        btnZ_3_P->setEnabled(false);
        sizePolicy.setHeightForWidth(btnZ_3_P->sizePolicy().hasHeightForWidth());
        btnZ_3_P->setSizePolicy(sizePolicy);
        btnZ_3_P->setFont(font1);
        btnZ_3_P->setCheckable(true);

        horizontalLayout_8->addWidget(btnZ_3_P);

        btnZ_3_N = new QPushButton(FangDianForm);
        btnZ_3_N->setObjectName(QString::fromUtf8("btnZ_3_N"));
        btnZ_3_N->setEnabled(false);
        sizePolicy.setHeightForWidth(btnZ_3_N->sizePolicy().hasHeightForWidth());
        btnZ_3_N->setSizePolicy(sizePolicy);
        btnZ_3_N->setFont(font1);
        btnZ_3_N->setCheckable(true);

        horizontalLayout_8->addWidget(btnZ_3_N);


        gridLayout->addLayout(horizontalLayout_8, 2, 1, 1, 1);

        label_1 = new QLabel(FangDianForm);
        label_1->setObjectName(QString::fromUtf8("label_1"));

        gridLayout->addWidget(label_1, 0, 0, 1, 1);

        label = new QLabel(FangDianForm);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 5, 0, 1, 1);

        inHtl = new QLineEdit4AxisValue(FangDianForm);
        inHtl->setObjectName(QString::fromUtf8("inHtl"));

        gridLayout->addWidget(inHtl, 5, 1, 1, 1);


        verticalLayout_2->addLayout(gridLayout);

        inJgtjDetail = new QHBoxLayout();
        inJgtjDetail->setSpacing(0);
        inJgtjDetail->setObjectName(QString::fromUtf8("inJgtjDetail"));
        inJgtjDetail->setContentsMargins(0, 10, 0, 10);
        horizontalSpacer_2 = new QSpacerItem(400, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Minimum);

        inJgtjDetail->addItem(horizontalSpacer_2);


        verticalLayout_2->addLayout(inJgtjDetail);

        verticalSpacer2 = new QSpacerItem(20, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);

        verticalLayout_2->addItem(verticalSpacer2);


        horizontalLayout_9->addLayout(verticalLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        labelPix = new QLabel(FangDianForm);
        labelPix->setObjectName(QString::fromUtf8("labelPix"));

        verticalLayout->addWidget(labelPix);

        textBrowser = new QTextBrowser(FangDianForm);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));

        verticalLayout->addWidget(textBrowser);

        verticalLayout->setStretch(0, 2);
        verticalLayout->setStretch(1, 3);

        horizontalLayout_9->addLayout(verticalLayout);

        horizontalLayout_9->setStretch(0, 3);
        horizontalLayout_9->setStretch(1, 1);

        retranslateUi(FangDianForm);

        QMetaObject::connectSlotsByName(FangDianForm);
    } // setupUi

    void retranslateUi(QWidget *FangDianForm)
    {
        FangDianForm->setWindowTitle(QCoreApplication::translate("FangDianForm", "Form", nullptr));
        btnX->setText(QCoreApplication::translate("FangDianForm", "X", nullptr));
        btnX_3_P->setText(QCoreApplication::translate("FangDianForm", "+", nullptr));
        btnX_3_N->setText(QCoreApplication::translate("FangDianForm", "-", nullptr));
        label_4->setText(QCoreApplication::translate("FangDianForm", "JGTJ", nullptr));
        btnU->setText(QCoreApplication::translate("FangDianForm", "U", nullptr));
        btnU_3_P->setText(QCoreApplication::translate("FangDianForm", "+", nullptr));
        btnU_3_N->setText(QCoreApplication::translate("FangDianForm", "-", nullptr));
        inJgtj->setText(QCoreApplication::translate("FangDianForm", "777", nullptr));
        btnY->setText(QCoreApplication::translate("FangDianForm", "Y", nullptr));
        btnY_3_P->setText(QCoreApplication::translate("FangDianForm", "+", nullptr));
        btnY_3_N->setText(QCoreApplication::translate("FangDianForm", "-", nullptr));
        btnZ->setText(QCoreApplication::translate("FangDianForm", "Z", nullptr));
        btnZ_3_P->setText(QCoreApplication::translate("FangDianForm", "+", nullptr));
        btnZ_3_N->setText(QCoreApplication::translate("FangDianForm", "-", nullptr));
        label_1->setText(QCoreApplication::translate("FangDianForm", "SJSR", nullptr));
        label->setText(QCoreApplication::translate("FangDianForm", "HTL", nullptr));
        labelPix->setText(QString());
        textBrowser->setHtml(QString());
    } // retranslateUi

};

namespace Ui {
    class FangDianForm: public Ui_FangDianForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FANGDIANFORM_H
