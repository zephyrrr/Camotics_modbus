/********************************************************************************
** Form generated from reading UI file 'settingsmodbustcp.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGSMODBUSTCP_H
#define UI_SETTINGSMODBUSTCP_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SettingsModbusTCP
{
public:
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QLabel *lblSlaveIP;
    QLabel *lblTCPPort;
    QLineEdit *leTCPPort;
    QLineEdit *leSlaveIP;
    QDialogButtonBox *buttonBox;

    void setupUi(QWidget *SettingsModbusTCP)
    {
        if (SettingsModbusTCP->objectName().isEmpty())
            SettingsModbusTCP->setObjectName(QString::fromUtf8("SettingsModbusTCP"));
        SettingsModbusTCP->setProperty("modal", QVariant(false));
        SettingsModbusTCP->resize(240, 110);
        SettingsModbusTCP->setMinimumSize(QSize(240, 110));
        verticalLayout = new QVBoxLayout(SettingsModbusTCP);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        lblSlaveIP = new QLabel(SettingsModbusTCP);
        lblSlaveIP->setObjectName(QString::fromUtf8("lblSlaveIP"));

        gridLayout->addWidget(lblSlaveIP, 1, 0, 1, 1);

        lblTCPPort = new QLabel(SettingsModbusTCP);
        lblTCPPort->setObjectName(QString::fromUtf8("lblTCPPort"));
        lblTCPPort->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignVCenter);

        gridLayout->addWidget(lblTCPPort, 2, 0, 1, 1);

        leTCPPort = new QLineEdit(SettingsModbusTCP);
        leTCPPort->setObjectName(QString::fromUtf8("leTCPPort"));
        leTCPPort->setMaxLength(5);

        gridLayout->addWidget(leTCPPort, 2, 1, 1, 1);

        leSlaveIP = new QLineEdit(SettingsModbusTCP);
        leSlaveIP->setObjectName(QString::fromUtf8("leSlaveIP"));
        leSlaveIP->setInputMask(QString::fromUtf8("999.999.999.999;_"));

        gridLayout->addWidget(leSlaveIP, 1, 1, 1, 1);


        verticalLayout->addLayout(gridLayout);

        buttonBox = new QDialogButtonBox(SettingsModbusTCP);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);

#if QT_CONFIG(shortcut)
        lblTCPPort->setBuddy(leTCPPort);
#endif // QT_CONFIG(shortcut)

        retranslateUi(SettingsModbusTCP);

        QMetaObject::connectSlotsByName(SettingsModbusTCP);
    } // setupUi

    void retranslateUi(QWidget *SettingsModbusTCP)
    {
        SettingsModbusTCP->setWindowTitle(QCoreApplication::translate("SettingsModbusTCP", "Modbus TCP Settings", nullptr));
        lblSlaveIP->setText(QCoreApplication::translate("SettingsModbusTCP", "Slave IP", nullptr));
        lblTCPPort->setText(QCoreApplication::translate("SettingsModbusTCP", "TCP Port", nullptr));
        leTCPPort->setInputMask(QString());
        leTCPPort->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class SettingsModbusTCP: public Ui_SettingsModbusTCP {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGSMODBUSTCP_H
