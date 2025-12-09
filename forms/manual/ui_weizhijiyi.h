/********************************************************************************
** Form generated from reading UI file 'weizhijiyi.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WEIZHIJIYI_H
#define UI_WEIZHIJIYI_H

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

class Ui_WeiZhiJiYiForm
{
public:
    QHBoxLayout *horizontalLayout_4;
    QVBoxLayout *verticalLayout_2;
    QSpacerItem *verticalSpacer1;
    QGridLayout *gridLayout;
    QLabel *label_3;
    QLabel *label_1;
    QSpacerItem *horizontalSpacer;
    QLabel *label_2;
    QPushButton *btnChangeCs;
    QPushButton *pushButton;
    QPushButton *btnChangeZ;
    QPushButton *pushButton_3;
    QPushButton *btnChangeU;
    QPushButton *pushButton_5;
    QVBoxLayout *mainTableLayout;
    QSpacerItem *verticalSpacer2;
    QVBoxLayout *verticalLayout;
    QLabel *labelPix;
    QTextBrowser *textBrowser;
    QButtonGroup *buttonGroup_3;
    QButtonGroup *buttonGroup;
    QButtonGroup *buttonGroup_2;

    void setupUi(QWidget *WeiZhiJiYiForm)
    {
        if (WeiZhiJiYiForm->objectName().isEmpty())
            WeiZhiJiYiForm->setObjectName(QString::fromUtf8("WeiZhiJiYiForm"));
        WeiZhiJiYiForm->resize(579, 415);
        horizontalLayout_4 = new QHBoxLayout(WeiZhiJiYiForm);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalSpacer1 = new QSpacerItem(20, 13, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer1);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_3 = new QLabel(WeiZhiJiYiForm);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        label_1 = new QLabel(WeiZhiJiYiForm);
        label_1->setObjectName(QString::fromUtf8("label_1"));

        gridLayout->addWidget(label_1, 0, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 3, 1, 1);

        label_2 = new QLabel(WeiZhiJiYiForm);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        btnChangeCs = new QPushButton(WeiZhiJiYiForm);
        buttonGroup = new QButtonGroup(WeiZhiJiYiForm);
        buttonGroup->setObjectName(QString::fromUtf8("buttonGroup"));
        buttonGroup->addButton(btnChangeCs);
        btnChangeCs->setObjectName(QString::fromUtf8("btnChangeCs"));
        btnChangeCs->setCheckable(true);
        btnChangeCs->setChecked(true);

        gridLayout->addWidget(btnChangeCs, 0, 1, 1, 1);

        pushButton = new QPushButton(WeiZhiJiYiForm);
        buttonGroup->addButton(pushButton);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setCheckable(true);

        gridLayout->addWidget(pushButton, 0, 2, 1, 1);

        btnChangeZ = new QPushButton(WeiZhiJiYiForm);
        buttonGroup_2 = new QButtonGroup(WeiZhiJiYiForm);
        buttonGroup_2->setObjectName(QString::fromUtf8("buttonGroup_2"));
        buttonGroup_2->addButton(btnChangeZ);
        btnChangeZ->setObjectName(QString::fromUtf8("btnChangeZ"));
        btnChangeZ->setCheckable(true);
        btnChangeZ->setChecked(true);

        gridLayout->addWidget(btnChangeZ, 1, 1, 1, 1);

        pushButton_3 = new QPushButton(WeiZhiJiYiForm);
        buttonGroup_2->addButton(pushButton_3);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setCheckable(true);
        pushButton_3->setChecked(false);

        gridLayout->addWidget(pushButton_3, 1, 2, 1, 1);

        btnChangeU = new QPushButton(WeiZhiJiYiForm);
        buttonGroup_3 = new QButtonGroup(WeiZhiJiYiForm);
        buttonGroup_3->setObjectName(QString::fromUtf8("buttonGroup_3"));
        buttonGroup_3->addButton(btnChangeU);
        btnChangeU->setObjectName(QString::fromUtf8("btnChangeU"));
        btnChangeU->setCheckable(true);
        btnChangeU->setChecked(true);

        gridLayout->addWidget(btnChangeU, 2, 1, 1, 1);

        pushButton_5 = new QPushButton(WeiZhiJiYiForm);
        buttonGroup_3->addButton(pushButton_5);
        pushButton_5->setObjectName(QString::fromUtf8("pushButton_5"));
        pushButton_5->setCheckable(true);
        pushButton_5->setChecked(true);

        gridLayout->addWidget(pushButton_5, 2, 2, 1, 1);

        mainTableLayout = new QVBoxLayout();
        mainTableLayout->setObjectName(QString::fromUtf8("mainTableLayout"));

        gridLayout->addLayout(mainTableLayout, 3, 0, 1, 3);

        gridLayout->setColumnStretch(0, 1);
        gridLayout->setColumnStretch(1, 3);
        gridLayout->setColumnStretch(2, 3);
        gridLayout->setColumnStretch(3, 4);

        verticalLayout_2->addLayout(gridLayout);

        verticalSpacer2 = new QSpacerItem(20, 13, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer2);


        horizontalLayout_4->addLayout(verticalLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        labelPix = new QLabel(WeiZhiJiYiForm);
        labelPix->setObjectName(QString::fromUtf8("labelPix"));

        verticalLayout->addWidget(labelPix);

        textBrowser = new QTextBrowser(WeiZhiJiYiForm);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));

        verticalLayout->addWidget(textBrowser);

        verticalLayout->setStretch(0, 2);
        verticalLayout->setStretch(1, 3);

        horizontalLayout_4->addLayout(verticalLayout);

        horizontalLayout_4->setStretch(0, 3);
        horizontalLayout_4->setStretch(1, 1);

        retranslateUi(WeiZhiJiYiForm);

        QMetaObject::connectSlotsByName(WeiZhiJiYiForm);
    } // setupUi

    void retranslateUi(QWidget *WeiZhiJiYiForm)
    {
        WeiZhiJiYiForm->setWindowTitle(QCoreApplication::translate("WeiZhiJiYiForm", "Form", nullptr));
        label_3->setText(QCoreApplication::translate("WeiZhiJiYiForm", "U\350\275\264\347\247\273\345\212\250", nullptr));
        label_1->setText(QCoreApplication::translate("WeiZhiJiYiForm", "\345\235\220\346\240\207\347\263\273\345\210\207\346\215\242", nullptr));
        label_2->setText(QCoreApplication::translate("WeiZhiJiYiForm", "Z\350\275\264\347\247\273\345\212\250", nullptr));
        btnChangeCs->setText(QCoreApplication::translate("WeiZhiJiYiForm", "ON", nullptr));
        pushButton->setText(QCoreApplication::translate("WeiZhiJiYiForm", "OFF", nullptr));
        btnChangeZ->setText(QCoreApplication::translate("WeiZhiJiYiForm", "ON", nullptr));
        pushButton_3->setText(QCoreApplication::translate("WeiZhiJiYiForm", "OFF", nullptr));
        btnChangeU->setText(QCoreApplication::translate("WeiZhiJiYiForm", "ON", nullptr));
        pushButton_5->setText(QCoreApplication::translate("WeiZhiJiYiForm", "OFF", nullptr));
        labelPix->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class WeiZhiJiYiForm: public Ui_WeiZhiJiYiForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WEIZHIJIYI_H
