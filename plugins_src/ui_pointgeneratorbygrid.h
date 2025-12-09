/********************************************************************************
** Form generated from reading UI file 'pointgeneratorbygrid.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_POINTGENERATORBYGRID_H
#define UI_POINTGENERATORBYGRID_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_PointGenratorByGrid
{
public:
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QLabel *label_5;
    QLabel *label_1;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_2;
    QLabel *label_6;
    QLineEdit *txtStartX;
    QLineEdit *txtStepX;
    QLineEdit *txtEndX;
    QLineEdit *txtStartY;
    QLineEdit *txtStepY;
    QLineEdit *txtEndY;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacerItem;
    QPushButton *okButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *PointGenratorByGrid)
    {
        if (PointGenratorByGrid->objectName().isEmpty())
            PointGenratorByGrid->setObjectName(QString::fromUtf8("PointGenratorByGrid"));
        PointGenratorByGrid->resize(400, 300);
        verticalLayout = new QVBoxLayout(PointGenratorByGrid);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_5 = new QLabel(PointGenratorByGrid);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout->addWidget(label_5, 5, 0, 1, 1);

        label_1 = new QLabel(PointGenratorByGrid);
        label_1->setObjectName(QString::fromUtf8("label_1"));

        gridLayout->addWidget(label_1, 0, 0, 1, 1);

        label_3 = new QLabel(PointGenratorByGrid);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        label_4 = new QLabel(PointGenratorByGrid);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 3, 0, 1, 1);

        label_2 = new QLabel(PointGenratorByGrid);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        label_6 = new QLabel(PointGenratorByGrid);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout->addWidget(label_6, 4, 0, 1, 1);

        txtStartX = new QLineEdit(PointGenratorByGrid);
        txtStartX->setObjectName(QString::fromUtf8("txtStartX"));

        gridLayout->addWidget(txtStartX, 0, 1, 1, 1);

        txtStepX = new QLineEdit(PointGenratorByGrid);
        txtStepX->setObjectName(QString::fromUtf8("txtStepX"));

        gridLayout->addWidget(txtStepX, 1, 1, 1, 1);

        txtEndX = new QLineEdit(PointGenratorByGrid);
        txtEndX->setObjectName(QString::fromUtf8("txtEndX"));

        gridLayout->addWidget(txtEndX, 2, 1, 1, 1);

        txtStartY = new QLineEdit(PointGenratorByGrid);
        txtStartY->setObjectName(QString::fromUtf8("txtStartY"));

        gridLayout->addWidget(txtStartY, 3, 1, 1, 1);

        txtStepY = new QLineEdit(PointGenratorByGrid);
        txtStepY->setObjectName(QString::fromUtf8("txtStepY"));

        gridLayout->addWidget(txtStepY, 4, 1, 1, 1);

        txtEndY = new QLineEdit(PointGenratorByGrid);
        txtEndY->setObjectName(QString::fromUtf8("txtEndY"));

        gridLayout->addWidget(txtEndY, 5, 1, 1, 1);


        verticalLayout->addLayout(gridLayout);

        hboxLayout = new QHBoxLayout();
#ifndef Q_OS_MAC
        hboxLayout->setSpacing(6);
#endif
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        spacerItem = new QSpacerItem(131, 31, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacerItem);

        okButton = new QPushButton(PointGenratorByGrid);
        okButton->setObjectName(QString::fromUtf8("okButton"));

        hboxLayout->addWidget(okButton);

        cancelButton = new QPushButton(PointGenratorByGrid);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        hboxLayout->addWidget(cancelButton);


        verticalLayout->addLayout(hboxLayout);


        retranslateUi(PointGenratorByGrid);

        QMetaObject::connectSlotsByName(PointGenratorByGrid);
    } // setupUi

    void retranslateUi(QDialog *PointGenratorByGrid)
    {
        PointGenratorByGrid->setWindowTitle(QCoreApplication::translate("PointGenratorByGrid", "\346\240\274\345\255\2201", nullptr));
        label_5->setText(QCoreApplication::translate("PointGenratorByGrid", "\347\273\223\346\235\237\347\202\271Y", nullptr));
        label_1->setText(QCoreApplication::translate("PointGenratorByGrid", "\345\274\200\345\247\213\347\202\271X", nullptr));
        label_3->setText(QCoreApplication::translate("PointGenratorByGrid", "\347\273\223\346\235\237\347\202\271X", nullptr));
        label_4->setText(QCoreApplication::translate("PointGenratorByGrid", "\345\274\200\345\247\213\347\202\271X", nullptr));
        label_2->setText(QCoreApplication::translate("PointGenratorByGrid", "\346\255\245\351\225\277X", nullptr));
        label_6->setText(QCoreApplication::translate("PointGenratorByGrid", "\346\255\245\351\225\277X", nullptr));
        txtStartX->setText(QCoreApplication::translate("PointGenratorByGrid", "1", nullptr));
        txtStepX->setText(QCoreApplication::translate("PointGenratorByGrid", "2", nullptr));
        txtEndX->setText(QCoreApplication::translate("PointGenratorByGrid", "9", nullptr));
        txtStartY->setText(QCoreApplication::translate("PointGenratorByGrid", "1", nullptr));
        txtStepY->setText(QCoreApplication::translate("PointGenratorByGrid", "3", nullptr));
        txtEndY->setText(QCoreApplication::translate("PointGenratorByGrid", "7", nullptr));
        okButton->setText(QCoreApplication::translate("PointGenratorByGrid", "OK", nullptr));
        cancelButton->setText(QCoreApplication::translate("PointGenratorByGrid", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PointGenratorByGrid: public Ui_PointGenratorByGrid {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_POINTGENERATORBYGRID_H
