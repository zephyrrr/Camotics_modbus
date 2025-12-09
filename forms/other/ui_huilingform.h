/********************************************************************************
** Form generated from reading UI file 'huilingform.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HUILINGFORM_H
#define UI_HUILINGFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_HuiLingFormClass
{
public:
    QHBoxLayout *horizontalLayout_9;
    QVBoxLayout *verticalLayout_2;
    QSpacerItem *verticalSpacer1;
    QFormLayout *formLayout;
    QLabel *label_1;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *btnX;
    QPushButton *btnX_P;
    QPushButton *btnX_N;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_7;
    QPushButton *btnY;
    QPushButton *btnY_P;
    QPushButton *btnY_N;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout_8;
    QPushButton *btnZ;
    QPushButton *btnZ_P;
    QPushButton *btnZ_N;
    QSpacerItem *horizontalSpacer_3;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *btnU;
    QPushButton *btnU_P;
    QPushButton *btnU_N;
    QSpacerItem *horizontalSpacer_4;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *verticalSpacer2;
    QVBoxLayout *verticalLayout;
    QLabel *labelPix;
    QTextBrowser *textBrowser;
    QButtonGroup *buttonGroup_3;
    QButtonGroup *buttonGroup_2;
    QButtonGroup *buttonGroup_4;
    QButtonGroup *buttonGroup;
    QButtonGroup *buttonGroup_5;

    void setupUi(QWidget *HuiLingForm)
    {
        if (HuiLingForm->objectName().isEmpty())
            HuiLingForm->setObjectName(QString::fromUtf8("HuiLingForm"));
        HuiLingForm->resize(637, 654);
        horizontalLayout_9 = new QHBoxLayout(HuiLingForm);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalSpacer1 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer1);

        formLayout = new QFormLayout();
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        label_1 = new QLabel(HuiLingForm);
        label_1->setObjectName(QString::fromUtf8("label_1"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label_1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        btnX = new QPushButton(HuiLingForm);
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

        btnX_P = new QPushButton(HuiLingForm);
        buttonGroup_2 = new QButtonGroup(HuiLingForm);
        buttonGroup_2->setObjectName(QString::fromUtf8("buttonGroup_2"));
        buttonGroup_2->addButton(btnX_P);
        btnX_P->setObjectName(QString::fromUtf8("btnX_P"));
        btnX_P->setEnabled(false);
        sizePolicy.setHeightForWidth(btnX_P->sizePolicy().hasHeightForWidth());
        btnX_P->setSizePolicy(sizePolicy);
        QFont font1;
        font1.setPointSize(36);
        btnX_P->setFont(font1);
        btnX_P->setCheckable(true);

        horizontalLayout_4->addWidget(btnX_P);

        btnX_N = new QPushButton(HuiLingForm);
        buttonGroup_2->addButton(btnX_N);
        btnX_N->setObjectName(QString::fromUtf8("btnX_N"));
        btnX_N->setEnabled(false);
        sizePolicy.setHeightForWidth(btnX_N->sizePolicy().hasHeightForWidth());
        btnX_N->setSizePolicy(sizePolicy);
        btnX_N->setFont(font1);
        btnX_N->setCheckable(true);

        horizontalLayout_4->addWidget(btnX_N);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer);

        horizontalLayout_4->setStretch(0, 1);
        horizontalLayout_4->setStretch(1, 6);
        horizontalLayout_4->setStretch(2, 6);
        horizontalLayout_4->setStretch(3, 10);

        formLayout->setLayout(0, QFormLayout::FieldRole, horizontalLayout_4);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        btnY = new QPushButton(HuiLingForm);
        btnY->setObjectName(QString::fromUtf8("btnY"));
        sizePolicy.setHeightForWidth(btnY->sizePolicy().hasHeightForWidth());
        btnY->setSizePolicy(sizePolicy);
        btnY->setFont(font);
        btnY->setCheckable(true);

        horizontalLayout_7->addWidget(btnY);

        btnY_P = new QPushButton(HuiLingForm);
        buttonGroup_3 = new QButtonGroup(HuiLingForm);
        buttonGroup_3->setObjectName(QString::fromUtf8("buttonGroup_3"));
        buttonGroup_3->addButton(btnY_P);
        btnY_P->setObjectName(QString::fromUtf8("btnY_P"));
        btnY_P->setEnabled(false);
        sizePolicy.setHeightForWidth(btnY_P->sizePolicy().hasHeightForWidth());
        btnY_P->setSizePolicy(sizePolicy);
        btnY_P->setFont(font1);
        btnY_P->setCheckable(true);

        horizontalLayout_7->addWidget(btnY_P);

        btnY_N = new QPushButton(HuiLingForm);
        buttonGroup_3->addButton(btnY_N);
        btnY_N->setObjectName(QString::fromUtf8("btnY_N"));
        btnY_N->setEnabled(false);
        sizePolicy.setHeightForWidth(btnY_N->sizePolicy().hasHeightForWidth());
        btnY_N->setSizePolicy(sizePolicy);
        btnY_N->setFont(font1);
        btnY_N->setCheckable(true);

        horizontalLayout_7->addWidget(btnY_N);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_2);

        horizontalLayout_7->setStretch(0, 1);
        horizontalLayout_7->setStretch(1, 6);
        horizontalLayout_7->setStretch(2, 6);
        horizontalLayout_7->setStretch(3, 10);

        formLayout->setLayout(1, QFormLayout::FieldRole, horizontalLayout_7);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        btnZ = new QPushButton(HuiLingForm);
        btnZ->setObjectName(QString::fromUtf8("btnZ"));
        sizePolicy.setHeightForWidth(btnZ->sizePolicy().hasHeightForWidth());
        btnZ->setSizePolicy(sizePolicy);
        btnZ->setFont(font);
        btnZ->setCheckable(true);

        horizontalLayout_8->addWidget(btnZ);

        btnZ_P = new QPushButton(HuiLingForm);
        buttonGroup_4 = new QButtonGroup(HuiLingForm);
        buttonGroup_4->setObjectName(QString::fromUtf8("buttonGroup_4"));
        buttonGroup_4->addButton(btnZ_P);
        btnZ_P->setObjectName(QString::fromUtf8("btnZ_P"));
        btnZ_P->setEnabled(false);
        sizePolicy.setHeightForWidth(btnZ_P->sizePolicy().hasHeightForWidth());
        btnZ_P->setSizePolicy(sizePolicy);
        btnZ_P->setFont(font1);
        btnZ_P->setCheckable(true);

        horizontalLayout_8->addWidget(btnZ_P);

        btnZ_N = new QPushButton(HuiLingForm);
        buttonGroup_4->addButton(btnZ_N);
        btnZ_N->setObjectName(QString::fromUtf8("btnZ_N"));
        btnZ_N->setEnabled(false);
        sizePolicy.setHeightForWidth(btnZ_N->sizePolicy().hasHeightForWidth());
        btnZ_N->setSizePolicy(sizePolicy);
        btnZ_N->setFont(font1);
        btnZ_N->setCheckable(true);

        horizontalLayout_8->addWidget(btnZ_N);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_3);

        horizontalLayout_8->setStretch(0, 1);
        horizontalLayout_8->setStretch(1, 6);
        horizontalLayout_8->setStretch(2, 6);
        horizontalLayout_8->setStretch(3, 10);

        formLayout->setLayout(2, QFormLayout::FieldRole, horizontalLayout_8);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        btnU = new QPushButton(HuiLingForm);
        btnU->setObjectName(QString::fromUtf8("btnU"));
        sizePolicy.setHeightForWidth(btnU->sizePolicy().hasHeightForWidth());
        btnU->setSizePolicy(sizePolicy);
        btnU->setFont(font);
        btnU->setCheckable(true);

        horizontalLayout_3->addWidget(btnU);

        btnU_P = new QPushButton(HuiLingForm);
        buttonGroup_5 = new QButtonGroup(HuiLingForm);
        buttonGroup_5->setObjectName(QString::fromUtf8("buttonGroup_5"));
        buttonGroup_5->addButton(btnU_P);
        btnU_P->setObjectName(QString::fromUtf8("btnU_P"));
        btnU_P->setEnabled(false);
        sizePolicy.setHeightForWidth(btnU_P->sizePolicy().hasHeightForWidth());
        btnU_P->setSizePolicy(sizePolicy);
        btnU_P->setFont(font1);
        btnU_P->setCheckable(true);

        horizontalLayout_3->addWidget(btnU_P);

        btnU_N = new QPushButton(HuiLingForm);
        buttonGroup_5->addButton(btnU_N);
        btnU_N->setObjectName(QString::fromUtf8("btnU_N"));
        btnU_N->setEnabled(false);
        sizePolicy.setHeightForWidth(btnU_N->sizePolicy().hasHeightForWidth());
        btnU_N->setSizePolicy(sizePolicy);
        btnU_N->setFont(font1);
        btnU_N->setCheckable(true);

        horizontalLayout_3->addWidget(btnU_N);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_4);

        horizontalLayout_3->setStretch(0, 1);
        horizontalLayout_3->setStretch(1, 6);
        horizontalLayout_3->setStretch(2, 6);
        horizontalLayout_3->setStretch(3, 10);

        formLayout->setLayout(3, QFormLayout::FieldRole, horizontalLayout_3);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));

        formLayout->setLayout(4, QFormLayout::FieldRole, horizontalLayout);


        verticalLayout_2->addLayout(formLayout);

        verticalSpacer2 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer2);

        verticalLayout_2->setStretch(0, 1);
        verticalLayout_2->setStretch(1, 8);
        verticalLayout_2->setStretch(2, 1);

        horizontalLayout_9->addLayout(verticalLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        labelPix = new QLabel(HuiLingForm);
        labelPix->setObjectName(QString::fromUtf8("labelPix"));

        verticalLayout->addWidget(labelPix);

        textBrowser = new QTextBrowser(HuiLingForm);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));

        verticalLayout->addWidget(textBrowser);

        verticalLayout->setStretch(0, 2);
        verticalLayout->setStretch(1, 3);

        horizontalLayout_9->addLayout(verticalLayout);

        horizontalLayout_9->setStretch(0, 3);
        horizontalLayout_9->setStretch(1, 1);

        retranslateUi(HuiLingForm);

        QMetaObject::connectSlotsByName(HuiLingForm);
    } // setupUi

    void retranslateUi(QWidget *HuiLingForm)
    {
        HuiLingForm->setWindowTitle(QCoreApplication::translate("HuiLingFormClass", "Form", nullptr));
        label_1->setText(QCoreApplication::translate("HuiLingFormClass", "HLFX", nullptr));
        btnX->setText(QCoreApplication::translate("HuiLingFormClass", "X", nullptr));
        btnX_P->setText(QCoreApplication::translate("HuiLingFormClass", "+", nullptr));
        btnX_N->setText(QCoreApplication::translate("HuiLingFormClass", "-", nullptr));
        btnY->setText(QCoreApplication::translate("HuiLingFormClass", "Y", nullptr));
        btnY_P->setText(QCoreApplication::translate("HuiLingFormClass", "+", nullptr));
        btnY_N->setText(QCoreApplication::translate("HuiLingFormClass", "-", nullptr));
        btnZ->setText(QCoreApplication::translate("HuiLingFormClass", "Z", nullptr));
        btnZ_P->setText(QCoreApplication::translate("HuiLingFormClass", "+", nullptr));
        btnZ_N->setText(QCoreApplication::translate("HuiLingFormClass", "-", nullptr));
        btnU->setText(QCoreApplication::translate("HuiLingFormClass", "U", nullptr));
        btnU_P->setText(QCoreApplication::translate("HuiLingFormClass", "+", nullptr));
        btnU_N->setText(QCoreApplication::translate("HuiLingFormClass", "-", nullptr));
        labelPix->setText(QString());
        textBrowser->setHtml(QString());
    } // retranslateUi

};

namespace Ui {
    class HuiLingFormClass: public Ui_HuiLingFormClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HUILINGFORM_H
