/********************************************************************************
** Form generated from reading UI file 'zhuzhongxingform.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ZHUZHONGXINGFORM_H
#define UI_ZHUZHONGXINGFORM_H

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

class Ui_ZhuZhongXingForm
{
public:
    QHBoxLayout *horizontalLayout_9;
    QVBoxLayout *verticalLayout_2;
    QSpacerItem *verticalSpacer1;
    QHBoxLayout *horizontalLayout_22;
    QGridLayout *gridLayout;
    QLabel *label_1;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *btnX;
    QLineEdit4AxisValue *inX;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *btnY;
    QLineEdit4AxisValue *inY;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *btnZ;
    QLineEdit4AxisValue *inZ;
    QLabel *label_4;
    QLineEdit4AxisValue *inBack;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout_5;
    QPushButton *inSetToZTouch;
    QPushButton *pushButton;
    QLabel *label_3;
    QHBoxLayout *horizontalLayout_6;
    QPushButton *inSetToZero;
    QPushButton *pushButton_6;
    QLabel *label_7;
    QLineEdit4Keyboard *inCnt;
    QLabel *label_6;
    QTableView *tableView;
    QSpacerItem *horizontalSpacer11;
    QSpacerItem *verticalSpacer2;
    QVBoxLayout *verticalLayout;
    QLabel *labelPix;
    QHBoxLayout *horizontalLayout;
    QTextBrowser *textBrowser;
    QButtonGroup *buttonGroup_2;
    QButtonGroup *buttonGroup;

    void setupUi(QWidget *ZhuZhongXingForm)
    {
        if (ZhuZhongXingForm->objectName().isEmpty())
            ZhuZhongXingForm->setObjectName(QString::fromUtf8("ZhuZhongXingForm"));
        ZhuZhongXingForm->resize(879, 651);
        horizontalLayout_9 = new QHBoxLayout(ZhuZhongXingForm);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalSpacer1 = new QSpacerItem(20, 20, QSizePolicy::Policy::Fixed, QSizePolicy::Minimum);

        verticalLayout_2->addItem(verticalSpacer1);

        horizontalLayout_22 = new QHBoxLayout();
        horizontalLayout_22->setObjectName(QString::fromUtf8("horizontalLayout_22"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(30, 30, 30, 30);
        label_1 = new QLabel(ZhuZhongXingForm);
        label_1->setObjectName(QString::fromUtf8("label_1"));

        gridLayout->addWidget(label_1, 0, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        btnX = new QPushButton(ZhuZhongXingForm);
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

        horizontalLayout_2->addWidget(btnX);

        inX = new QLineEdit4AxisValue(ZhuZhongXingForm);
        inX->setObjectName(QString::fromUtf8("inX"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(inX->sizePolicy().hasHeightForWidth());
        inX->setSizePolicy(sizePolicy1);
        inX->setFont(font);
        inX->setReadOnly(true);

        horizontalLayout_2->addWidget(inX);


        gridLayout->addLayout(horizontalLayout_2, 0, 1, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        btnY = new QPushButton(ZhuZhongXingForm);
        btnY->setObjectName(QString::fromUtf8("btnY"));
        sizePolicy.setHeightForWidth(btnY->sizePolicy().hasHeightForWidth());
        btnY->setSizePolicy(sizePolicy);
        btnY->setFont(font);
        btnY->setCheckable(true);

        horizontalLayout_3->addWidget(btnY);

        inY = new QLineEdit4AxisValue(ZhuZhongXingForm);
        inY->setObjectName(QString::fromUtf8("inY"));
        sizePolicy1.setHeightForWidth(inY->sizePolicy().hasHeightForWidth());
        inY->setSizePolicy(sizePolicy1);
        inY->setFont(font);
        inY->setReadOnly(true);

        horizontalLayout_3->addWidget(inY);


        gridLayout->addLayout(horizontalLayout_3, 1, 1, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        btnZ = new QPushButton(ZhuZhongXingForm);
        btnZ->setObjectName(QString::fromUtf8("btnZ"));
        sizePolicy.setHeightForWidth(btnZ->sizePolicy().hasHeightForWidth());
        btnZ->setSizePolicy(sizePolicy);
        btnZ->setFont(font);
        btnZ->setCheckable(true);

        horizontalLayout_4->addWidget(btnZ);

        inZ = new QLineEdit4AxisValue(ZhuZhongXingForm);
        inZ->setObjectName(QString::fromUtf8("inZ"));
        sizePolicy1.setHeightForWidth(inZ->sizePolicy().hasHeightForWidth());
        inZ->setSizePolicy(sizePolicy1);
        inZ->setFont(font);
        inZ->setReadOnly(true);

        horizontalLayout_4->addWidget(inZ);


        gridLayout->addLayout(horizontalLayout_4, 2, 1, 1, 1);

        label_4 = new QLabel(ZhuZhongXingForm);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 3, 0, 1, 1);

        inBack = new QLineEdit4AxisValue(ZhuZhongXingForm);
        inBack->setObjectName(QString::fromUtf8("inBack"));
        sizePolicy1.setHeightForWidth(inBack->sizePolicy().hasHeightForWidth());
        inBack->setSizePolicy(sizePolicy1);
        inBack->setFont(font);

        gridLayout->addWidget(inBack, 3, 1, 1, 1);

        label_2 = new QLabel(ZhuZhongXingForm);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 4, 0, 1, 1);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        inSetToZTouch = new QPushButton(ZhuZhongXingForm);
        buttonGroup_2 = new QButtonGroup(ZhuZhongXingForm);
        buttonGroup_2->setObjectName(QString::fromUtf8("buttonGroup_2"));
        buttonGroup_2->addButton(inSetToZTouch);
        inSetToZTouch->setObjectName(QString::fromUtf8("inSetToZTouch"));
        sizePolicy.setHeightForWidth(inSetToZTouch->sizePolicy().hasHeightForWidth());
        inSetToZTouch->setSizePolicy(sizePolicy);
        inSetToZTouch->setCheckable(true);
        inSetToZTouch->setChecked(true);

        horizontalLayout_5->addWidget(inSetToZTouch);

        pushButton = new QPushButton(ZhuZhongXingForm);
        buttonGroup_2->addButton(pushButton);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        sizePolicy.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy);
        pushButton->setCheckable(true);

        horizontalLayout_5->addWidget(pushButton);


        gridLayout->addLayout(horizontalLayout_5, 4, 1, 1, 1);

        label_3 = new QLabel(ZhuZhongXingForm);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 5, 0, 1, 1);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        inSetToZero = new QPushButton(ZhuZhongXingForm);
        buttonGroup = new QButtonGroup(ZhuZhongXingForm);
        buttonGroup->setObjectName(QString::fromUtf8("buttonGroup"));
        buttonGroup->addButton(inSetToZero);
        inSetToZero->setObjectName(QString::fromUtf8("inSetToZero"));
        sizePolicy.setHeightForWidth(inSetToZero->sizePolicy().hasHeightForWidth());
        inSetToZero->setSizePolicy(sizePolicy);
        inSetToZero->setCheckable(true);
        inSetToZero->setChecked(true);

        horizontalLayout_6->addWidget(inSetToZero);

        pushButton_6 = new QPushButton(ZhuZhongXingForm);
        buttonGroup->addButton(pushButton_6);
        pushButton_6->setObjectName(QString::fromUtf8("pushButton_6"));
        sizePolicy.setHeightForWidth(pushButton_6->sizePolicy().hasHeightForWidth());
        pushButton_6->setSizePolicy(sizePolicy);
        pushButton_6->setCheckable(true);
        pushButton_6->setChecked(false);

        horizontalLayout_6->addWidget(pushButton_6);


        gridLayout->addLayout(horizontalLayout_6, 5, 1, 1, 1);

        label_7 = new QLabel(ZhuZhongXingForm);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout->addWidget(label_7, 6, 0, 1, 1);

        inCnt = new QLineEdit4Keyboard(ZhuZhongXingForm);
        inCnt->setObjectName(QString::fromUtf8("inCnt"));

        gridLayout->addWidget(inCnt, 6, 1, 1, 1);

        label_6 = new QLabel(ZhuZhongXingForm);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout->addWidget(label_6, 7, 0, 1, 1);

        tableView = new QTableView(ZhuZhongXingForm);
        tableView->setObjectName(QString::fromUtf8("tableView"));
        tableView->setMinimumSize(QSize(0, 0));

        gridLayout->addWidget(tableView, 7, 1, 1, 1);


        horizontalLayout_22->addLayout(gridLayout);

        horizontalSpacer11 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_22->addItem(horizontalSpacer11);

        horizontalLayout_22->setStretch(0, 2);
        horizontalLayout_22->setStretch(1, 1);

        verticalLayout_2->addLayout(horizontalLayout_22);

        verticalSpacer2 = new QSpacerItem(20, 13, QSizePolicy::Expanding, QSizePolicy::Minimum);

        verticalLayout_2->addItem(verticalSpacer2);


        horizontalLayout_9->addLayout(verticalLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        labelPix = new QLabel(ZhuZhongXingForm);
        labelPix->setObjectName(QString::fromUtf8("labelPix"));

        verticalLayout->addWidget(labelPix);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));

        verticalLayout->addLayout(horizontalLayout);

        textBrowser = new QTextBrowser(ZhuZhongXingForm);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));

        verticalLayout->addWidget(textBrowser);

        verticalLayout->setStretch(0, 2);
        verticalLayout->setStretch(2, 3);

        horizontalLayout_9->addLayout(verticalLayout);

        horizontalLayout_9->setStretch(0, 3);
        horizontalLayout_9->setStretch(1, 1);

        retranslateUi(ZhuZhongXingForm);

        QMetaObject::connectSlotsByName(ZhuZhongXingForm);
    } // setupUi

    void retranslateUi(QWidget *ZhuZhongXingForm)
    {
        ZhuZhongXingForm->setWindowTitle(QCoreApplication::translate("ZhuZhongXingForm", "Form", nullptr));
        label_1->setText(QCoreApplication::translate("ZhuZhongXingForm", "KJL", nullptr));
        btnX->setText(QCoreApplication::translate("ZhuZhongXingForm", "X", nullptr));
        inX->setText(QString());
        btnY->setText(QCoreApplication::translate("ZhuZhongXingForm", "Y", nullptr));
        inY->setText(QString());
        btnZ->setText(QCoreApplication::translate("ZhuZhongXingForm", "Z", nullptr));
        inZ->setText(QString());
        label_4->setText(QCoreApplication::translate("ZhuZhongXingForm", "JCGZHTZ", nullptr));
        inBack->setText(QCoreApplication::translate("ZhuZhongXingForm", "0.5", nullptr));
        label_2->setText(QCoreApplication::translate("ZhuZhongXingForm", "ZZJCGZDZ", nullptr));
        inSetToZTouch->setText(QCoreApplication::translate("ZhuZhongXingForm", "ON", nullptr));
        pushButton->setText(QCoreApplication::translate("ZhuZhongXingForm", "OFF", nullptr));
        label_3->setText(QCoreApplication::translate("ZhuZhongXingForm", "ZB0SD", nullptr));
        inSetToZero->setText(QCoreApplication::translate("ZhuZhongXingForm", "ON", nullptr));
        pushButton_6->setText(QCoreApplication::translate("ZhuZhongXingForm", "OFF", nullptr));
        label_7->setText(QCoreApplication::translate("ZhuZhongXingForm", "CS", nullptr));
        inCnt->setText(QCoreApplication::translate("ZhuZhongXingForm", "1", nullptr));
        label_6->setText(QCoreApplication::translate("ZhuZhongXingForm", "JG", nullptr));
        labelPix->setText(QString());
        textBrowser->setHtml(QString());
    } // retranslateUi

};

namespace Ui {
    class ZhuZhongXingForm: public Ui_ZhuZhongXingForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ZHUZHONGXINGFORM_H
