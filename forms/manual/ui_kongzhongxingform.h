/********************************************************************************
** Form generated from reading UI file 'kongzhongxingform.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_KONGZHONGXINGFORM_H
#define UI_KONGZHONGXINGFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "widgets/qlineedit4axisvalue.h"

QT_BEGIN_NAMESPACE

class Ui_KongZhongXingForm
{
public:
    QHBoxLayout *horizontalLayout_9;
    QVBoxLayout *verticalLayout_2;
    QSpacerItem *verticalSpacer1;
    QHBoxLayout *horizontalLayout_22;
    QGridLayout *gridLayout;
    QLabel *label_3;
    QHBoxLayout *horizontalLayout_8;
    QPushButton *btnZ;
    QLineEdit4AxisValue *inZ;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout;
    QPushButton *inSetToZero;
    QPushButton *pushButton_6;
    QHBoxLayout *horizontalLayout_7;
    QPushButton *btnY;
    QLineEdit4AxisValue *inY;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *btnX;
    QLineEdit4AxisValue *inX;
    QLabel *label_1;
    QTableView *tableView;
    QLabel *label_4;
    QLineEdit4AxisValue *inBack;
    QLabel *label_5;
    QLineEdit4Keyboard *inCnt;
    QSpacerItem *horizontalSpacer11;
    QSpacerItem *verticalSpacer2;
    QVBoxLayout *verticalLayout;
    QLabel *labelPix;
    QTextBrowser *textBrowser;
    QButtonGroup *buttonGroup;

    void setupUi(QWidget *KongZhongXingForm)
    {
        if (KongZhongXingForm->objectName().isEmpty())
            KongZhongXingForm->setObjectName(QString::fromUtf8("KongZhongXingForm"));
        KongZhongXingForm->resize(872, 654);
        horizontalLayout_9 = new QHBoxLayout(KongZhongXingForm);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalSpacer1 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer1);

        horizontalLayout_22 = new QHBoxLayout();
        horizontalLayout_22->setObjectName(QString::fromUtf8("horizontalLayout_22"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(30, 30, 30, 30);
        label_3 = new QLabel(KongZhongXingForm);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 4, 0, 1, 1);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        btnZ = new QPushButton(KongZhongXingForm);
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

        inZ = new QLineEdit4AxisValue(KongZhongXingForm);
        inZ->setObjectName(QString::fromUtf8("inZ"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(inZ->sizePolicy().hasHeightForWidth());
        inZ->setSizePolicy(sizePolicy1);
        inZ->setFont(font);
        inZ->setReadOnly(true);

        horizontalLayout_8->addWidget(inZ);


        gridLayout->addLayout(horizontalLayout_8, 2, 1, 1, 1);

        label_2 = new QLabel(KongZhongXingForm);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 6, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        inSetToZero = new QPushButton(KongZhongXingForm);
        buttonGroup = new QButtonGroup(KongZhongXingForm);
        buttonGroup->setObjectName(QString::fromUtf8("buttonGroup"));
        buttonGroup->addButton(inSetToZero);
        inSetToZero->setObjectName(QString::fromUtf8("inSetToZero"));
        sizePolicy.setHeightForWidth(inSetToZero->sizePolicy().hasHeightForWidth());
        inSetToZero->setSizePolicy(sizePolicy);
        inSetToZero->setCheckable(true);
        inSetToZero->setChecked(true);

        horizontalLayout->addWidget(inSetToZero);

        pushButton_6 = new QPushButton(KongZhongXingForm);
        buttonGroup->addButton(pushButton_6);
        pushButton_6->setObjectName(QString::fromUtf8("pushButton_6"));
        sizePolicy.setHeightForWidth(pushButton_6->sizePolicy().hasHeightForWidth());
        pushButton_6->setSizePolicy(sizePolicy);
        pushButton_6->setCheckable(true);
        pushButton_6->setChecked(false);

        horizontalLayout->addWidget(pushButton_6);


        gridLayout->addLayout(horizontalLayout, 4, 1, 1, 1);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        btnY = new QPushButton(KongZhongXingForm);
        btnY->setObjectName(QString::fromUtf8("btnY"));
        sizePolicy.setHeightForWidth(btnY->sizePolicy().hasHeightForWidth());
        btnY->setSizePolicy(sizePolicy);
        btnY->setFont(font);
        btnY->setCheckable(true);

        horizontalLayout_7->addWidget(btnY);

        inY = new QLineEdit4AxisValue(KongZhongXingForm);
        inY->setObjectName(QString::fromUtf8("inY"));
        sizePolicy1.setHeightForWidth(inY->sizePolicy().hasHeightForWidth());
        inY->setSizePolicy(sizePolicy1);
        inY->setFont(font);
        inY->setReadOnly(true);

        horizontalLayout_7->addWidget(inY);


        gridLayout->addLayout(horizontalLayout_7, 1, 1, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        btnX = new QPushButton(KongZhongXingForm);
        btnX->setObjectName(QString::fromUtf8("btnX"));
        sizePolicy.setHeightForWidth(btnX->sizePolicy().hasHeightForWidth());
        btnX->setSizePolicy(sizePolicy);
        btnX->setFont(font);
        btnX->setCheckable(true);

        horizontalLayout_4->addWidget(btnX);

        inX = new QLineEdit4AxisValue(KongZhongXingForm);
        inX->setObjectName(QString::fromUtf8("inX"));
        sizePolicy1.setHeightForWidth(inX->sizePolicy().hasHeightForWidth());
        inX->setSizePolicy(sizePolicy1);
        inX->setFont(font);
        inX->setReadOnly(true);

        horizontalLayout_4->addWidget(inX);


        gridLayout->addLayout(horizontalLayout_4, 0, 1, 1, 1);

        label_1 = new QLabel(KongZhongXingForm);
        label_1->setObjectName(QString::fromUtf8("label_1"));

        gridLayout->addWidget(label_1, 0, 0, 1, 1);

        tableView = new QTableView(KongZhongXingForm);
        tableView->setObjectName(QString::fromUtf8("tableView"));
        tableView->setMinimumSize(QSize(0, 0));

        gridLayout->addWidget(tableView, 6, 1, 1, 1);

        label_4 = new QLabel(KongZhongXingForm);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 3, 0, 1, 1);

        inBack = new QLineEdit4AxisValue(KongZhongXingForm);
        inBack->setObjectName(QString::fromUtf8("inBack"));
        sizePolicy1.setHeightForWidth(inBack->sizePolicy().hasHeightForWidth());
        inBack->setSizePolicy(sizePolicy1);
        inBack->setFont(font);

        gridLayout->addWidget(inBack, 3, 1, 1, 1);

        label_5 = new QLabel(KongZhongXingForm);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout->addWidget(label_5, 5, 0, 1, 1);

        inCnt = new QLineEdit4Keyboard(KongZhongXingForm);
        inCnt->setObjectName(QString::fromUtf8("inCnt"));

        gridLayout->addWidget(inCnt, 5, 1, 1, 1);


        horizontalLayout_22->addLayout(gridLayout);

        horizontalSpacer11 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_22->addItem(horizontalSpacer11);

        horizontalLayout_22->setStretch(0, 2);
        horizontalLayout_22->setStretch(1, 1);

        verticalLayout_2->addLayout(horizontalLayout_22);

        verticalSpacer2 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer2);


        horizontalLayout_9->addLayout(verticalLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        labelPix = new QLabel(KongZhongXingForm);
        labelPix->setObjectName(QString::fromUtf8("labelPix"));

        verticalLayout->addWidget(labelPix);

        textBrowser = new QTextBrowser(KongZhongXingForm);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));

        verticalLayout->addWidget(textBrowser);

        verticalLayout->setStretch(0, 2);
        verticalLayout->setStretch(1, 3);

        horizontalLayout_9->addLayout(verticalLayout);

        horizontalLayout_9->setStretch(0, 3);
        horizontalLayout_9->setStretch(1, 1);

        retranslateUi(KongZhongXingForm);

        QMetaObject::connectSlotsByName(KongZhongXingForm);
    } // setupUi

    void retranslateUi(QWidget *KongZhongXingForm)
    {
        KongZhongXingForm->setWindowTitle(QCoreApplication::translate("KongZhongXingForm", "Form", nullptr));
        label_3->setText(QCoreApplication::translate("KongZhongXingForm", "ZB0SD", nullptr));
        btnZ->setText(QCoreApplication::translate("KongZhongXingForm", "Z", nullptr));
        inZ->setText(QString());
        label_2->setText(QCoreApplication::translate("KongZhongXingForm", "JG", nullptr));
        inSetToZero->setText(QCoreApplication::translate("KongZhongXingForm", "ON", nullptr));
        pushButton_6->setText(QCoreApplication::translate("KongZhongXingForm", "OFF", nullptr));
        btnY->setText(QCoreApplication::translate("KongZhongXingForm", "Y", nullptr));
        inY->setText(QString());
        btnX->setText(QCoreApplication::translate("KongZhongXingForm", "X", nullptr));
        inX->setText(QString());
        label_1->setText(QCoreApplication::translate("KongZhongXingForm", "KJL", nullptr));
        label_4->setText(QCoreApplication::translate("KongZhongXingForm", "JCGZHTZ", nullptr));
        inBack->setText(QCoreApplication::translate("KongZhongXingForm", "0.5", nullptr));
        label_5->setText(QCoreApplication::translate("KongZhongXingForm", "ChiShu", nullptr));
        inCnt->setText(QCoreApplication::translate("KongZhongXingForm", "1", nullptr));
        labelPix->setText(QString());
        textBrowser->setHtml(QString());
    } // retranslateUi

};

namespace Ui {
    class KongZhongXingForm: public Ui_KongZhongXingForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_KONGZHONGXINGFORM_H
