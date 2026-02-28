/********************************************************************************
** Form generated from reading UI file 'zuobiaoxishezi.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ZUOBIAOXISHEZI_H
#define UI_ZUOBIAOXISHEZI_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ZuoBiaoXiSheZiForm
{
public:
    QHBoxLayout *horizontalLayout_2;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_2;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_3;
    QWidget *widgetTable;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *verticalSpacer_2;
    QVBoxLayout *verticalLayout;
    QLabel *labelPix;
    QTextBrowser *textBrowser;

    void setupUi(QWidget *ZuoBiaoXiSheZiForm)
    {
        if (ZuoBiaoXiSheZiForm->objectName().isEmpty())
            ZuoBiaoXiSheZiForm->setObjectName(QString::fromUtf8("ZuoBiaoXiSheZiForm"));
        ZuoBiaoXiSheZiForm->resize(1085, 606);
        QFont font;
        font.setPointSize(14);
        ZuoBiaoXiSheZiForm->setFont(font);
        horizontalLayout_2 = new QHBoxLayout(ZuoBiaoXiSheZiForm);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalSpacer = new QSpacerItem(20, 50, QSizePolicy::Policy::Fixed, QSizePolicy::Minimum);

        verticalLayout_2->addItem(verticalSpacer);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(0);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        widgetTable = new QWidget(ZuoBiaoXiSheZiForm);
        widgetTable->setObjectName(QString::fromUtf8("widgetTable"));

        horizontalLayout_3->addWidget(widgetTable);

        horizontalSpacer = new QSpacerItem(10, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        horizontalLayout_3->setStretch(0, 2);
        horizontalLayout_3->setStretch(1, 1);

        verticalLayout_2->addLayout(horizontalLayout_3);

        verticalSpacer_2 = new QSpacerItem(20, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);

        verticalLayout_2->addItem(verticalSpacer_2);


        horizontalLayout->addLayout(verticalLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        labelPix = new QLabel(ZuoBiaoXiSheZiForm);
        labelPix->setObjectName(QString::fromUtf8("labelPix"));

        verticalLayout->addWidget(labelPix);

        textBrowser = new QTextBrowser(ZuoBiaoXiSheZiForm);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));

        verticalLayout->addWidget(textBrowser);

        verticalLayout->setStretch(0, 2);
        verticalLayout->setStretch(1, 3);

        horizontalLayout->addLayout(verticalLayout);

        horizontalLayout->setStretch(0, 3);
        horizontalLayout->setStretch(1, 1);

        horizontalLayout_2->addLayout(horizontalLayout);


        retranslateUi(ZuoBiaoXiSheZiForm);

        QMetaObject::connectSlotsByName(ZuoBiaoXiSheZiForm);
    } // setupUi

    void retranslateUi(QWidget *ZuoBiaoXiSheZiForm)
    {
        ZuoBiaoXiSheZiForm->setWindowTitle(QCoreApplication::translate("ZuoBiaoXiSheZiForm", "Form", nullptr));
        labelPix->setText(QString());
        textBrowser->setHtml(QString());
    } // retranslateUi

};

namespace Ui {
    class ZuoBiaoXiSheZiForm: public Ui_ZuoBiaoXiSheZiForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ZUOBIAOXISHEZI_H
