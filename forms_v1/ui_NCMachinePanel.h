/********************************************************************************
** Form generated from reading UI file 'NCMachinePanel.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NCMACHINEPANEL_H
#define UI_NCMACHINEPANEL_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_NCMachinePanel
{
public:
    QWidget *layoutWidget1;
    QGridLayout *gridLayout;
    QPushButton *btnSendKaiji;
    QPushButton *btnSendGuanji;
    QPushButton *btnSendBackToZero1;
    QPushButton *btnSendBackToZero3;
    QPushButton *btnSendBackToZero5;
    QWidget *layoutWidget2;
    QHBoxLayout *horizontalLayout;
    QLineEdit *lineEditState1;
    QLineEdit *lineEditState2;
    QLineEdit *lineEditState3;
    QPushButton *btnDebug1;
    QPushButton *btnDebug2;

    void setupUi(QWidget *NCMachinePanel)
    {
        if (NCMachinePanel->objectName().isEmpty())
            NCMachinePanel->setObjectName(QString::fromUtf8("NCMachinePanel"));
        NCMachinePanel->resize(400, 300);
        layoutWidget1 = new QWidget(NCMachinePanel);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(30, 30, 351, 161));
        gridLayout = new QGridLayout(layoutWidget1);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetMaximumSize);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        btnSendKaiji = new QPushButton(layoutWidget1);
        btnSendKaiji->setObjectName(QString::fromUtf8("btnSendKaiji"));

        gridLayout->addWidget(btnSendKaiji, 0, 0, 1, 1);

        btnSendGuanji = new QPushButton(layoutWidget1);
        btnSendGuanji->setObjectName(QString::fromUtf8("btnSendGuanji"));

        gridLayout->addWidget(btnSendGuanji, 0, 1, 1, 1);

        btnSendBackToZero1 = new QPushButton(layoutWidget1);
        btnSendBackToZero1->setObjectName(QString::fromUtf8("btnSendBackToZero1"));

        gridLayout->addWidget(btnSendBackToZero1, 1, 0, 1, 1);

        btnSendBackToZero3 = new QPushButton(layoutWidget1);
        btnSendBackToZero3->setObjectName(QString::fromUtf8("btnSendBackToZero3"));

        gridLayout->addWidget(btnSendBackToZero3, 2, 0, 1, 1);

        btnSendBackToZero5 = new QPushButton(layoutWidget1);
        btnSendBackToZero5->setObjectName(QString::fromUtf8("btnSendBackToZero5"));

        gridLayout->addWidget(btnSendBackToZero5, 3, 0, 1, 1);

        layoutWidget2 = new QWidget(NCMachinePanel);
        layoutWidget2->setObjectName(QString::fromUtf8("layoutWidget2"));
        layoutWidget2->setGeometry(QRect(0, 230, 381, 51));
        horizontalLayout = new QHBoxLayout(layoutWidget2);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        lineEditState1 = new QLineEdit(layoutWidget2);
        lineEditState1->setObjectName(QString::fromUtf8("lineEditState1"));
        lineEditState1->setToolTipDuration(-1);

        horizontalLayout->addWidget(lineEditState1);

        lineEditState2 = new QLineEdit(layoutWidget2);
        lineEditState2->setObjectName(QString::fromUtf8("lineEditState2"));

        horizontalLayout->addWidget(lineEditState2);

        lineEditState3 = new QLineEdit(layoutWidget2);
        lineEditState3->setObjectName(QString::fromUtf8("lineEditState3"));

        horizontalLayout->addWidget(lineEditState3);

        btnDebug1 = new QPushButton(layoutWidget2);
        btnDebug1->setObjectName(QString::fromUtf8("btnDebug1"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(btnDebug1->sizePolicy().hasHeightForWidth());
        btnDebug1->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(btnDebug1);

        btnDebug2 = new QPushButton(layoutWidget2);
        btnDebug2->setObjectName(QString::fromUtf8("btnDebug2"));

        horizontalLayout->addWidget(btnDebug2);


        retranslateUi(NCMachinePanel);

        QMetaObject::connectSlotsByName(NCMachinePanel);
    } // setupUi

    void retranslateUi(QWidget *NCMachinePanel)
    {
        NCMachinePanel->setWindowTitle(QCoreApplication::translate("NCMachinePanel", "Form", nullptr));
        btnSendKaiji->setText(QCoreApplication::translate("NCMachinePanel", "KJ", nullptr));
        btnSendGuanji->setText(QCoreApplication::translate("NCMachinePanel", "GJ", nullptr));
        btnSendBackToZero1->setText(QCoreApplication::translate("NCMachinePanel", "HLX", nullptr));
        btnSendBackToZero3->setText(QCoreApplication::translate("NCMachinePanel", "HLY", nullptr));
        btnSendBackToZero5->setText(QCoreApplication::translate("NCMachinePanel", "HLZ", nullptr));
        lineEditState1->setText(QCoreApplication::translate("NCMachinePanel", "0", nullptr));
        lineEditState2->setText(QCoreApplication::translate("NCMachinePanel", "0", nullptr));
        lineEditState3->setText(QCoreApplication::translate("NCMachinePanel", "0", nullptr));
        btnDebug1->setText(QCoreApplication::translate("NCMachinePanel", "SetState", nullptr));
        btnDebug2->setText(QCoreApplication::translate("NCMachinePanel", "Debug2", nullptr));
    } // retranslateUi

};

namespace Ui {
    class NCMachinePanel: public Ui_NCMachinePanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NCMACHINEPANEL_H
