/********************************************************************************
** Form generated from reading UI file 'settingsmodbusrtu.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGSMODBUSRTU_H
#define UI_SETTINGSMODBUSRTU_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SettingsModbusRTU
{
public:
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QComboBox *cmbRTS;
    QSpinBox *sbPort;
    QLabel *lblDev;
    QLabel *label;
    QSpinBox *sbByteTimeout;
    QLabel *lblDataBits;
    QLabel *lblRTS;
    QComboBox *cmbStopBits;
    QLabel *lblStopBits;
    QLabel *label_3;
    QLabel *label_2;
    QSpinBox *sbScanRate;
    QSpinBox *sbResponseTimeout;
    QComboBox *cmbDataBits;
    QLabel *lblPort;
    QLabel *lblBaud;
    QComboBox *cmbDev;
    QComboBox *cmbBaud;
    QLabel *lblParity;
    QComboBox *cmbParity;
    QDialogButtonBox *buttonBox;

    void setupUi(QWidget *SettingsModbusRTU)
    {
        if (SettingsModbusRTU->objectName().isEmpty())
            SettingsModbusRTU->setObjectName(QString::fromUtf8("SettingsModbusRTU"));
        SettingsModbusRTU->resize(313, 327);
        SettingsModbusRTU->setMinimumSize(QSize(220, 256));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/options-16.png"), QSize(), QIcon::Normal, QIcon::Off);
        SettingsModbusRTU->setWindowIcon(icon);
        verticalLayout = new QVBoxLayout(SettingsModbusRTU);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        cmbRTS = new QComboBox(SettingsModbusRTU);
        cmbRTS->setObjectName(QString::fromUtf8("cmbRTS"));

        gridLayout->addWidget(cmbRTS, 7, 1, 1, 1);

        sbPort = new QSpinBox(SettingsModbusRTU);
        sbPort->setObjectName(QString::fromUtf8("sbPort"));
        sbPort->setMinimum(0);
        sbPort->setMaximum(128);
        sbPort->setValue(3);

        gridLayout->addWidget(sbPort, 1, 1, 1, 1);

        lblDev = new QLabel(SettingsModbusRTU);
        lblDev->setObjectName(QString::fromUtf8("lblDev"));

        gridLayout->addWidget(lblDev, 0, 0, 1, 1);

        label = new QLabel(SettingsModbusRTU);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 8, 0, 1, 1);

        sbByteTimeout = new QSpinBox(SettingsModbusRTU);
        sbByteTimeout->setObjectName(QString::fromUtf8("sbByteTimeout"));
        sbByteTimeout->setMaximum(1000);
        sbByteTimeout->setSingleStep(100);
        sbByteTimeout->setValue(500);

        gridLayout->addWidget(sbByteTimeout, 9, 1, 1, 1);

        lblDataBits = new QLabel(SettingsModbusRTU);
        lblDataBits->setObjectName(QString::fromUtf8("lblDataBits"));
        lblDataBits->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout->addWidget(lblDataBits, 3, 0, 1, 1);

        lblRTS = new QLabel(SettingsModbusRTU);
        lblRTS->setObjectName(QString::fromUtf8("lblRTS"));
        lblRTS->setText(QString::fromUtf8("RTS"));
        lblRTS->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout->addWidget(lblRTS, 7, 0, 1, 1);

        cmbStopBits = new QComboBox(SettingsModbusRTU);
        cmbStopBits->addItem(QString::fromUtf8("1"));
        cmbStopBits->addItem(QString::fromUtf8("1.5"));
        cmbStopBits->addItem(QString::fromUtf8("2"));
        cmbStopBits->setObjectName(QString::fromUtf8("cmbStopBits"));

        gridLayout->addWidget(cmbStopBits, 5, 1, 1, 1);

        lblStopBits = new QLabel(SettingsModbusRTU);
        lblStopBits->setObjectName(QString::fromUtf8("lblStopBits"));
        lblStopBits->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout->addWidget(lblStopBits, 5, 0, 1, 1);

        label_3 = new QLabel(SettingsModbusRTU);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 10, 0, 1, 1);

        label_2 = new QLabel(SettingsModbusRTU);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 9, 0, 1, 1);

        sbScanRate = new QSpinBox(SettingsModbusRTU);
        sbScanRate->setObjectName(QString::fromUtf8("sbScanRate"));
        sbScanRate->setMaximum(10000);
        sbScanRate->setSingleStep(10);
        sbScanRate->setValue(50);

        gridLayout->addWidget(sbScanRate, 10, 1, 1, 1);

        sbResponseTimeout = new QSpinBox(SettingsModbusRTU);
        sbResponseTimeout->setObjectName(QString::fromUtf8("sbResponseTimeout"));
        sbResponseTimeout->setMaximum(1000);
        sbResponseTimeout->setSingleStep(100);
        sbResponseTimeout->setValue(500);

        gridLayout->addWidget(sbResponseTimeout, 8, 1, 1, 1);

        cmbDataBits = new QComboBox(SettingsModbusRTU);
        cmbDataBits->addItem(QString::fromUtf8("7"));
        cmbDataBits->addItem(QString::fromUtf8("8"));
        cmbDataBits->setObjectName(QString::fromUtf8("cmbDataBits"));

        gridLayout->addWidget(cmbDataBits, 3, 1, 1, 1);

        lblPort = new QLabel(SettingsModbusRTU);
        lblPort->setObjectName(QString::fromUtf8("lblPort"));
        lblPort->setMinimumSize(QSize(0, 0));
        lblPort->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout->addWidget(lblPort, 1, 0, 1, 1);

        lblBaud = new QLabel(SettingsModbusRTU);
        lblBaud->setObjectName(QString::fromUtf8("lblBaud"));
        lblBaud->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout->addWidget(lblBaud, 2, 0, 1, 1);

        cmbDev = new QComboBox(SettingsModbusRTU);
        cmbDev->addItem(QString());
        cmbDev->addItem(QString());
        cmbDev->setObjectName(QString::fromUtf8("cmbDev"));
        cmbDev->setEditable(true);

        gridLayout->addWidget(cmbDev, 0, 1, 1, 1);

        cmbBaud = new QComboBox(SettingsModbusRTU);
        cmbBaud->addItem(QString::fromUtf8("110"));
        cmbBaud->addItem(QString::fromUtf8("300"));
        cmbBaud->addItem(QString::fromUtf8("600"));
        cmbBaud->addItem(QString::fromUtf8("1200"));
        cmbBaud->addItem(QString::fromUtf8("2400"));
        cmbBaud->addItem(QString::fromUtf8("4800"));
        cmbBaud->addItem(QString::fromUtf8("9600"));
        cmbBaud->addItem(QString::fromUtf8("14400"));
        cmbBaud->addItem(QString::fromUtf8("19200"));
        cmbBaud->addItem(QString::fromUtf8("28800"));
        cmbBaud->addItem(QString::fromUtf8("38400"));
        cmbBaud->addItem(QString::fromUtf8("57600"));
        cmbBaud->addItem(QString::fromUtf8("115200"));
        cmbBaud->addItem(QString::fromUtf8("128000"));
        cmbBaud->addItem(QString::fromUtf8("256000"));
        cmbBaud->addItem(QString::fromUtf8("921600"));
        cmbBaud->setObjectName(QString::fromUtf8("cmbBaud"));

        gridLayout->addWidget(cmbBaud, 2, 1, 1, 1);

        lblParity = new QLabel(SettingsModbusRTU);
        lblParity->setObjectName(QString::fromUtf8("lblParity"));
        lblParity->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout->addWidget(lblParity, 4, 0, 1, 1);

        cmbParity = new QComboBox(SettingsModbusRTU);
        cmbParity->addItem(QString());
        cmbParity->addItem(QString());
        cmbParity->addItem(QString());
        cmbParity->setObjectName(QString::fromUtf8("cmbParity"));

        gridLayout->addWidget(cmbParity, 4, 1, 1, 1);


        verticalLayout->addLayout(gridLayout);

        buttonBox = new QDialogButtonBox(SettingsModbusRTU);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);

#if QT_CONFIG(shortcut)
        lblDataBits->setBuddy(cmbDataBits);
        lblStopBits->setBuddy(cmbStopBits);
        lblBaud->setBuddy(cmbBaud);
        lblParity->setBuddy(cmbParity);
#endif // QT_CONFIG(shortcut)

        retranslateUi(SettingsModbusRTU);
        QObject::connect(buttonBox, SIGNAL(accepted()), SettingsModbusRTU, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), SettingsModbusRTU, SLOT(reject()));

        cmbStopBits->setCurrentIndex(0);
        cmbDataBits->setCurrentIndex(1);
        cmbBaud->setCurrentIndex(8);
        cmbParity->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(SettingsModbusRTU);
    } // setupUi

    void retranslateUi(QWidget *SettingsModbusRTU)
    {
        SettingsModbusRTU->setWindowTitle(QCoreApplication::translate("SettingsModbusRTU", "Modbus RTU Settings", nullptr));
        lblDev->setText(QCoreApplication::translate("SettingsModbusRTU", "Serial device", nullptr));
        label->setText(QCoreApplication::translate("SettingsModbusRTU", "Response Timeout (ms)", nullptr));
        lblDataBits->setText(QCoreApplication::translate("SettingsModbusRTU", "Data Bits", nullptr));

        lblStopBits->setText(QCoreApplication::translate("SettingsModbusRTU", "Stop Bits", nullptr));
        label_3->setText(QCoreApplication::translate("SettingsModbusRTU", "Scan Rate (ms)", nullptr));
        label_2->setText(QCoreApplication::translate("SettingsModbusRTU", "Byte Timeout (ms)", nullptr));

        lblPort->setText(QCoreApplication::translate("SettingsModbusRTU", "Serial port", nullptr));
        lblBaud->setText(QCoreApplication::translate("SettingsModbusRTU", "Baud", nullptr));
        cmbDev->setItemText(0, QCoreApplication::translate("SettingsModbusRTU", "/dev/ttyS", nullptr));
        cmbDev->setItemText(1, QCoreApplication::translate("SettingsModbusRTU", "/dev/ttyUSB", nullptr));


        lblParity->setText(QCoreApplication::translate("SettingsModbusRTU", "Parity", nullptr));
        cmbParity->setItemText(0, QCoreApplication::translate("SettingsModbusRTU", "None", nullptr));
        cmbParity->setItemText(1, QCoreApplication::translate("SettingsModbusRTU", "Odd", nullptr));
        cmbParity->setItemText(2, QCoreApplication::translate("SettingsModbusRTU", "Even", nullptr));

    } // retranslateUi

};

namespace Ui {
    class SettingsModbusRTU: public Ui_SettingsModbusRTU {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGSMODBUSRTU_H
