/********************************************************************************
** Form generated from reading UI file 'runmanualmulti.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RUNMANUALMULTI_H
#define UI_RUNMANUALMULTI_H

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

class Ui_RunManualMulti
{
public:
    QHBoxLayout *horizontalLayout_9;
    QVBoxLayout *verticalLayout2;
    QSpacerItem *verticalSpacer1;
    QHBoxLayout *horizontalLayout_2;
    QGridLayout *gridLayout;
    QPushButton *btnJghs;
    QLabel *label_5;
    QLabel *label_1;
    QHBoxLayout *horizontalLayout;
    QPushButton *btnJgff;
    QPushButton *btnJgff2;
    QLabel *label_3;
    QLineEdit4AxisValue *lineEditZKaisi;
    QLabel *label_2;
    QLineEdit4AxisValue *lineEditZAnQuan;
    QVBoxLayout *verticalLayout_table1;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *verticalSpacer2;
    QVBoxLayout *verticalLayout;
    QLabel *labelPix;
    QTextBrowser *textBrowser;
    QButtonGroup *buttonGroup;

    void setupUi(QWidget *RunManualMulti)
    {
        if (RunManualMulti->objectName().isEmpty())
            RunManualMulti->setObjectName(QString::fromUtf8("RunManualMulti"));
        RunManualMulti->resize(795, 534);
        horizontalLayout_9 = new QHBoxLayout(RunManualMulti);
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        verticalLayout2 = new QVBoxLayout();
        verticalLayout2->setSpacing(6);
        verticalLayout2->setObjectName(QString::fromUtf8("verticalLayout2"));
        verticalSpacer1 = new QSpacerItem(20, 10, QSizePolicy::Policy::Minimum, QSizePolicy::Minimum);

        verticalLayout2->addItem(verticalSpacer1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(30);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(30, 30, 30, 30);
        btnJghs = new QPushButton(RunManualMulti);
        btnJghs->setObjectName(QString::fromUtf8("btnJghs"));

        gridLayout->addWidget(btnJghs, 0, 1, 1, 1);

        label_5 = new QLabel(RunManualMulti);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout->addWidget(label_5, 0, 0, 1, 1);

        label_1 = new QLabel(RunManualMulti);
        label_1->setObjectName(QString::fromUtf8("label_1"));

        gridLayout->addWidget(label_1, 1, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        btnJgff = new QPushButton(RunManualMulti);
        buttonGroup = new QButtonGroup(RunManualMulti);
        buttonGroup->setObjectName(QString::fromUtf8("buttonGroup"));
        buttonGroup->addButton(btnJgff);
        btnJgff->setObjectName(QString::fromUtf8("btnJgff"));
        btnJgff->setCheckable(true);
        btnJgff->setChecked(true);

        horizontalLayout->addWidget(btnJgff);

        btnJgff2 = new QPushButton(RunManualMulti);
        buttonGroup->addButton(btnJgff2);
        btnJgff2->setObjectName(QString::fromUtf8("btnJgff2"));
        btnJgff2->setCheckable(true);

        horizontalLayout->addWidget(btnJgff2);


        gridLayout->addLayout(horizontalLayout, 1, 1, 1, 1);

        label_3 = new QLabel(RunManualMulti);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 3, 0, 1, 1);

        lineEditZKaisi = new QLineEdit4AxisValue(RunManualMulti);
        lineEditZKaisi->setObjectName(QString::fromUtf8("lineEditZKaisi"));

        gridLayout->addWidget(lineEditZKaisi, 3, 1, 1, 1);

        label_2 = new QLabel(RunManualMulti);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 2, 0, 1, 1);

        lineEditZAnQuan = new QLineEdit4AxisValue(RunManualMulti);
        lineEditZAnQuan->setObjectName(QString::fromUtf8("lineEditZAnQuan"));

        gridLayout->addWidget(lineEditZAnQuan, 2, 1, 1, 1);

        verticalLayout_table1 = new QVBoxLayout();
        verticalLayout_table1->setSpacing(6);
        verticalLayout_table1->setObjectName(QString::fromUtf8("verticalLayout_table1"));

        gridLayout->addLayout(verticalLayout_table1, 4, 0, 1, 2);


        horizontalLayout_2->addLayout(gridLayout);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        horizontalLayout_2->setStretch(0, 2);
        horizontalLayout_2->setStretch(1, 1);

        verticalLayout2->addLayout(horizontalLayout_2);

        verticalSpacer2 = new QSpacerItem(20, 10, QSizePolicy::Policy::Minimum, QSizePolicy::Minimum);

        verticalLayout2->addItem(verticalSpacer2);


        horizontalLayout_9->addLayout(verticalLayout2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        labelPix = new QLabel(RunManualMulti);
        labelPix->setObjectName(QString::fromUtf8("labelPix"));

        verticalLayout->addWidget(labelPix);

        textBrowser = new QTextBrowser(RunManualMulti);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));

        verticalLayout->addWidget(textBrowser);

        verticalLayout->setStretch(0, 2);
        verticalLayout->setStretch(1, 3);

        horizontalLayout_9->addLayout(verticalLayout);

        horizontalLayout_9->setStretch(0, 3);
        horizontalLayout_9->setStretch(1, 1);

        retranslateUi(RunManualMulti);

        QMetaObject::connectSlotsByName(RunManualMulti);
    } // setupUi

    void retranslateUi(QWidget *RunManualMulti)
    {
        RunManualMulti->setWindowTitle(QCoreApplication::translate("RunManualMulti", "RunManual", nullptr));
        btnJghs->setText(QCoreApplication::translate("RunManualMulti", "8", nullptr));
        label_5->setText(QCoreApplication::translate("RunManualMulti", "\345\212\240\345\267\245\345\233\236\346\225\260", nullptr));
        label_1->setText(QCoreApplication::translate("RunManualMulti", "\345\212\240\345\267\245\346\226\271\346\263\225", nullptr));
        btnJgff->setText(QCoreApplication::translate("RunManualMulti", "\345\215\225\347\213\254", nullptr));
        btnJgff2->setText(QCoreApplication::translate("RunManualMulti", "\350\277\236\347\273\255", nullptr));
        label_3->setText(QCoreApplication::translate("RunManualMulti", "Z\350\275\264\345\274\200\345\247\213\351\253\230\345\272\246", nullptr));
        label_2->setText(QCoreApplication::translate("RunManualMulti", "Z\350\275\264\345\256\211\345\205\250\351\253\230\345\272\246", nullptr));
        labelPix->setText(QString());
        textBrowser->setHtml(QCoreApplication::translate("RunManualMulti", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
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
    class RunManualMulti: public Ui_RunManualMulti {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RUNMANUALMULTI_H
