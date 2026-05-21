/********************************************************************************
** Form generated from reading UI file 'tool4posset.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TOOL4POSSET_H
#define UI_TOOL4POSSET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Tool4PosSetClass
{
public:
    QVBoxLayout *verticalLayout;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QPushButton *btnRecord;
    QPushButton *btnCurrentPosIdx;
    QPushButton *btnBatchSet;
    QLabel *lblRecordDate;

    void setupUi(QWidget *Tool4PosSetClass)
    {
        if (Tool4PosSetClass->objectName().isEmpty())
            Tool4PosSetClass->setObjectName(QString::fromUtf8("Tool4PosSetClass"));
        Tool4PosSetClass->resize(360, 180);
        Tool4PosSetClass->setStyleSheet(QString::fromUtf8(""));
        verticalLayout = new QVBoxLayout(Tool4PosSetClass);
        verticalLayout->setSpacing(5);
        verticalLayout->setContentsMargins(5, 5, 5, 5);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(5);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(5);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        btnRecord = new QPushButton(Tool4PosSetClass);
        btnRecord->setObjectName(QString::fromUtf8("btnRecord"));

        horizontalLayout->addWidget(btnRecord);

        btnCurrentPosIdx = new QPushButton(Tool4PosSetClass);
        btnCurrentPosIdx->setObjectName(QString::fromUtf8("btnCurrentPosIdx"));
        btnCurrentPosIdx->setEnabled(false);

        horizontalLayout->addWidget(btnCurrentPosIdx);

        btnBatchSet = new QPushButton(Tool4PosSetClass);
        btnBatchSet->setObjectName(QString::fromUtf8("btnBatchSet"));

        horizontalLayout->addWidget(btnBatchSet);


        verticalLayout_2->addLayout(horizontalLayout);

        lblRecordDate = new QLabel(Tool4PosSetClass);
        lblRecordDate->setObjectName(QString::fromUtf8("lblRecordDate"));

        verticalLayout_2->addWidget(lblRecordDate);


        verticalLayout->addLayout(verticalLayout_2);


        retranslateUi(Tool4PosSetClass);

        QMetaObject::connectSlotsByName(Tool4PosSetClass);
    } // setupUi

    void retranslateUi(QWidget *Tool4PosSetClass)
    {
        Tool4PosSetClass->setWindowTitle(QCoreApplication::translate("Tool4PosSetClass", "Tool4PosSet", nullptr));
        btnRecord->setText(QCoreApplication::translate("Tool4PosSetClass", "JY", nullptr));
        btnCurrentPosIdx->setText(QCoreApplication::translate("Tool4PosSetClass", "1", nullptr));
        btnBatchSet->setText(QCoreApplication::translate("Tool4PosSetClass", "SZ", nullptr));
        lblRecordDate->setText(QCoreApplication::translate("Tool4PosSetClass", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Tool4PosSetClass: public Ui_Tool4PosSetClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TOOL4POSSET_H
