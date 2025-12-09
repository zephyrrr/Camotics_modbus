/********************************************************************************
** Form generated from reading UI file 'NCMachinePosition.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NCMACHINEPOSITION_H
#define UI_NCMACHINEPOSITION_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FormNCMachinePosition
{
public:
    QGridLayout *gridLayout_2;
    QLabel *label_13;
    QLabel *uLabel;
    QLabel *zLabel;
    QLabel *label_12;
    QLabel *label_10;
    QLabel *xLabel;
    QLabel *label_11;
    QLabel *yLabel;

    void setupUi(QWidget *NCMachinePosition)
    {
        if (NCMachinePosition->objectName().isEmpty())
            NCMachinePosition->setObjectName(QString::fromUtf8("NCMachinePosition"));
        gridLayout_2 = new QGridLayout(NCMachinePosition);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label_13 = new QLabel(NCMachinePosition);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_13->sizePolicy().hasHeightForWidth());
        label_13->setSizePolicy(sizePolicy);
        QFont font;
        font.setPointSize(20);
        font.setUnderline(true);
        label_13->setFont(font);
        label_13->setStyleSheet(QString::fromUtf8("color:red"));

        gridLayout_2->addWidget(label_13, 3, 0, 1, 1);

        uLabel = new QLabel(NCMachinePosition);
        uLabel->setObjectName(QString::fromUtf8("uLabel"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Courier 10 Pitch"));
        font1.setPointSize(20);
        uLabel->setFont(font1);
        uLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(uLabel, 3, 1, 1, 1);

        zLabel = new QLabel(NCMachinePosition);
        zLabel->setObjectName(QString::fromUtf8("zLabel"));
        zLabel->setFont(font1);
        zLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(zLabel, 2, 1, 1, 1);

        label_12 = new QLabel(NCMachinePosition);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        sizePolicy.setHeightForWidth(label_12->sizePolicy().hasHeightForWidth());
        label_12->setSizePolicy(sizePolicy);
        label_12->setFont(font);
        label_12->setStyleSheet(QString::fromUtf8("color:blue"));

        gridLayout_2->addWidget(label_12, 2, 0, 1, 1);

        label_10 = new QLabel(NCMachinePosition);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        sizePolicy.setHeightForWidth(label_10->sizePolicy().hasHeightForWidth());
        label_10->setSizePolicy(sizePolicy);
        label_10->setFont(font);
        label_10->setStyleSheet(QString::fromUtf8("color:red"));

        gridLayout_2->addWidget(label_10, 0, 0, 1, 1);

        xLabel = new QLabel(NCMachinePosition);
        xLabel->setObjectName(QString::fromUtf8("xLabel"));
        xLabel->setFont(font1);
        xLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(xLabel, 0, 1, 1, 1);

        label_11 = new QLabel(NCMachinePosition);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        sizePolicy.setHeightForWidth(label_11->sizePolicy().hasHeightForWidth());
        label_11->setSizePolicy(sizePolicy);
        label_11->setFont(font);
        label_11->setStyleSheet(QString::fromUtf8("color:green"));

        gridLayout_2->addWidget(label_11, 1, 0, 1, 1);

        yLabel = new QLabel(NCMachinePosition);
        yLabel->setObjectName(QString::fromUtf8("yLabel"));
        yLabel->setFont(font1);
        yLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(yLabel, 1, 1, 1, 1);

        gridLayout_2->setColumnStretch(1, 1);

        retranslateUi(NCMachinePosition);

        QMetaObject::connectSlotsByName(NCMachinePosition);
    } // setupUi

    void retranslateUi(QWidget *NCMachinePosition)
    {
        label_13->setText(QCoreApplication::translate("FormNCMachinePosition", "U", nullptr));
#if QT_CONFIG(tooltip)
        uLabel->setToolTip(QCoreApplication::translate("FormNCMachinePosition", "Current U position", nullptr));
#endif // QT_CONFIG(tooltip)
        uLabel->setText(QCoreApplication::translate("FormNCMachinePosition", "0.0", nullptr));
#if QT_CONFIG(tooltip)
        zLabel->setToolTip(QCoreApplication::translate("FormNCMachinePosition", "Current Z position", nullptr));
#endif // QT_CONFIG(tooltip)
        zLabel->setText(QCoreApplication::translate("FormNCMachinePosition", "0.0", nullptr));
        label_12->setText(QCoreApplication::translate("FormNCMachinePosition", "Z", nullptr));
        label_10->setText(QCoreApplication::translate("FormNCMachinePosition", "X", nullptr));
#if QT_CONFIG(tooltip)
        xLabel->setToolTip(QCoreApplication::translate("FormNCMachinePosition", "Current X position", nullptr));
#endif // QT_CONFIG(tooltip)
        xLabel->setText(QCoreApplication::translate("FormNCMachinePosition", "0.0", nullptr));
        label_11->setText(QCoreApplication::translate("FormNCMachinePosition", "Y", nullptr));
#if QT_CONFIG(tooltip)
        yLabel->setToolTip(QCoreApplication::translate("FormNCMachinePosition", "Current Y position", nullptr));
#endif // QT_CONFIG(tooltip)
        yLabel->setText(QCoreApplication::translate("FormNCMachinePosition", "0.0", nullptr));
        (void)NCMachinePosition;
    } // retranslateUi

};

namespace Ui {
    class FormNCMachinePosition: public Ui_FormNCMachinePosition {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NCMACHINEPOSITION_H
