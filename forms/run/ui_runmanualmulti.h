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
        label_5->setText(QCoreApplication::translate("RunManualMulti", "JGHS", nullptr));
        label_1->setText(QCoreApplication::translate("RunManualMulti", "JGFF", nullptr));
        btnJgff->setText(QCoreApplication::translate("RunManualMulti", "DD", nullptr));
        btnJgff2->setText(QCoreApplication::translate("RunManualMulti", "LX", nullptr));
        label_3->setText(QCoreApplication::translate("RunManualMulti", "ZZKSGD", nullptr));
        label_2->setText(QCoreApplication::translate("RunManualMulti", "ZZAQGD", nullptr));
        labelPix->setText(QString());
        textBrowser->setHtml(QString());
    } // retranslateUi

};

namespace Ui {
    class RunManualMulti: public Ui_RunManualMulti {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RUNMANUALMULTI_H
