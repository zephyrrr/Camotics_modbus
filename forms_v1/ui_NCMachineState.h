/********************************************************************************
** Form generated from reading UI file 'NCMachineState.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NCMACHINESTATE_H
#define UI_NCMACHINESTATE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FormNCMachineState
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label_10;
    QLabel *xLabel;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_11;
    QLabel *yLabel;
    QLabel *msgLabel;

    void setupUi(QWidget *FormNCMachineState)
    {
        if (FormNCMachineState->objectName().isEmpty())
            FormNCMachineState->setObjectName(QString::fromUtf8("FormNCMachineState"));
        verticalLayout = new QVBoxLayout(FormNCMachineState);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_10 = new QLabel(FormNCMachineState);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_10->sizePolicy().hasHeightForWidth());
        label_10->setSizePolicy(sizePolicy);
        QFont font;
        font.setPointSize(20);
        font.setUnderline(true);
        label_10->setFont(font);
        label_10->setStyleSheet(QString::fromUtf8("color:red"));

        horizontalLayout->addWidget(label_10);

        xLabel = new QLabel(FormNCMachineState);
        xLabel->setObjectName(QString::fromUtf8("xLabel"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Courier 10 Pitch"));
        font1.setPointSize(20);
        xLabel->setFont(font1);
        xLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout->addWidget(xLabel);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_11 = new QLabel(FormNCMachineState);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        sizePolicy.setHeightForWidth(label_11->sizePolicy().hasHeightForWidth());
        label_11->setSizePolicy(sizePolicy);
        label_11->setFont(font);
        label_11->setStyleSheet(QString::fromUtf8("color:green"));

        horizontalLayout_2->addWidget(label_11);

        yLabel = new QLabel(FormNCMachineState);
        yLabel->setObjectName(QString::fromUtf8("yLabel"));
        yLabel->setFont(font1);
        yLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_2->addWidget(yLabel);


        verticalLayout->addLayout(horizontalLayout_2);

        msgLabel = new QLabel(FormNCMachineState);
        msgLabel->setObjectName(QString::fromUtf8("msgLabel"));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Microsoft YaHei UI"));
        font2.setPointSize(8);
        msgLabel->setFont(font2);
        msgLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        verticalLayout->addWidget(msgLabel);


        retranslateUi(FormNCMachineState);

        QMetaObject::connectSlotsByName(FormNCMachineState);
    } // setupUi

    void retranslateUi(QWidget *FormNCMachineState)
    {
        label_10->setText(QCoreApplication::translate("FormNCMachineState", "YXZT", nullptr));
        xLabel->setText(QCoreApplication::translate("FormNCMachineState", "ZC", nullptr));
        label_11->setText(QCoreApplication::translate("FormNCMachineState", "ZXJG", nullptr));
        yLabel->setText(QCoreApplication::translate("FormNCMachineState", "ZC", nullptr));
        msgLabel->setText(QCoreApplication::translate("FormNCMachineState", "ZC", nullptr));
        (void)FormNCMachineState;
    } // retranslateUi

};

namespace Ui {
    class FormNCMachineState: public Ui_FormNCMachineState {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NCMACHINESTATE_H
