/********************************************************************************
** Form generated from reading UI file 'runmanual.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RUNMANUAL_H
#define UI_RUNMANUAL_H

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

QT_BEGIN_NAMESPACE

class Ui_RunManualClass
{
public:
    QHBoxLayout *horizontalLayout_9;
    QVBoxLayout *verticalLayout_2;
    QSpacerItem *verticalSpacer1;
    QHBoxLayout *horizontalLayout_2;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout_table1;
    QLabel *label_3;
    QHBoxLayout *horizontalLayout;
    QPushButton *inAbsolute;
    QPushButton *inAbsolute2;
    QLabel *label_1;
    QVBoxLayout *verticalLayout_table2;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *verticalSpacer2;
    QVBoxLayout *verticalLayout;
    QLabel *labelPix;
    QTextBrowser *textBrowser;
    QButtonGroup *buttonGroup;

    void setupUi(QWidget *RunManualClass)
    {
        if (RunManualClass->objectName().isEmpty())
            RunManualClass->setObjectName(QString::fromUtf8("RunManualClass"));
        RunManualClass->resize(795, 514);
        horizontalLayout_9 = new QHBoxLayout(RunManualClass);
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalSpacer1 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(30);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(30, 30, 30, 30);
        verticalLayout_table1 = new QVBoxLayout();
        verticalLayout_table1->setSpacing(6);
        verticalLayout_table1->setObjectName(QString::fromUtf8("verticalLayout_table1"));

        gridLayout->addLayout(verticalLayout_table1, 1, 1, 1, 1);

        label_3 = new QLabel(RunManualClass);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy);

        gridLayout->addWidget(label_3, 0, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        inAbsolute = new QPushButton(RunManualClass);
        buttonGroup = new QButtonGroup(RunManualClass);
        buttonGroup->setObjectName(QString::fromUtf8("buttonGroup"));
        buttonGroup->addButton(inAbsolute);
        inAbsolute->setObjectName(QString::fromUtf8("inAbsolute"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(inAbsolute->sizePolicy().hasHeightForWidth());
        inAbsolute->setSizePolicy(sizePolicy1);
        inAbsolute->setCheckable(true);
        inAbsolute->setChecked(true);

        horizontalLayout->addWidget(inAbsolute);

        inAbsolute2 = new QPushButton(RunManualClass);
        buttonGroup->addButton(inAbsolute2);
        inAbsolute2->setObjectName(QString::fromUtf8("inAbsolute2"));
        sizePolicy1.setHeightForWidth(inAbsolute2->sizePolicy().hasHeightForWidth());
        inAbsolute2->setSizePolicy(sizePolicy1);
        inAbsolute2->setCheckable(true);
        inAbsolute2->setChecked(false);

        horizontalLayout->addWidget(inAbsolute2);


        gridLayout->addLayout(horizontalLayout, 0, 1, 1, 1);

        label_1 = new QLabel(RunManualClass);
        label_1->setObjectName(QString::fromUtf8("label_1"));

        gridLayout->addWidget(label_1, 1, 0, 1, 1);

        verticalLayout_table2 = new QVBoxLayout();
        verticalLayout_table2->setSpacing(6);
        verticalLayout_table2->setObjectName(QString::fromUtf8("verticalLayout_table2"));

        gridLayout->addLayout(verticalLayout_table2, 2, 0, 1, 2);


        horizontalLayout_2->addLayout(gridLayout);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        horizontalLayout_2->setStretch(0, 2);
        horizontalLayout_2->setStretch(1, 1);

        verticalLayout_2->addLayout(horizontalLayout_2);

        verticalSpacer2 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer2);


        horizontalLayout_9->addLayout(verticalLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        labelPix = new QLabel(RunManualClass);
        labelPix->setObjectName(QString::fromUtf8("labelPix"));

        verticalLayout->addWidget(labelPix);

        textBrowser = new QTextBrowser(RunManualClass);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));

        verticalLayout->addWidget(textBrowser);

        verticalLayout->setStretch(0, 2);
        verticalLayout->setStretch(1, 3);

        horizontalLayout_9->addLayout(verticalLayout);

        horizontalLayout_9->setStretch(0, 3);
        horizontalLayout_9->setStretch(1, 1);

        retranslateUi(RunManualClass);

        QMetaObject::connectSlotsByName(RunManualClass);
    } // setupUi

    void retranslateUi(QWidget *RunManualClass)
    {
        RunManualClass->setWindowTitle(QCoreApplication::translate("RunManualClass", "RunManual", nullptr));
        label_3->setText(QCoreApplication::translate("RunManualClass", "\345\235\220\346\240\207", nullptr));
        inAbsolute->setText(QCoreApplication::translate("RunManualClass", "\347\273\235\345\257\271", nullptr));
        inAbsolute2->setText(QCoreApplication::translate("RunManualClass", "\347\233\270\345\257\271", nullptr));
        label_1->setText(QCoreApplication::translate("RunManualClass", "\345\212\240\345\267\245\346\267\261\345\272\246", nullptr));
        labelPix->setText(QString());
        textBrowser->setHtml(QCoreApplication::translate("RunManualClass", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
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
    class RunManualClass: public Ui_RunManualClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RUNMANUAL_H
