/********************************************************************************
** Form generated from reading UI file 'jiaodingweiform.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_JIAODINGWEIFORM_H
#define UI_JIAODINGWEIFORM_H

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

class Ui_JiaoDingWeiForm
{
public:
    QHBoxLayout *horizontalLayout_9;
    QVBoxLayout *verticalLayout_2;
    QSpacerItem *verticalSpacer1;
    QHBoxLayout *horizontalLayout_22;
    QGridLayout *gridLayout;
    QLabel *label_4;
    QLabel *label_7;
    QHBoxLayout *horizontalLayout_5;
    QPushButton *inBtnType1;
    QPushButton *inBtnType2;
    QPushButton *inBtnType3;
    QPushButton *inBtnType4;
    QLineEdit4AxisValue *inBack;
    QTableView *tableView;
    QLabel *label_1;
    QLabel *label_3;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *btnX;
    QLineEdit4AxisValue *inX;
    QLabel *label_5;
    QHBoxLayout *horizontalLayout_8;
    QPushButton *btnZ;
    QLineEdit4AxisValue *inZ;
    QHBoxLayout *horizontalLayout;
    QPushButton *inSetToZero;
    QPushButton *pushButton_6;
    QHBoxLayout *horizontalLayout_7;
    QPushButton *btnY;
    QLineEdit4AxisValue *inY;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *inSetToZTouch;
    QPushButton *pushButton;
    QLabel *label_6;
    QLineEdit4Keyboard *inCnt;
    QSpacerItem *horizontalSpacer11;
    QSpacerItem *verticalSpacer2;
    QVBoxLayout *verticalLayout;
    QLabel *labelPix;
    QTextBrowser *textBrowser;
    QButtonGroup *buttonGroup_2;
    QButtonGroup *buttonGroup;
    QButtonGroup *buttonGroupJiaoWeiZhi;

    void setupUi(QWidget *JiaoDingWeiForm)
    {
        if (JiaoDingWeiForm->objectName().isEmpty())
            JiaoDingWeiForm->setObjectName(QString::fromUtf8("JiaoDingWeiForm"));
        JiaoDingWeiForm->resize(872, 706);
        horizontalLayout_9 = new QHBoxLayout(JiaoDingWeiForm);
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
        label_4 = new QLabel(JiaoDingWeiForm);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 4, 0, 1, 1);

        label_7 = new QLabel(JiaoDingWeiForm);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout->addWidget(label_7, 0, 0, 1, 1);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        inBtnType1 = new QPushButton(JiaoDingWeiForm);
        buttonGroupJiaoWeiZhi = new QButtonGroup(JiaoDingWeiForm);
        buttonGroupJiaoWeiZhi->setObjectName(QString::fromUtf8("buttonGroupJiaoWeiZhi"));
        buttonGroupJiaoWeiZhi->addButton(inBtnType1);
        inBtnType1->setObjectName(QString::fromUtf8("inBtnType1"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(inBtnType1->sizePolicy().hasHeightForWidth());
        inBtnType1->setSizePolicy(sizePolicy);
        inBtnType1->setCheckable(true);
        inBtnType1->setChecked(true);

        horizontalLayout_5->addWidget(inBtnType1);

        inBtnType2 = new QPushButton(JiaoDingWeiForm);
        buttonGroupJiaoWeiZhi->addButton(inBtnType2);
        inBtnType2->setObjectName(QString::fromUtf8("inBtnType2"));
        sizePolicy.setHeightForWidth(inBtnType2->sizePolicy().hasHeightForWidth());
        inBtnType2->setSizePolicy(sizePolicy);
        inBtnType2->setCheckable(true);

        horizontalLayout_5->addWidget(inBtnType2);

        inBtnType3 = new QPushButton(JiaoDingWeiForm);
        buttonGroupJiaoWeiZhi->addButton(inBtnType3);
        inBtnType3->setObjectName(QString::fromUtf8("inBtnType3"));
        sizePolicy.setHeightForWidth(inBtnType3->sizePolicy().hasHeightForWidth());
        inBtnType3->setSizePolicy(sizePolicy);
        inBtnType3->setCheckable(true);

        horizontalLayout_5->addWidget(inBtnType3);

        inBtnType4 = new QPushButton(JiaoDingWeiForm);
        buttonGroupJiaoWeiZhi->addButton(inBtnType4);
        inBtnType4->setObjectName(QString::fromUtf8("inBtnType4"));
        sizePolicy.setHeightForWidth(inBtnType4->sizePolicy().hasHeightForWidth());
        inBtnType4->setSizePolicy(sizePolicy);
        inBtnType4->setCheckable(true);

        horizontalLayout_5->addWidget(inBtnType4);


        gridLayout->addLayout(horizontalLayout_5, 0, 1, 1, 1);

        inBack = new QLineEdit4AxisValue(JiaoDingWeiForm);
        inBack->setObjectName(QString::fromUtf8("inBack"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(inBack->sizePolicy().hasHeightForWidth());
        inBack->setSizePolicy(sizePolicy1);
        QFont font;
        font.setPointSize(20);
        inBack->setFont(font);

        gridLayout->addWidget(inBack, 4, 1, 1, 1);

        tableView = new QTableView(JiaoDingWeiForm);
        tableView->setObjectName(QString::fromUtf8("tableView"));
        tableView->setMinimumSize(QSize(0, 0));

        gridLayout->addWidget(tableView, 8, 1, 1, 1);

        label_1 = new QLabel(JiaoDingWeiForm);
        label_1->setObjectName(QString::fromUtf8("label_1"));

        gridLayout->addWidget(label_1, 1, 0, 1, 1);

        label_3 = new QLabel(JiaoDingWeiForm);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 6, 0, 1, 1);

        label_2 = new QLabel(JiaoDingWeiForm);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 5, 0, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        btnX = new QPushButton(JiaoDingWeiForm);
        btnX->setObjectName(QString::fromUtf8("btnX"));
        sizePolicy.setHeightForWidth(btnX->sizePolicy().hasHeightForWidth());
        btnX->setSizePolicy(sizePolicy);
        btnX->setFont(font);
        btnX->setCheckable(true);

        horizontalLayout_4->addWidget(btnX);

        inX = new QLineEdit4AxisValue(JiaoDingWeiForm);
        inX->setObjectName(QString::fromUtf8("inX"));
        sizePolicy1.setHeightForWidth(inX->sizePolicy().hasHeightForWidth());
        inX->setSizePolicy(sizePolicy1);
        inX->setFont(font);
        inX->setReadOnly(true);

        horizontalLayout_4->addWidget(inX);


        gridLayout->addLayout(horizontalLayout_4, 1, 1, 1, 1);

        label_5 = new QLabel(JiaoDingWeiForm);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout->addWidget(label_5, 8, 0, 1, 1);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        btnZ = new QPushButton(JiaoDingWeiForm);
        btnZ->setObjectName(QString::fromUtf8("btnZ"));
        sizePolicy.setHeightForWidth(btnZ->sizePolicy().hasHeightForWidth());
        btnZ->setSizePolicy(sizePolicy);
        btnZ->setFont(font);
        btnZ->setCheckable(true);

        horizontalLayout_8->addWidget(btnZ);

        inZ = new QLineEdit4AxisValue(JiaoDingWeiForm);
        inZ->setObjectName(QString::fromUtf8("inZ"));
        sizePolicy1.setHeightForWidth(inZ->sizePolicy().hasHeightForWidth());
        inZ->setSizePolicy(sizePolicy1);
        inZ->setFont(font);
        inZ->setReadOnly(true);

        horizontalLayout_8->addWidget(inZ);


        gridLayout->addLayout(horizontalLayout_8, 3, 1, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        inSetToZero = new QPushButton(JiaoDingWeiForm);
        buttonGroup = new QButtonGroup(JiaoDingWeiForm);
        buttonGroup->setObjectName(QString::fromUtf8("buttonGroup"));
        buttonGroup->addButton(inSetToZero);
        inSetToZero->setObjectName(QString::fromUtf8("inSetToZero"));
        sizePolicy.setHeightForWidth(inSetToZero->sizePolicy().hasHeightForWidth());
        inSetToZero->setSizePolicy(sizePolicy);
        inSetToZero->setCheckable(true);
        inSetToZero->setChecked(true);

        horizontalLayout->addWidget(inSetToZero);

        pushButton_6 = new QPushButton(JiaoDingWeiForm);
        buttonGroup->addButton(pushButton_6);
        pushButton_6->setObjectName(QString::fromUtf8("pushButton_6"));
        sizePolicy.setHeightForWidth(pushButton_6->sizePolicy().hasHeightForWidth());
        pushButton_6->setSizePolicy(sizePolicy);
        pushButton_6->setCheckable(true);
        pushButton_6->setChecked(false);

        horizontalLayout->addWidget(pushButton_6);


        gridLayout->addLayout(horizontalLayout, 6, 1, 1, 1);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        btnY = new QPushButton(JiaoDingWeiForm);
        btnY->setObjectName(QString::fromUtf8("btnY"));
        sizePolicy.setHeightForWidth(btnY->sizePolicy().hasHeightForWidth());
        btnY->setSizePolicy(sizePolicy);
        btnY->setFont(font);
        btnY->setCheckable(true);

        horizontalLayout_7->addWidget(btnY);

        inY = new QLineEdit4AxisValue(JiaoDingWeiForm);
        inY->setObjectName(QString::fromUtf8("inY"));
        sizePolicy1.setHeightForWidth(inY->sizePolicy().hasHeightForWidth());
        inY->setSizePolicy(sizePolicy1);
        inY->setFont(font);
        inY->setReadOnly(true);

        horizontalLayout_7->addWidget(inY);


        gridLayout->addLayout(horizontalLayout_7, 2, 1, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        inSetToZTouch = new QPushButton(JiaoDingWeiForm);
        buttonGroup_2 = new QButtonGroup(JiaoDingWeiForm);
        buttonGroup_2->setObjectName(QString::fromUtf8("buttonGroup_2"));
        buttonGroup_2->addButton(inSetToZTouch);
        inSetToZTouch->setObjectName(QString::fromUtf8("inSetToZTouch"));
        sizePolicy.setHeightForWidth(inSetToZTouch->sizePolicy().hasHeightForWidth());
        inSetToZTouch->setSizePolicy(sizePolicy);
        inSetToZTouch->setCheckable(true);
        inSetToZTouch->setChecked(true);

        horizontalLayout_3->addWidget(inSetToZTouch);

        pushButton = new QPushButton(JiaoDingWeiForm);
        buttonGroup_2->addButton(pushButton);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        sizePolicy.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy);
        pushButton->setCheckable(true);

        horizontalLayout_3->addWidget(pushButton);


        gridLayout->addLayout(horizontalLayout_3, 5, 1, 1, 1);

        label_6 = new QLabel(JiaoDingWeiForm);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout->addWidget(label_6, 7, 0, 1, 1);

        inCnt = new QLineEdit4Keyboard(JiaoDingWeiForm);
        inCnt->setObjectName(QString::fromUtf8("inCnt"));

        gridLayout->addWidget(inCnt, 7, 1, 1, 1);


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
        labelPix = new QLabel(JiaoDingWeiForm);
        labelPix->setObjectName(QString::fromUtf8("labelPix"));

        verticalLayout->addWidget(labelPix);

        textBrowser = new QTextBrowser(JiaoDingWeiForm);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));

        verticalLayout->addWidget(textBrowser);

        verticalLayout->setStretch(0, 2);
        verticalLayout->setStretch(1, 3);

        horizontalLayout_9->addLayout(verticalLayout);

        horizontalLayout_9->setStretch(0, 3);
        horizontalLayout_9->setStretch(1, 1);

        retranslateUi(JiaoDingWeiForm);

        QMetaObject::connectSlotsByName(JiaoDingWeiForm);
    } // setupUi

    void retranslateUi(QWidget *JiaoDingWeiForm)
    {
        JiaoDingWeiForm->setWindowTitle(QCoreApplication::translate("JiaoDingWeiForm", "Form", nullptr));
        label_4->setText(QCoreApplication::translate("JiaoDingWeiForm", "\346\216\245\350\247\246\346\204\237\347\237\245\345\233\236\351\200\200\345\200\274", nullptr));
        label_7->setText(QCoreApplication::translate("JiaoDingWeiForm", "\350\247\222\344\275\215\347\275\256", nullptr));
        inBtnType1->setText(QCoreApplication::translate("JiaoDingWeiForm", "\345\267\246\344\270\212", nullptr));
        inBtnType2->setText(QCoreApplication::translate("JiaoDingWeiForm", "\345\267\246\344\270\213", nullptr));
        inBtnType3->setText(QCoreApplication::translate("JiaoDingWeiForm", "\345\217\263\344\270\212", nullptr));
        inBtnType4->setText(QCoreApplication::translate("JiaoDingWeiForm", "\345\217\263\344\270\213", nullptr));
        inBack->setText(QCoreApplication::translate("JiaoDingWeiForm", "0.5", nullptr));
        label_1->setText(QCoreApplication::translate("JiaoDingWeiForm", "\345\277\253\350\277\233\351\207\217", nullptr));
        label_3->setText(QCoreApplication::translate("JiaoDingWeiForm", "\345\235\220\346\240\2070\350\256\276\345\256\232", nullptr));
        label_2->setText(QCoreApplication::translate("JiaoDingWeiForm", "Z\350\275\264\346\216\245\350\247\246\346\204\237\347\237\245\345\212\250\344\275\234", nullptr));
        btnX->setText(QCoreApplication::translate("JiaoDingWeiForm", "X", nullptr));
        inX->setText(QString());
        label_5->setText(QCoreApplication::translate("JiaoDingWeiForm", "\347\273\223\346\236\234", nullptr));
        btnZ->setText(QCoreApplication::translate("JiaoDingWeiForm", "Z", nullptr));
        inZ->setText(QString());
        inSetToZero->setText(QCoreApplication::translate("JiaoDingWeiForm", "ON", nullptr));
        pushButton_6->setText(QCoreApplication::translate("JiaoDingWeiForm", "OFF", nullptr));
        btnY->setText(QCoreApplication::translate("JiaoDingWeiForm", "Y", nullptr));
        inY->setText(QString());
        inSetToZTouch->setText(QCoreApplication::translate("JiaoDingWeiForm", "ON", nullptr));
        pushButton->setText(QCoreApplication::translate("JiaoDingWeiForm", "OFF", nullptr));
        label_6->setText(QCoreApplication::translate("JiaoDingWeiForm", "\346\254\241\346\225\260", nullptr));
        inCnt->setText(QCoreApplication::translate("JiaoDingWeiForm", "1", nullptr));
        labelPix->setText(QString());
        textBrowser->setHtml(QCoreApplication::translate("JiaoDingWeiForm", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Microsoft YaHei UI'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">                 </p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">                 </p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'SimSun'; font-size:20pt; font-weight:696;\">[\350\275\264\345\217\267]</"
                        "span>                 </p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'SimSun'; font-size:20pt; font-weight:696;\">\345\212\250\344\275\234\347\232\204\345\257\271\345\272\224\350\275\264\345\217\267\343\200\202</span>                 </p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'SimSun'; font-size:20pt; font-weight:696;\">[\346\226\271\345\220\221]</span>                 </p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'SimSun'; font-size:20pt; font-weight:696;\">\345\212\250\344\275\234\347\232\204\346\226\271\345\220\221\357\274\214\342\200\234+\342\200\235\344\270\272\345\257\271\345\272\224\350\275\264\346\255\243\346\226\271\345\220\221\357\274\214\342\200\234-\342\200\235\344\270\272"
                        "\345\257\271\345\272\224\350\275\264\347\232\204\350\264\237\346\226\271\345\220\221\343\200\202</span>               </p></body></html>", nullptr));
    } // retranslateUi

};

namespace Ui {
    class JiaoDingWeiForm: public Ui_JiaoDingWeiForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_JIAODINGWEIFORM_H
