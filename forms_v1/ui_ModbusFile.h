/********************************************************************************
** Form generated from reading UI file 'ModbusFile.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MODBUSFILE_H
#define UI_MODBUSFILE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ModbusFile
{
public:
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *editRecord;
    QLineEdit *editFileNum;
    QLabel *label_2;
    QLineEdit *editNum;
    QLabel *label_4;
    QLabel *label_3;
    QLineEdit *editData;
    QHBoxLayout *horizontalLayout;
    QPushButton *btnRead;
    QPushButton *btnWrite;

    void setupUi(QDialog *ModbusFile)
    {
        if (ModbusFile->objectName().isEmpty())
            ModbusFile->setObjectName(QString::fromUtf8("ModbusFile"));
        ModbusFile->resize(857, 165);
        layoutWidget = new QWidget(ModbusFile);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(0, 0, 857, 151));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(layoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        editRecord = new QLineEdit(layoutWidget);
        editRecord->setObjectName(QString::fromUtf8("editRecord"));

        gridLayout->addWidget(editRecord, 1, 1, 1, 1);

        editFileNum = new QLineEdit(layoutWidget);
        editFileNum->setObjectName(QString::fromUtf8("editFileNum"));

        gridLayout->addWidget(editFileNum, 0, 1, 1, 1);

        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        editNum = new QLineEdit(layoutWidget);
        editNum->setObjectName(QString::fromUtf8("editNum"));

        gridLayout->addWidget(editNum, 2, 1, 1, 1);

        label_4 = new QLabel(layoutWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 3, 0, 1, 1);

        label_3 = new QLabel(layoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        editData = new QLineEdit(layoutWidget);
        editData->setObjectName(QString::fromUtf8("editData"));

        gridLayout->addWidget(editData, 3, 1, 1, 1);


        verticalLayout->addLayout(gridLayout);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        btnRead = new QPushButton(layoutWidget);
        btnRead->setObjectName(QString::fromUtf8("btnRead"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(btnRead->sizePolicy().hasHeightForWidth());
        btnRead->setSizePolicy(sizePolicy);
        btnRead->setFlat(false);

        horizontalLayout->addWidget(btnRead, 0, Qt::AlignTop);

        btnWrite = new QPushButton(layoutWidget);
        btnWrite->setObjectName(QString::fromUtf8("btnWrite"));
        sizePolicy.setHeightForWidth(btnWrite->sizePolicy().hasHeightForWidth());
        btnWrite->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(btnWrite, 0, Qt::AlignTop);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(ModbusFile);

        QMetaObject::connectSlotsByName(ModbusFile);
    } // setupUi

    void retranslateUi(QDialog *ModbusFile)
    {
        ModbusFile->setWindowTitle(QCoreApplication::translate("ModbusFile", "ModbusFile", nullptr));
        label->setText(QCoreApplication::translate("ModbusFile", "FileNum", nullptr));
        editRecord->setText(QCoreApplication::translate("ModbusFile", "0", nullptr));
        editFileNum->setText(QCoreApplication::translate("ModbusFile", "0", nullptr));
        label_2->setText(QCoreApplication::translate("ModbusFile", "Record", nullptr));
        editNum->setText(QCoreApplication::translate("ModbusFile", "1", nullptr));
        label_4->setText(QCoreApplication::translate("ModbusFile", "Data", nullptr));
        label_3->setText(QCoreApplication::translate("ModbusFile", "Num", nullptr));
        btnRead->setText(QCoreApplication::translate("ModbusFile", "Read", nullptr));
        btnWrite->setText(QCoreApplication::translate("ModbusFile", "Write", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ModbusFile: public Ui_ModbusFile {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MODBUSFILE_H
