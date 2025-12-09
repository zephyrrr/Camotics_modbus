/********************************************************************************
** Form generated from reading UI file 'weizhiyidongform.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WEIZHIYIDONGFORM_H
#define UI_WEIZHIYIDONGFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "widgets/qlineedit4axisvalue.h"

QT_BEGIN_NAMESPACE

class Ui_WeiZhiYiDongForm
{
public:
    QHBoxLayout *horizontalLayout_9;
    QVBoxLayout *verticalLayout_2;
    QSpacerItem *verticalSpacer1;
    QHBoxLayout *horizontalLayout_3;
    QTabWidget *tabWidget;
    QWidget *tab;
    QHBoxLayout *horizontalLayout_31;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout_8;
    QPushButton *btnZ;
    QLineEdit4AxisValue *inZ;
    QLabel *label_2;
    QLabel *label_3;
    QSpacerItem *verticalSpacer_5;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *inTouchSense;
    QPushButton *pushButton_8;
    QLineEdit4Coor *inCoor;
    QHBoxLayout *horizontalLayout_311;
    QPushButton *btnU;
    QLineEdit4AxisValue *inU;
    QLabel *label_4;
    QHBoxLayout *horizontalLayout;
    QPushButton *inAbsolute;
    QPushButton *pushButton_6;
    QLabel *label_1;
    QHBoxLayout *horizontalLayout_7;
    QPushButton *btnY;
    QLineEdit4AxisValue *inY;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *btnX;
    QLineEdit4AxisValue *inX;
    QSpacerItem *verticalSpacer_6;
    QWidget *tab_2;
    QHBoxLayout *horizontalLayout_14;
    QGridLayout *gridLayout_2;
    QHBoxLayout *horizontalLayout_13;
    QPushButton *btnZ_2;
    QLineEdit4AxisValue *inZ_2;
    QHBoxLayout *horizontalLayout_10;
    QPushButton *inTouchSense_2;
    QPushButton *pushButton_9;
    QHBoxLayout *horizontalLayout_11;
    QPushButton *btnX_2;
    QLineEdit4AxisValue *inX_2;
    QLineEdit4Coor *inCoor_2;
    QHBoxLayout *horizontalLayout_5;
    QPushButton *btnU_2;
    QLineEdit4AxisValue *inU_2;
    QSpacerItem *verticalSpacer_7;
    QSpacerItem *verticalSpacer_8;
    QLabel *label_8;
    QLabel *label_9;
    QLabel *label_5;
    QHBoxLayout *horizontalLayout_12;
    QPushButton *btnY_2;
    QLineEdit4AxisValue *inY_2;
    QWidget *tab_3;
    QHBoxLayout *horizontalLayout_21;
    QGridLayout *gridLayout_3;
    QHBoxLayout *horizontalLayout_18;
    QPushButton *btnX_3;
    QPushButton *btnX_3_P;
    QPushButton *btnX_3_N;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout_19;
    QPushButton *btnY_3;
    QPushButton *btnY_3_P;
    QPushButton *btnY_3_N;
    QHBoxLayout *horizontalLayout_20;
    QPushButton *btnZ_3;
    QPushButton *btnZ_3_P;
    QPushButton *btnZ_3_N;
    QLabel *label_11;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_15;
    QPushButton *btnU_3;
    QPushButton *btnU_3_P;
    QPushButton *btnU_3_N;
    QSpacerItem *horizontalSpacer1;
    QSpacerItem *verticalSpacer2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_17;
    QLabel *labelPix;
    QTextBrowser *textBrowser;
    QButtonGroup *buttonGroup;
    QButtonGroup *buttonGroup_7;
    QButtonGroup *buttonGroup_2;

    void setupUi(QWidget *WeiZhiYiDongForm)
    {
        if (WeiZhiYiDongForm->objectName().isEmpty())
            WeiZhiYiDongForm->setObjectName(QString::fromUtf8("WeiZhiYiDongForm"));
        WeiZhiYiDongForm->resize(1019, 546);
        horizontalLayout_9 = new QHBoxLayout(WeiZhiYiDongForm);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalSpacer1 = new QSpacerItem(20, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);

        verticalLayout_2->addItem(verticalSpacer1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        tabWidget = new QTabWidget(WeiZhiYiDongForm);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setStyleSheet(QString::fromUtf8(""));
        tabWidget->setTabShape(QTabWidget::TabShape::Rounded);
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        horizontalLayout_31 = new QHBoxLayout(tab);
        horizontalLayout_31->setObjectName(QString::fromUtf8("horizontalLayout_31"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(30, 30, 30, 30);
        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        btnZ = new QPushButton(tab);
        btnZ->setObjectName(QString::fromUtf8("btnZ"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(btnZ->sizePolicy().hasHeightForWidth());
        btnZ->setSizePolicy(sizePolicy);
        QFont font;
        font.setPointSize(20);
        btnZ->setFont(font);
        btnZ->setCheckable(true);

        horizontalLayout_8->addWidget(btnZ);

        inZ = new QLineEdit4AxisValue(tab);
        inZ->setObjectName(QString::fromUtf8("inZ"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(inZ->sizePolicy().hasHeightForWidth());
        inZ->setSizePolicy(sizePolicy1);
        inZ->setFont(font);
        inZ->setReadOnly(true);

        horizontalLayout_8->addWidget(inZ);


        gridLayout->addLayout(horizontalLayout_8, 3, 1, 1, 1);

        label_2 = new QLabel(tab);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy2);

        gridLayout->addWidget(label_2, 7, 0, 1, 1);

        label_3 = new QLabel(tab);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        sizePolicy2.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy2);

        gridLayout->addWidget(label_3, 6, 0, 1, 1);

        verticalSpacer_5 = new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(verticalSpacer_5, 0, 1, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        inTouchSense = new QPushButton(tab);
        buttonGroup_2 = new QButtonGroup(WeiZhiYiDongForm);
        buttonGroup_2->setObjectName(QString::fromUtf8("buttonGroup_2"));
        buttonGroup_2->addButton(inTouchSense);
        inTouchSense->setObjectName(QString::fromUtf8("inTouchSense"));
        sizePolicy.setHeightForWidth(inTouchSense->sizePolicy().hasHeightForWidth());
        inTouchSense->setSizePolicy(sizePolicy);
        inTouchSense->setCheckable(true);
        inTouchSense->setChecked(true);

        horizontalLayout_2->addWidget(inTouchSense);

        pushButton_8 = new QPushButton(tab);
        buttonGroup_2->addButton(pushButton_8);
        pushButton_8->setObjectName(QString::fromUtf8("pushButton_8"));
        sizePolicy.setHeightForWidth(pushButton_8->sizePolicy().hasHeightForWidth());
        pushButton_8->setSizePolicy(sizePolicy);
        pushButton_8->setCheckable(true);
        pushButton_8->setChecked(false);

        horizontalLayout_2->addWidget(pushButton_8);


        gridLayout->addLayout(horizontalLayout_2, 7, 1, 1, 1);

        inCoor = new QLineEdit4Coor(tab);
        inCoor->setObjectName(QString::fromUtf8("inCoor"));
        sizePolicy1.setHeightForWidth(inCoor->sizePolicy().hasHeightForWidth());
        inCoor->setSizePolicy(sizePolicy1);
        inCoor->setFont(font);

        gridLayout->addWidget(inCoor, 5, 1, 1, 1);

        horizontalLayout_311 = new QHBoxLayout();
        horizontalLayout_311->setObjectName(QString::fromUtf8("horizontalLayout_311"));
        btnU = new QPushButton(tab);
        btnU->setObjectName(QString::fromUtf8("btnU"));
        sizePolicy.setHeightForWidth(btnU->sizePolicy().hasHeightForWidth());
        btnU->setSizePolicy(sizePolicy);
        btnU->setFont(font);
        btnU->setCheckable(true);

        horizontalLayout_311->addWidget(btnU);

        inU = new QLineEdit4AxisValue(tab);
        inU->setObjectName(QString::fromUtf8("inU"));
        sizePolicy1.setHeightForWidth(inU->sizePolicy().hasHeightForWidth());
        inU->setSizePolicy(sizePolicy1);
        inU->setFont(font);
        inU->setReadOnly(true);

        horizontalLayout_311->addWidget(inU);


        gridLayout->addLayout(horizontalLayout_311, 4, 1, 1, 1);

        label_4 = new QLabel(tab);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        sizePolicy2.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy2);

        gridLayout->addWidget(label_4, 5, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        inAbsolute = new QPushButton(tab);
        buttonGroup = new QButtonGroup(WeiZhiYiDongForm);
        buttonGroup->setObjectName(QString::fromUtf8("buttonGroup"));
        buttonGroup->addButton(inAbsolute);
        inAbsolute->setObjectName(QString::fromUtf8("inAbsolute"));
        sizePolicy.setHeightForWidth(inAbsolute->sizePolicy().hasHeightForWidth());
        inAbsolute->setSizePolicy(sizePolicy);
        inAbsolute->setCheckable(true);
        inAbsolute->setChecked(true);

        horizontalLayout->addWidget(inAbsolute);

        pushButton_6 = new QPushButton(tab);
        buttonGroup->addButton(pushButton_6);
        pushButton_6->setObjectName(QString::fromUtf8("pushButton_6"));
        sizePolicy.setHeightForWidth(pushButton_6->sizePolicy().hasHeightForWidth());
        pushButton_6->setSizePolicy(sizePolicy);
        pushButton_6->setCheckable(true);
        pushButton_6->setChecked(false);

        horizontalLayout->addWidget(pushButton_6);


        gridLayout->addLayout(horizontalLayout, 6, 1, 1, 1);

        label_1 = new QLabel(tab);
        label_1->setObjectName(QString::fromUtf8("label_1"));
        sizePolicy2.setHeightForWidth(label_1->sizePolicy().hasHeightForWidth());
        label_1->setSizePolicy(sizePolicy2);

        gridLayout->addWidget(label_1, 1, 0, 1, 1);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        btnY = new QPushButton(tab);
        btnY->setObjectName(QString::fromUtf8("btnY"));
        sizePolicy.setHeightForWidth(btnY->sizePolicy().hasHeightForWidth());
        btnY->setSizePolicy(sizePolicy);
        btnY->setFont(font);
        btnY->setCheckable(true);

        horizontalLayout_7->addWidget(btnY);

        inY = new QLineEdit4AxisValue(tab);
        inY->setObjectName(QString::fromUtf8("inY"));
        sizePolicy1.setHeightForWidth(inY->sizePolicy().hasHeightForWidth());
        inY->setSizePolicy(sizePolicy1);
        inY->setFont(font);
        inY->setReadOnly(true);

        horizontalLayout_7->addWidget(inY);


        gridLayout->addLayout(horizontalLayout_7, 2, 1, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        btnX = new QPushButton(tab);
        btnX->setObjectName(QString::fromUtf8("btnX"));
        sizePolicy.setHeightForWidth(btnX->sizePolicy().hasHeightForWidth());
        btnX->setSizePolicy(sizePolicy);
        btnX->setFont(font);
        btnX->setCheckable(true);

        horizontalLayout_4->addWidget(btnX);

        inX = new QLineEdit4AxisValue(tab);
        inX->setObjectName(QString::fromUtf8("inX"));
        sizePolicy1.setHeightForWidth(inX->sizePolicy().hasHeightForWidth());
        inX->setSizePolicy(sizePolicy1);
        inX->setFont(font);
        inX->setReadOnly(true);

        horizontalLayout_4->addWidget(inX);


        gridLayout->addLayout(horizontalLayout_4, 1, 1, 1, 1);

        verticalSpacer_6 = new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(verticalSpacer_6, 8, 1, 1, 1);

        gridLayout->setColumnStretch(0, 1);

        horizontalLayout_31->addLayout(gridLayout);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        horizontalLayout_14 = new QHBoxLayout(tab_2);
        horizontalLayout_14->setObjectName(QString::fromUtf8("horizontalLayout_14"));
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(30, 30, 30, 30);
        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setObjectName(QString::fromUtf8("horizontalLayout_13"));
        btnZ_2 = new QPushButton(tab_2);
        btnZ_2->setObjectName(QString::fromUtf8("btnZ_2"));
        sizePolicy.setHeightForWidth(btnZ_2->sizePolicy().hasHeightForWidth());
        btnZ_2->setSizePolicy(sizePolicy);
        btnZ_2->setFont(font);
        btnZ_2->setCheckable(true);

        horizontalLayout_13->addWidget(btnZ_2);

        inZ_2 = new QLineEdit4AxisValue(tab_2);
        inZ_2->setObjectName(QString::fromUtf8("inZ_2"));
        sizePolicy1.setHeightForWidth(inZ_2->sizePolicy().hasHeightForWidth());
        inZ_2->setSizePolicy(sizePolicy1);
        inZ_2->setFont(font);
        inZ_2->setReadOnly(true);

        horizontalLayout_13->addWidget(inZ_2);


        gridLayout_2->addLayout(horizontalLayout_13, 3, 1, 1, 1);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        inTouchSense_2 = new QPushButton(tab_2);
        buttonGroup_7 = new QButtonGroup(WeiZhiYiDongForm);
        buttonGroup_7->setObjectName(QString::fromUtf8("buttonGroup_7"));
        buttonGroup_7->addButton(inTouchSense_2);
        inTouchSense_2->setObjectName(QString::fromUtf8("inTouchSense_2"));
        sizePolicy.setHeightForWidth(inTouchSense_2->sizePolicy().hasHeightForWidth());
        inTouchSense_2->setSizePolicy(sizePolicy);
        inTouchSense_2->setCheckable(true);
        inTouchSense_2->setChecked(true);

        horizontalLayout_10->addWidget(inTouchSense_2);

        pushButton_9 = new QPushButton(tab_2);
        buttonGroup_7->addButton(pushButton_9);
        pushButton_9->setObjectName(QString::fromUtf8("pushButton_9"));
        sizePolicy.setHeightForWidth(pushButton_9->sizePolicy().hasHeightForWidth());
        pushButton_9->setSizePolicy(sizePolicy);
        pushButton_9->setCheckable(true);
        pushButton_9->setChecked(false);

        horizontalLayout_10->addWidget(pushButton_9);


        gridLayout_2->addLayout(horizontalLayout_10, 6, 1, 1, 1);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        btnX_2 = new QPushButton(tab_2);
        btnX_2->setObjectName(QString::fromUtf8("btnX_2"));
        sizePolicy.setHeightForWidth(btnX_2->sizePolicy().hasHeightForWidth());
        btnX_2->setSizePolicy(sizePolicy);
        btnX_2->setFont(font);
        btnX_2->setCheckable(true);

        horizontalLayout_11->addWidget(btnX_2);

        inX_2 = new QLineEdit4AxisValue(tab_2);
        inX_2->setObjectName(QString::fromUtf8("inX_2"));
        sizePolicy1.setHeightForWidth(inX_2->sizePolicy().hasHeightForWidth());
        inX_2->setSizePolicy(sizePolicy1);
        inX_2->setFont(font);
        inX_2->setReadOnly(true);

        horizontalLayout_11->addWidget(inX_2);


        gridLayout_2->addLayout(horizontalLayout_11, 1, 1, 1, 1);

        inCoor_2 = new QLineEdit4Coor(tab_2);
        inCoor_2->setObjectName(QString::fromUtf8("inCoor_2"));
        sizePolicy1.setHeightForWidth(inCoor_2->sizePolicy().hasHeightForWidth());
        inCoor_2->setSizePolicy(sizePolicy1);
        inCoor_2->setFont(font);

        gridLayout_2->addWidget(inCoor_2, 5, 1, 1, 1);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        btnU_2 = new QPushButton(tab_2);
        btnU_2->setObjectName(QString::fromUtf8("btnU_2"));
        sizePolicy.setHeightForWidth(btnU_2->sizePolicy().hasHeightForWidth());
        btnU_2->setSizePolicy(sizePolicy);
        btnU_2->setFont(font);
        btnU_2->setCheckable(true);

        horizontalLayout_5->addWidget(btnU_2);

        inU_2 = new QLineEdit4AxisValue(tab_2);
        inU_2->setObjectName(QString::fromUtf8("inU_2"));
        sizePolicy1.setHeightForWidth(inU_2->sizePolicy().hasHeightForWidth());
        inU_2->setSizePolicy(sizePolicy1);
        inU_2->setFont(font);
        inU_2->setReadOnly(true);

        horizontalLayout_5->addWidget(inU_2);


        gridLayout_2->addLayout(horizontalLayout_5, 4, 1, 1, 1);

        verticalSpacer_7 = new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(verticalSpacer_7, 0, 1, 1, 1);

        verticalSpacer_8 = new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(verticalSpacer_8, 7, 1, 1, 1);

        label_8 = new QLabel(tab_2);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout_2->addWidget(label_8, 6, 0, 1, 1);

        label_9 = new QLabel(tab_2);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        gridLayout_2->addWidget(label_9, 1, 0, 1, 1);

        label_5 = new QLabel(tab_2);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout_2->addWidget(label_5, 5, 0, 1, 1);

        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));
        btnY_2 = new QPushButton(tab_2);
        btnY_2->setObjectName(QString::fromUtf8("btnY_2"));
        sizePolicy.setHeightForWidth(btnY_2->sizePolicy().hasHeightForWidth());
        btnY_2->setSizePolicy(sizePolicy);
        btnY_2->setFont(font);
        btnY_2->setCheckable(true);

        horizontalLayout_12->addWidget(btnY_2);

        inY_2 = new QLineEdit4AxisValue(tab_2);
        inY_2->setObjectName(QString::fromUtf8("inY_2"));
        sizePolicy1.setHeightForWidth(inY_2->sizePolicy().hasHeightForWidth());
        inY_2->setSizePolicy(sizePolicy1);
        inY_2->setFont(font);
        inY_2->setReadOnly(true);

        horizontalLayout_12->addWidget(inY_2);


        gridLayout_2->addLayout(horizontalLayout_12, 2, 1, 1, 1);


        horizontalLayout_14->addLayout(gridLayout_2);

        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        horizontalLayout_21 = new QHBoxLayout(tab_3);
        horizontalLayout_21->setObjectName(QString::fromUtf8("horizontalLayout_21"));
        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout_3->setContentsMargins(30, 30, 30, 30);
        horizontalLayout_18 = new QHBoxLayout();
        horizontalLayout_18->setObjectName(QString::fromUtf8("horizontalLayout_18"));
        btnX_3 = new QPushButton(tab_3);
        btnX_3->setObjectName(QString::fromUtf8("btnX_3"));
        sizePolicy.setHeightForWidth(btnX_3->sizePolicy().hasHeightForWidth());
        btnX_3->setSizePolicy(sizePolicy);
        btnX_3->setFont(font);
        btnX_3->setCheckable(true);

        horizontalLayout_18->addWidget(btnX_3);

        btnX_3_P = new QPushButton(tab_3);
        btnX_3_P->setObjectName(QString::fromUtf8("btnX_3_P"));
        btnX_3_P->setEnabled(false);
        sizePolicy.setHeightForWidth(btnX_3_P->sizePolicy().hasHeightForWidth());
        btnX_3_P->setSizePolicy(sizePolicy);
        QFont font1;
        font1.setPointSize(30);
        btnX_3_P->setFont(font1);
        btnX_3_P->setCheckable(true);

        horizontalLayout_18->addWidget(btnX_3_P);

        btnX_3_N = new QPushButton(tab_3);
        btnX_3_N->setObjectName(QString::fromUtf8("btnX_3_N"));
        btnX_3_N->setEnabled(false);
        sizePolicy.setHeightForWidth(btnX_3_N->sizePolicy().hasHeightForWidth());
        btnX_3_N->setSizePolicy(sizePolicy);
        btnX_3_N->setFont(font1);
        btnX_3_N->setCheckable(true);

        horizontalLayout_18->addWidget(btnX_3_N);

        horizontalLayout_18->setStretch(0, 1);
        horizontalLayout_18->setStretch(1, 6);
        horizontalLayout_18->setStretch(2, 6);

        gridLayout_3->addLayout(horizontalLayout_18, 1, 1, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(verticalSpacer_2, 5, 1, 1, 1);

        horizontalLayout_19 = new QHBoxLayout();
        horizontalLayout_19->setObjectName(QString::fromUtf8("horizontalLayout_19"));
        btnY_3 = new QPushButton(tab_3);
        btnY_3->setObjectName(QString::fromUtf8("btnY_3"));
        sizePolicy.setHeightForWidth(btnY_3->sizePolicy().hasHeightForWidth());
        btnY_3->setSizePolicy(sizePolicy);
        btnY_3->setFont(font);
        btnY_3->setCheckable(true);

        horizontalLayout_19->addWidget(btnY_3);

        btnY_3_P = new QPushButton(tab_3);
        btnY_3_P->setObjectName(QString::fromUtf8("btnY_3_P"));
        btnY_3_P->setEnabled(false);
        sizePolicy.setHeightForWidth(btnY_3_P->sizePolicy().hasHeightForWidth());
        btnY_3_P->setSizePolicy(sizePolicy);
        btnY_3_P->setFont(font1);
        btnY_3_P->setCheckable(true);

        horizontalLayout_19->addWidget(btnY_3_P);

        btnY_3_N = new QPushButton(tab_3);
        btnY_3_N->setObjectName(QString::fromUtf8("btnY_3_N"));
        btnY_3_N->setEnabled(false);
        sizePolicy.setHeightForWidth(btnY_3_N->sizePolicy().hasHeightForWidth());
        btnY_3_N->setSizePolicy(sizePolicy);
        btnY_3_N->setFont(font1);
        btnY_3_N->setCheckable(true);

        horizontalLayout_19->addWidget(btnY_3_N);

        horizontalLayout_19->setStretch(0, 1);
        horizontalLayout_19->setStretch(1, 6);
        horizontalLayout_19->setStretch(2, 6);

        gridLayout_3->addLayout(horizontalLayout_19, 2, 1, 1, 1);

        horizontalLayout_20 = new QHBoxLayout();
        horizontalLayout_20->setObjectName(QString::fromUtf8("horizontalLayout_20"));
        btnZ_3 = new QPushButton(tab_3);
        btnZ_3->setObjectName(QString::fromUtf8("btnZ_3"));
        sizePolicy.setHeightForWidth(btnZ_3->sizePolicy().hasHeightForWidth());
        btnZ_3->setSizePolicy(sizePolicy);
        btnZ_3->setFont(font);
        btnZ_3->setCheckable(true);

        horizontalLayout_20->addWidget(btnZ_3);

        btnZ_3_P = new QPushButton(tab_3);
        btnZ_3_P->setObjectName(QString::fromUtf8("btnZ_3_P"));
        btnZ_3_P->setEnabled(false);
        sizePolicy.setHeightForWidth(btnZ_3_P->sizePolicy().hasHeightForWidth());
        btnZ_3_P->setSizePolicy(sizePolicy);
        btnZ_3_P->setFont(font1);
        btnZ_3_P->setCheckable(true);

        horizontalLayout_20->addWidget(btnZ_3_P);

        btnZ_3_N = new QPushButton(tab_3);
        btnZ_3_N->setObjectName(QString::fromUtf8("btnZ_3_N"));
        btnZ_3_N->setEnabled(false);
        sizePolicy.setHeightForWidth(btnZ_3_N->sizePolicy().hasHeightForWidth());
        btnZ_3_N->setSizePolicy(sizePolicy);
        btnZ_3_N->setFont(font1);
        btnZ_3_N->setCheckable(true);

        horizontalLayout_20->addWidget(btnZ_3_N);

        horizontalLayout_20->setStretch(0, 1);
        horizontalLayout_20->setStretch(1, 6);
        horizontalLayout_20->setStretch(2, 6);

        gridLayout_3->addLayout(horizontalLayout_20, 3, 1, 1, 1);

        label_11 = new QLabel(tab_3);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        gridLayout_3->addWidget(label_11, 1, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(verticalSpacer, 0, 1, 1, 1);

        horizontalLayout_15 = new QHBoxLayout();
        horizontalLayout_15->setObjectName(QString::fromUtf8("horizontalLayout_15"));
        btnU_3 = new QPushButton(tab_3);
        btnU_3->setObjectName(QString::fromUtf8("btnU_3"));
        sizePolicy.setHeightForWidth(btnU_3->sizePolicy().hasHeightForWidth());
        btnU_3->setSizePolicy(sizePolicy);
        btnU_3->setFont(font);
        btnU_3->setCheckable(true);

        horizontalLayout_15->addWidget(btnU_3);

        btnU_3_P = new QPushButton(tab_3);
        btnU_3_P->setObjectName(QString::fromUtf8("btnU_3_P"));
        btnU_3_P->setEnabled(false);
        sizePolicy.setHeightForWidth(btnU_3_P->sizePolicy().hasHeightForWidth());
        btnU_3_P->setSizePolicy(sizePolicy);
        btnU_3_P->setFont(font1);
        btnU_3_P->setCheckable(true);

        horizontalLayout_15->addWidget(btnU_3_P);

        btnU_3_N = new QPushButton(tab_3);
        btnU_3_N->setObjectName(QString::fromUtf8("btnU_3_N"));
        btnU_3_N->setEnabled(false);
        sizePolicy.setHeightForWidth(btnU_3_N->sizePolicy().hasHeightForWidth());
        btnU_3_N->setSizePolicy(sizePolicy);
        btnU_3_N->setFont(font1);
        btnU_3_N->setCheckable(true);

        horizontalLayout_15->addWidget(btnU_3_N);

        horizontalLayout_15->setStretch(0, 1);
        horizontalLayout_15->setStretch(1, 6);
        horizontalLayout_15->setStretch(2, 6);

        gridLayout_3->addLayout(horizontalLayout_15, 4, 1, 1, 1);


        horizontalLayout_21->addLayout(gridLayout_3);

        tabWidget->addTab(tab_3, QString());

        horizontalLayout_3->addWidget(tabWidget);

        horizontalSpacer1 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer1);

        horizontalLayout_3->setStretch(0, 2);
        horizontalLayout_3->setStretch(1, 1);

        verticalLayout_2->addLayout(horizontalLayout_3);

        verticalSpacer2 = new QSpacerItem(20, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);

        verticalLayout_2->addItem(verticalSpacer2);


        horizontalLayout_9->addLayout(verticalLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_17 = new QHBoxLayout();
        horizontalLayout_17->setObjectName(QString::fromUtf8("horizontalLayout_17"));

        verticalLayout->addLayout(horizontalLayout_17);

        labelPix = new QLabel(WeiZhiYiDongForm);
        labelPix->setObjectName(QString::fromUtf8("labelPix"));

        verticalLayout->addWidget(labelPix);

        textBrowser = new QTextBrowser(WeiZhiYiDongForm);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));

        verticalLayout->addWidget(textBrowser);

        verticalLayout->setStretch(1, 2);
        verticalLayout->setStretch(2, 3);

        horizontalLayout_9->addLayout(verticalLayout);

        horizontalLayout_9->setStretch(0, 3);
        horizontalLayout_9->setStretch(1, 1);

        retranslateUi(WeiZhiYiDongForm);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(WeiZhiYiDongForm);
    } // setupUi

    void retranslateUi(QWidget *WeiZhiYiDongForm)
    {
        WeiZhiYiDongForm->setWindowTitle(QCoreApplication::translate("WeiZhiYiDongForm", "Form", nullptr));
        btnZ->setText(QCoreApplication::translate("WeiZhiYiDongForm", "Z", nullptr));
        label_2->setText(QCoreApplication::translate("WeiZhiYiDongForm", "\346\216\245\350\247\246\346\204\237\347\237\245", nullptr));
        label_3->setText(QCoreApplication::translate("WeiZhiYiDongForm", "\347\247\273\345\212\250\346\226\271\345\274\217", nullptr));
        inTouchSense->setText(QCoreApplication::translate("WeiZhiYiDongForm", "ON", nullptr));
        pushButton_8->setText(QCoreApplication::translate("WeiZhiYiDongForm", "OFF", nullptr));
        btnU->setText(QCoreApplication::translate("WeiZhiYiDongForm", "U", nullptr));
        label_4->setText(QCoreApplication::translate("WeiZhiYiDongForm", "\345\235\220\346\240\207\347\263\273", nullptr));
        inAbsolute->setText(QCoreApplication::translate("WeiZhiYiDongForm", "\347\273\235\345\257\271", nullptr));
        pushButton_6->setText(QCoreApplication::translate("WeiZhiYiDongForm", "\347\233\270\345\257\271", nullptr));
        label_1->setText(QCoreApplication::translate("WeiZhiYiDongForm", "\346\225\260\346\215\256\350\276\223\345\205\245", nullptr));
        btnY->setText(QCoreApplication::translate("WeiZhiYiDongForm", "Y", nullptr));
        btnX->setText(QCoreApplication::translate("WeiZhiYiDongForm", "X", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("WeiZhiYiDongForm", "\347\247\273\345\212\250", nullptr));
        btnZ_2->setText(QCoreApplication::translate("WeiZhiYiDongForm", "Z", nullptr));
        inTouchSense_2->setText(QCoreApplication::translate("WeiZhiYiDongForm", "ON", nullptr));
        pushButton_9->setText(QCoreApplication::translate("WeiZhiYiDongForm", "OFF", nullptr));
        btnX_2->setText(QCoreApplication::translate("WeiZhiYiDongForm", "X", nullptr));
        btnU_2->setText(QCoreApplication::translate("WeiZhiYiDongForm", "U", nullptr));
        label_8->setText(QCoreApplication::translate("WeiZhiYiDongForm", "\346\216\245\350\247\246\346\204\237\347\237\245", nullptr));
        label_9->setText(QCoreApplication::translate("WeiZhiYiDongForm", "\346\225\260\346\215\256\350\276\223\345\205\245", nullptr));
        label_5->setText(QCoreApplication::translate("WeiZhiYiDongForm", "\345\235\220\346\240\207\347\263\273", nullptr));
        btnY_2->setText(QCoreApplication::translate("WeiZhiYiDongForm", "Y", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("WeiZhiYiDongForm", "\345\215\212\347\250\213\347\247\273\345\212\250", nullptr));
        btnX_3->setText(QCoreApplication::translate("WeiZhiYiDongForm", "X", nullptr));
        btnX_3_P->setText(QCoreApplication::translate("WeiZhiYiDongForm", "+", nullptr));
        btnX_3_N->setText(QCoreApplication::translate("WeiZhiYiDongForm", "-", nullptr));
        btnY_3->setText(QCoreApplication::translate("WeiZhiYiDongForm", "Y", nullptr));
        btnY_3_P->setText(QCoreApplication::translate("WeiZhiYiDongForm", "+", nullptr));
        btnY_3_N->setText(QCoreApplication::translate("WeiZhiYiDongForm", "-", nullptr));
        btnZ_3->setText(QCoreApplication::translate("WeiZhiYiDongForm", "Z", nullptr));
        btnZ_3_P->setText(QCoreApplication::translate("WeiZhiYiDongForm", "+", nullptr));
        btnZ_3_N->setText(QCoreApplication::translate("WeiZhiYiDongForm", "-", nullptr));
        label_11->setText(QCoreApplication::translate("WeiZhiYiDongForm", "\346\225\260\346\215\256\350\276\223\345\205\245", nullptr));
        btnU_3->setText(QCoreApplication::translate("WeiZhiYiDongForm", "U", nullptr));
        btnU_3_P->setText(QCoreApplication::translate("WeiZhiYiDongForm", "+", nullptr));
        btnU_3_N->setText(QCoreApplication::translate("WeiZhiYiDongForm", "-", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QCoreApplication::translate("WeiZhiYiDongForm", "\351\231\220\344\275\215\347\247\273\345\212\250", nullptr));
        labelPix->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class WeiZhiYiDongForm: public Ui_WeiZhiYiDongForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WEIZHIYIDONGFORM_H
