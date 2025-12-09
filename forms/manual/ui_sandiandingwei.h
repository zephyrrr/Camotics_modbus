/********************************************************************************
** Form generated from reading UI file 'sandiandingwei.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SANDIANDINGWEI_H
#define UI_SANDIANDINGWEI_H

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

class Ui_SanDianDingWeiForm
{
public:
    QHBoxLayout *horizontalLayout_9;
    QVBoxLayout *verticalLayout_2;
    QSpacerItem *verticalSpacer1;
    QHBoxLayout *horizontalLayout_22;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout_11;
    QPushButton *inPositionOuter;
    QPushButton *pushButton_3;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *inSetToZTouch;
    QPushButton *pushButton;
    QLabel *label_7;
    QLineEdit4Keyboard *inCnt;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *btnX;
    QLineEdit4AxisValue *inX;
    QLineEdit4AxisValue *inBack;
    QLabel *label_3;
    QLabel *label_2;
    QLabel *label_6;
    QTableView *tableView;
    QHBoxLayout *horizontalLayout;
    QPushButton *inSetToZero;
    QPushButton *pushButton_6;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_6;
    QPushButton *btnA1;
    QLineEdit4AxisValue *inA1;
    QHBoxLayout *horizontalLayout_6_2;
    QPushButton *btnA2;
    QLineEdit4AxisValue *inA2;
    QHBoxLayout *horizontalLayout_6_3;
    QPushButton *btnA3;
    QLineEdit4AxisValue *inA3;
    QLabel *label_1;
    QLabel *label_4;
    QLabel *label_9;
    QLabel *label_8;
    QHBoxLayout *horizontalLayout_8;
    QPushButton *btnZ;
    QLineEdit4AxisValue *inZ;
    QSpacerItem *horizontalSpacer11;
    QSpacerItem *verticalSpacer2;
    QVBoxLayout *verticalLayout;
    QLabel *labelPix;
    QTextBrowser *textBrowser;
    QButtonGroup *buttonGroup_2;
    QButtonGroup *buttonGroup_3;
    QButtonGroup *buttonGroup;

    void setupUi(QWidget *SanDianDingWeiForm)
    {
        if (SanDianDingWeiForm->objectName().isEmpty())
            SanDianDingWeiForm->setObjectName(QString::fromUtf8("SanDianDingWeiForm"));
        SanDianDingWeiForm->resize(872, 751);
        horizontalLayout_9 = new QHBoxLayout(SanDianDingWeiForm);
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
        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        inPositionOuter = new QPushButton(SanDianDingWeiForm);
        buttonGroup_3 = new QButtonGroup(SanDianDingWeiForm);
        buttonGroup_3->setObjectName(QString::fromUtf8("buttonGroup_3"));
        buttonGroup_3->addButton(inPositionOuter);
        inPositionOuter->setObjectName(QString::fromUtf8("inPositionOuter"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(inPositionOuter->sizePolicy().hasHeightForWidth());
        inPositionOuter->setSizePolicy(sizePolicy);
        inPositionOuter->setCheckable(true);
        inPositionOuter->setChecked(true);

        horizontalLayout_11->addWidget(inPositionOuter);

        pushButton_3 = new QPushButton(SanDianDingWeiForm);
        buttonGroup_3->addButton(pushButton_3);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        sizePolicy.setHeightForWidth(pushButton_3->sizePolicy().hasHeightForWidth());
        pushButton_3->setSizePolicy(sizePolicy);
        pushButton_3->setCheckable(true);

        horizontalLayout_11->addWidget(pushButton_3);


        gridLayout->addLayout(horizontalLayout_11, 6, 1, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        inSetToZTouch = new QPushButton(SanDianDingWeiForm);
        buttonGroup_2 = new QButtonGroup(SanDianDingWeiForm);
        buttonGroup_2->setObjectName(QString::fromUtf8("buttonGroup_2"));
        buttonGroup_2->addButton(inSetToZTouch);
        inSetToZTouch->setObjectName(QString::fromUtf8("inSetToZTouch"));
        sizePolicy.setHeightForWidth(inSetToZTouch->sizePolicy().hasHeightForWidth());
        inSetToZTouch->setSizePolicy(sizePolicy);
        inSetToZTouch->setCheckable(true);
        inSetToZTouch->setChecked(true);

        horizontalLayout_3->addWidget(inSetToZTouch);

        pushButton = new QPushButton(SanDianDingWeiForm);
        buttonGroup_2->addButton(pushButton);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        sizePolicy.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy);
        pushButton->setCheckable(true);

        horizontalLayout_3->addWidget(pushButton);


        gridLayout->addLayout(horizontalLayout_3, 3, 1, 1, 1);

        label_7 = new QLabel(SanDianDingWeiForm);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout->addWidget(label_7, 5, 0, 1, 1);

        inCnt = new QLineEdit4Keyboard(SanDianDingWeiForm);
        inCnt->setObjectName(QString::fromUtf8("inCnt"));

        gridLayout->addWidget(inCnt, 7, 1, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        btnX = new QPushButton(SanDianDingWeiForm);
        btnX->setObjectName(QString::fromUtf8("btnX"));
        sizePolicy.setHeightForWidth(btnX->sizePolicy().hasHeightForWidth());
        btnX->setSizePolicy(sizePolicy);
        QFont font;
        font.setPointSize(20);
        btnX->setFont(font);
        btnX->setCheckable(true);

        horizontalLayout_4->addWidget(btnX);

        inX = new QLineEdit4AxisValue(SanDianDingWeiForm);
        inX->setObjectName(QString::fromUtf8("inX"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(inX->sizePolicy().hasHeightForWidth());
        inX->setSizePolicy(sizePolicy1);
        inX->setFont(font);
        inX->setReadOnly(true);

        horizontalLayout_4->addWidget(inX);


        gridLayout->addLayout(horizontalLayout_4, 0, 1, 1, 1);

        inBack = new QLineEdit4AxisValue(SanDianDingWeiForm);
        inBack->setObjectName(QString::fromUtf8("inBack"));
        sizePolicy1.setHeightForWidth(inBack->sizePolicy().hasHeightForWidth());
        inBack->setSizePolicy(sizePolicy1);
        inBack->setFont(font);

        gridLayout->addWidget(inBack, 2, 1, 1, 1);

        label_3 = new QLabel(SanDianDingWeiForm);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 4, 0, 1, 1);

        label_2 = new QLabel(SanDianDingWeiForm);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 8, 0, 1, 1);

        label_6 = new QLabel(SanDianDingWeiForm);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout->addWidget(label_6, 3, 0, 1, 1);

        tableView = new QTableView(SanDianDingWeiForm);
        tableView->setObjectName(QString::fromUtf8("tableView"));
        tableView->setMinimumSize(QSize(0, 0));

        gridLayout->addWidget(tableView, 8, 1, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        inSetToZero = new QPushButton(SanDianDingWeiForm);
        buttonGroup = new QButtonGroup(SanDianDingWeiForm);
        buttonGroup->setObjectName(QString::fromUtf8("buttonGroup"));
        buttonGroup->addButton(inSetToZero);
        inSetToZero->setObjectName(QString::fromUtf8("inSetToZero"));
        sizePolicy.setHeightForWidth(inSetToZero->sizePolicy().hasHeightForWidth());
        inSetToZero->setSizePolicy(sizePolicy);
        inSetToZero->setCheckable(true);
        inSetToZero->setChecked(true);

        horizontalLayout->addWidget(inSetToZero);

        pushButton_6 = new QPushButton(SanDianDingWeiForm);
        buttonGroup->addButton(pushButton_6);
        pushButton_6->setObjectName(QString::fromUtf8("pushButton_6"));
        sizePolicy.setHeightForWidth(pushButton_6->sizePolicy().hasHeightForWidth());
        pushButton_6->setSizePolicy(sizePolicy);
        pushButton_6->setCheckable(true);
        pushButton_6->setChecked(false);

        horizontalLayout->addWidget(pushButton_6);


        gridLayout->addLayout(horizontalLayout, 4, 1, 1, 1);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        btnA1 = new QPushButton(SanDianDingWeiForm);
        btnA1->setObjectName(QString::fromUtf8("btnA1"));
        sizePolicy.setHeightForWidth(btnA1->sizePolicy().hasHeightForWidth());
        btnA1->setSizePolicy(sizePolicy);
        btnA1->setCheckable(true);

        horizontalLayout_6->addWidget(btnA1);

        inA1 = new QLineEdit4AxisValue(SanDianDingWeiForm);
        inA1->setObjectName(QString::fromUtf8("inA1"));
        sizePolicy1.setHeightForWidth(inA1->sizePolicy().hasHeightForWidth());
        inA1->setSizePolicy(sizePolicy1);
        inA1->setReadOnly(true);

        horizontalLayout_6->addWidget(inA1);


        verticalLayout_3->addLayout(horizontalLayout_6);

        horizontalLayout_6_2 = new QHBoxLayout();
        horizontalLayout_6_2->setObjectName(QString::fromUtf8("horizontalLayout_6_2"));
        btnA2 = new QPushButton(SanDianDingWeiForm);
        btnA2->setObjectName(QString::fromUtf8("btnA2"));
        sizePolicy.setHeightForWidth(btnA2->sizePolicy().hasHeightForWidth());
        btnA2->setSizePolicy(sizePolicy);
        btnA2->setCheckable(true);

        horizontalLayout_6_2->addWidget(btnA2);

        inA2 = new QLineEdit4AxisValue(SanDianDingWeiForm);
        inA2->setObjectName(QString::fromUtf8("inA2"));
        sizePolicy1.setHeightForWidth(inA2->sizePolicy().hasHeightForWidth());
        inA2->setSizePolicy(sizePolicy1);
        inA2->setReadOnly(true);

        horizontalLayout_6_2->addWidget(inA2);


        verticalLayout_3->addLayout(horizontalLayout_6_2);

        horizontalLayout_6_3 = new QHBoxLayout();
        horizontalLayout_6_3->setObjectName(QString::fromUtf8("horizontalLayout_6_3"));
        btnA3 = new QPushButton(SanDianDingWeiForm);
        btnA3->setObjectName(QString::fromUtf8("btnA3"));
        sizePolicy.setHeightForWidth(btnA3->sizePolicy().hasHeightForWidth());
        btnA3->setSizePolicy(sizePolicy);
        btnA3->setCheckable(true);

        horizontalLayout_6_3->addWidget(btnA3);

        inA3 = new QLineEdit4AxisValue(SanDianDingWeiForm);
        inA3->setObjectName(QString::fromUtf8("inA3"));
        sizePolicy1.setHeightForWidth(inA3->sizePolicy().hasHeightForWidth());
        inA3->setSizePolicy(sizePolicy1);
        inA3->setReadOnly(true);

        horizontalLayout_6_3->addWidget(inA3);


        verticalLayout_3->addLayout(horizontalLayout_6_3);


        gridLayout->addLayout(verticalLayout_3, 5, 1, 1, 1);

        label_1 = new QLabel(SanDianDingWeiForm);
        label_1->setObjectName(QString::fromUtf8("label_1"));

        gridLayout->addWidget(label_1, 0, 0, 1, 1);

        label_4 = new QLabel(SanDianDingWeiForm);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 2, 0, 1, 1);

        label_9 = new QLabel(SanDianDingWeiForm);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        gridLayout->addWidget(label_9, 7, 0, 1, 1);

        label_8 = new QLabel(SanDianDingWeiForm);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout->addWidget(label_8, 6, 0, 1, 1);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        btnZ = new QPushButton(SanDianDingWeiForm);
        btnZ->setObjectName(QString::fromUtf8("btnZ"));
        sizePolicy.setHeightForWidth(btnZ->sizePolicy().hasHeightForWidth());
        btnZ->setSizePolicy(sizePolicy);
        btnZ->setFont(font);
        btnZ->setCheckable(true);

        horizontalLayout_8->addWidget(btnZ);

        inZ = new QLineEdit4AxisValue(SanDianDingWeiForm);
        inZ->setObjectName(QString::fromUtf8("inZ"));
        sizePolicy1.setHeightForWidth(inZ->sizePolicy().hasHeightForWidth());
        inZ->setSizePolicy(sizePolicy1);
        inZ->setFont(font);
        inZ->setReadOnly(true);

        horizontalLayout_8->addWidget(inZ);


        gridLayout->addLayout(horizontalLayout_8, 1, 1, 1, 1);


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
        labelPix = new QLabel(SanDianDingWeiForm);
        labelPix->setObjectName(QString::fromUtf8("labelPix"));

        verticalLayout->addWidget(labelPix);

        textBrowser = new QTextBrowser(SanDianDingWeiForm);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));

        verticalLayout->addWidget(textBrowser);

        verticalLayout->setStretch(0, 2);
        verticalLayout->setStretch(1, 3);

        horizontalLayout_9->addLayout(verticalLayout);

        horizontalLayout_9->setStretch(0, 3);
        horizontalLayout_9->setStretch(1, 1);

        retranslateUi(SanDianDingWeiForm);

        QMetaObject::connectSlotsByName(SanDianDingWeiForm);
    } // setupUi

    void retranslateUi(QWidget *SanDianDingWeiForm)
    {
        SanDianDingWeiForm->setWindowTitle(QCoreApplication::translate("SanDianDingWeiForm", "Form", nullptr));
        inPositionOuter->setText(QCoreApplication::translate("SanDianDingWeiForm", "WJ", nullptr));
        pushButton_3->setText(QCoreApplication::translate("SanDianDingWeiForm", "NJ", nullptr));
        inSetToZTouch->setText(QCoreApplication::translate("SanDianDingWeiForm", "ON", nullptr));
        pushButton->setText(QCoreApplication::translate("SanDianDingWeiForm", "OFF", nullptr));
        label_7->setText(QCoreApplication::translate("SanDianDingWeiForm", "JD", nullptr));
        inCnt->setText(QCoreApplication::translate("SanDianDingWeiForm", "1", nullptr));
        btnX->setText(QCoreApplication::translate("SanDianDingWeiForm", "R", nullptr));
        inX->setText(QString());
        inBack->setText(QCoreApplication::translate("SanDianDingWeiForm", "0.5", nullptr));
        label_3->setText(QCoreApplication::translate("SanDianDingWeiForm", "ZB0SD", nullptr));
        label_2->setText(QCoreApplication::translate("SanDianDingWeiForm", "JG", nullptr));
        label_6->setText(QCoreApplication::translate("SanDianDingWeiForm", "ZZJCGZDZ", nullptr));
        inSetToZero->setText(QCoreApplication::translate("SanDianDingWeiForm", "ON", nullptr));
        pushButton_6->setText(QCoreApplication::translate("SanDianDingWeiForm", "OFF", nullptr));
        btnA1->setText(QCoreApplication::translate("SanDianDingWeiForm", "1", nullptr));
        btnA2->setText(QCoreApplication::translate("SanDianDingWeiForm", "2", nullptr));
        btnA3->setText(QCoreApplication::translate("SanDianDingWeiForm", "3", nullptr));
        label_1->setText(QCoreApplication::translate("SanDianDingWeiForm", "KJL", nullptr));
        label_4->setText(QCoreApplication::translate("SanDianDingWeiForm", "JCGZHTZ", nullptr));
        label_9->setText(QCoreApplication::translate("SanDianDingWeiForm", "CS", nullptr));
        label_8->setText(QCoreApplication::translate("SanDianDingWeiForm", "CDWZ", nullptr));
        btnZ->setText(QCoreApplication::translate("SanDianDingWeiForm", "Z", nullptr));
        inZ->setText(QString());
        labelPix->setText(QString());
        textBrowser->setHtml(QString());
    } // retranslateUi

};

namespace Ui {
    class SanDianDingWeiForm: public Ui_SanDianDingWeiForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SANDIANDINGWEI_H
