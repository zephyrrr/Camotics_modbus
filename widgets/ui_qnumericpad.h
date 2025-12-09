/********************************************************************************
** Form generated from reading UI file 'qnumericpad.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QNUMERICPAD_H
#define UI_QNUMERICPAD_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_QNumericPadClass
{
public:
    QGridLayout *gridLayout;
    QPushButton *pushButtonKey8;
    QPushButton *pushButtonKeySub;
    QPushButton *pushButtonKeyDot;
    QPushButton *pushButtonKeyHalf;
    QPushButton *pushButtonKey1;
    QPushButton *pushButtonKey4;
    QPushButton *pushButtonKeyAdd;
    QPushButton *pushButtonKey3;
    QPushButton *pushButtonKey2;
    QPushButton *pushButtonKey6;
    QPushButton *pushButtonKey7;
    QPushButton *pushButtonKey5;
    QPushButton *pushButtonKey0;
    QPushButton *pushButtonKey9;
    QPushButton *pushButtonKeyBack;
    QPushButton *pushButtonKeyDel;
    QPushButton *pushButtonKeyEnter;
    QPushButton *pushButtonKeyCancel;

    void setupUi(QDialog *QNumericPadClass)
    {
        if (QNumericPadClass->objectName().isEmpty())
            QNumericPadClass->setObjectName(QString::fromUtf8("QNumericPadClass"));
        QNumericPadClass->resize(380, 400);
        gridLayout = new QGridLayout(QNumericPadClass);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        pushButtonKey8 = new QPushButton(QNumericPadClass);
        pushButtonKey8->setObjectName(QString::fromUtf8("pushButtonKey8"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pushButtonKey8->sizePolicy().hasHeightForWidth());
        pushButtonKey8->setSizePolicy(sizePolicy);
        pushButtonKey8->setMinimumSize(QSize(10, 10));
        pushButtonKey8->setFocusPolicy(Qt::NoFocus);

        gridLayout->addWidget(pushButtonKey8, 1, 1, 1, 1);

        pushButtonKeySub = new QPushButton(QNumericPadClass);
        pushButtonKeySub->setObjectName(QString::fromUtf8("pushButtonKeySub"));
        sizePolicy.setHeightForWidth(pushButtonKeySub->sizePolicy().hasHeightForWidth());
        pushButtonKeySub->setSizePolicy(sizePolicy);
        pushButtonKeySub->setMinimumSize(QSize(10, 10));
        pushButtonKeySub->setFocusPolicy(Qt::NoFocus);

        gridLayout->addWidget(pushButtonKeySub, 4, 2, 1, 1);

        pushButtonKeyDot = new QPushButton(QNumericPadClass);
        pushButtonKeyDot->setObjectName(QString::fromUtf8("pushButtonKeyDot"));
        sizePolicy.setHeightForWidth(pushButtonKeyDot->sizePolicy().hasHeightForWidth());
        pushButtonKeyDot->setSizePolicy(sizePolicy);
        pushButtonKeyDot->setMinimumSize(QSize(10, 10));
        pushButtonKeyDot->setFocusPolicy(Qt::NoFocus);

        gridLayout->addWidget(pushButtonKeyDot, 5, 0, 1, 1);

        pushButtonKeyHalf = new QPushButton(QNumericPadClass);
        pushButtonKeyHalf->setObjectName(QString::fromUtf8("pushButtonKeyHalf"));
        sizePolicy.setHeightForWidth(pushButtonKeyHalf->sizePolicy().hasHeightForWidth());
        pushButtonKeyHalf->setSizePolicy(sizePolicy);
        pushButtonKeyHalf->setMinimumSize(QSize(10, 10));
        pushButtonKeyHalf->setFocusPolicy(Qt::NoFocus);
        pushButtonKeyHalf->setLayoutDirection(Qt::LeftToRight);

        gridLayout->addWidget(pushButtonKeyHalf, 5, 1, 1, 1);

        pushButtonKey1 = new QPushButton(QNumericPadClass);
        pushButtonKey1->setObjectName(QString::fromUtf8("pushButtonKey1"));
        sizePolicy.setHeightForWidth(pushButtonKey1->sizePolicy().hasHeightForWidth());
        pushButtonKey1->setSizePolicy(sizePolicy);
        pushButtonKey1->setMinimumSize(QSize(10, 10));
        pushButtonKey1->setFocusPolicy(Qt::NoFocus);

        gridLayout->addWidget(pushButtonKey1, 3, 0, 1, 1);

        pushButtonKey4 = new QPushButton(QNumericPadClass);
        pushButtonKey4->setObjectName(QString::fromUtf8("pushButtonKey4"));
        sizePolicy.setHeightForWidth(pushButtonKey4->sizePolicy().hasHeightForWidth());
        pushButtonKey4->setSizePolicy(sizePolicy);
        pushButtonKey4->setMinimumSize(QSize(10, 10));
        pushButtonKey4->setFocusPolicy(Qt::NoFocus);

        gridLayout->addWidget(pushButtonKey4, 2, 0, 1, 1);

        pushButtonKeyAdd = new QPushButton(QNumericPadClass);
        pushButtonKeyAdd->setObjectName(QString::fromUtf8("pushButtonKeyAdd"));
        sizePolicy.setHeightForWidth(pushButtonKeyAdd->sizePolicy().hasHeightForWidth());
        pushButtonKeyAdd->setSizePolicy(sizePolicy);
        pushButtonKeyAdd->setMinimumSize(QSize(10, 10));
        pushButtonKeyAdd->setFocusPolicy(Qt::NoFocus);

        gridLayout->addWidget(pushButtonKeyAdd, 4, 1, 1, 1);

        pushButtonKey3 = new QPushButton(QNumericPadClass);
        pushButtonKey3->setObjectName(QString::fromUtf8("pushButtonKey3"));
        sizePolicy.setHeightForWidth(pushButtonKey3->sizePolicy().hasHeightForWidth());
        pushButtonKey3->setSizePolicy(sizePolicy);
        pushButtonKey3->setMinimumSize(QSize(10, 10));
        pushButtonKey3->setFocusPolicy(Qt::NoFocus);

        gridLayout->addWidget(pushButtonKey3, 3, 2, 1, 1);

        pushButtonKey2 = new QPushButton(QNumericPadClass);
        pushButtonKey2->setObjectName(QString::fromUtf8("pushButtonKey2"));
        sizePolicy.setHeightForWidth(pushButtonKey2->sizePolicy().hasHeightForWidth());
        pushButtonKey2->setSizePolicy(sizePolicy);
        pushButtonKey2->setMinimumSize(QSize(10, 10));
        pushButtonKey2->setFocusPolicy(Qt::NoFocus);

        gridLayout->addWidget(pushButtonKey2, 3, 1, 1, 1);

        pushButtonKey6 = new QPushButton(QNumericPadClass);
        pushButtonKey6->setObjectName(QString::fromUtf8("pushButtonKey6"));
        sizePolicy.setHeightForWidth(pushButtonKey6->sizePolicy().hasHeightForWidth());
        pushButtonKey6->setSizePolicy(sizePolicy);
        pushButtonKey6->setMinimumSize(QSize(10, 10));
        pushButtonKey6->setFocusPolicy(Qt::NoFocus);

        gridLayout->addWidget(pushButtonKey6, 2, 2, 1, 1);

        pushButtonKey7 = new QPushButton(QNumericPadClass);
        pushButtonKey7->setObjectName(QString::fromUtf8("pushButtonKey7"));
        sizePolicy.setHeightForWidth(pushButtonKey7->sizePolicy().hasHeightForWidth());
        pushButtonKey7->setSizePolicy(sizePolicy);
        pushButtonKey7->setMinimumSize(QSize(10, 10));
        pushButtonKey7->setFocusPolicy(Qt::NoFocus);

        gridLayout->addWidget(pushButtonKey7, 1, 0, 1, 1);

        pushButtonKey5 = new QPushButton(QNumericPadClass);
        pushButtonKey5->setObjectName(QString::fromUtf8("pushButtonKey5"));
        sizePolicy.setHeightForWidth(pushButtonKey5->sizePolicy().hasHeightForWidth());
        pushButtonKey5->setSizePolicy(sizePolicy);
        pushButtonKey5->setMinimumSize(QSize(10, 10));
        pushButtonKey5->setFocusPolicy(Qt::NoFocus);

        gridLayout->addWidget(pushButtonKey5, 2, 1, 1, 1);

        pushButtonKey0 = new QPushButton(QNumericPadClass);
        pushButtonKey0->setObjectName(QString::fromUtf8("pushButtonKey0"));
        sizePolicy.setHeightForWidth(pushButtonKey0->sizePolicy().hasHeightForWidth());
        pushButtonKey0->setSizePolicy(sizePolicy);
        pushButtonKey0->setMinimumSize(QSize(10, 10));
        pushButtonKey0->setFocusPolicy(Qt::NoFocus);

        gridLayout->addWidget(pushButtonKey0, 4, 0, 1, 1);

        pushButtonKey9 = new QPushButton(QNumericPadClass);
        pushButtonKey9->setObjectName(QString::fromUtf8("pushButtonKey9"));
        sizePolicy.setHeightForWidth(pushButtonKey9->sizePolicy().hasHeightForWidth());
        pushButtonKey9->setSizePolicy(sizePolicy);
        pushButtonKey9->setMinimumSize(QSize(10, 10));
        pushButtonKey9->setFocusPolicy(Qt::NoFocus);

        gridLayout->addWidget(pushButtonKey9, 1, 2, 1, 1);

        pushButtonKeyBack = new QPushButton(QNumericPadClass);
        pushButtonKeyBack->setObjectName(QString::fromUtf8("pushButtonKeyBack"));
        sizePolicy.setHeightForWidth(pushButtonKeyBack->sizePolicy().hasHeightForWidth());
        pushButtonKeyBack->setSizePolicy(sizePolicy);
        pushButtonKeyBack->setMinimumSize(QSize(10, 10));
        pushButtonKeyBack->setFocusPolicy(Qt::NoFocus);

        gridLayout->addWidget(pushButtonKeyBack, 5, 2, 1, 1);

        pushButtonKeyDel = new QPushButton(QNumericPadClass);
        pushButtonKeyDel->setObjectName(QString::fromUtf8("pushButtonKeyDel"));
        sizePolicy.setHeightForWidth(pushButtonKeyDel->sizePolicy().hasHeightForWidth());
        pushButtonKeyDel->setSizePolicy(sizePolicy);
        pushButtonKeyDel->setMinimumSize(QSize(10, 10));
        pushButtonKeyDel->setFocusPolicy(Qt::NoFocus);

        gridLayout->addWidget(pushButtonKeyDel, 1, 3, 1, 1);

        pushButtonKeyEnter = new QPushButton(QNumericPadClass);
        pushButtonKeyEnter->setObjectName(QString::fromUtf8("pushButtonKeyEnter"));
        sizePolicy.setHeightForWidth(pushButtonKeyEnter->sizePolicy().hasHeightForWidth());
        pushButtonKeyEnter->setSizePolicy(sizePolicy);
        pushButtonKeyEnter->setMinimumSize(QSize(10, 10));
        pushButtonKeyEnter->setFocusPolicy(Qt::NoFocus);

        gridLayout->addWidget(pushButtonKeyEnter, 4, 3, 2, 1);

        pushButtonKeyCancel = new QPushButton(QNumericPadClass);
        pushButtonKeyCancel->setObjectName(QString::fromUtf8("pushButtonKeyCancel"));
        sizePolicy.setHeightForWidth(pushButtonKeyCancel->sizePolicy().hasHeightForWidth());
        pushButtonKeyCancel->setSizePolicy(sizePolicy);
        pushButtonKeyCancel->setMinimumSize(QSize(10, 10));
        pushButtonKeyCancel->setFocusPolicy(Qt::NoFocus);

        gridLayout->addWidget(pushButtonKeyCancel, 2, 3, 2, 1);


        retranslateUi(QNumericPadClass);

        QMetaObject::connectSlotsByName(QNumericPadClass);
    } // setupUi

    void retranslateUi(QDialog *QNumericPadClass)
    {
        QNumericPadClass->setWindowTitle(QCoreApplication::translate("QNumericPadClass", "\351\224\256\347\233\230", nullptr));
        pushButtonKey8->setText(QCoreApplication::translate("QNumericPadClass", "8", nullptr));
        pushButtonKeySub->setText(QCoreApplication::translate("QNumericPadClass", "-", nullptr));
        pushButtonKeyDot->setText(QCoreApplication::translate("QNumericPadClass", ".", nullptr));
        pushButtonKeyHalf->setText(QCoreApplication::translate("QNumericPadClass", "+/-", nullptr));
        pushButtonKey1->setText(QCoreApplication::translate("QNumericPadClass", "1", nullptr));
        pushButtonKey4->setText(QCoreApplication::translate("QNumericPadClass", "4", nullptr));
        pushButtonKeyAdd->setText(QCoreApplication::translate("QNumericPadClass", "+", nullptr));
        pushButtonKey3->setText(QCoreApplication::translate("QNumericPadClass", "3", nullptr));
        pushButtonKey2->setText(QCoreApplication::translate("QNumericPadClass", "2", nullptr));
        pushButtonKey6->setText(QCoreApplication::translate("QNumericPadClass", "6", nullptr));
        pushButtonKey7->setText(QCoreApplication::translate("QNumericPadClass", "7", nullptr));
        pushButtonKey5->setText(QCoreApplication::translate("QNumericPadClass", "5", nullptr));
        pushButtonKey0->setText(QCoreApplication::translate("QNumericPadClass", "0", nullptr));
        pushButtonKey9->setText(QCoreApplication::translate("QNumericPadClass", "9", nullptr));
        pushButtonKeyBack->setText(QCoreApplication::translate("QNumericPadClass", "\345\233\236\351\200\200", nullptr));
        pushButtonKeyDel->setText(QCoreApplication::translate("QNumericPadClass", "\346\270\205\347\251\272", nullptr));
        pushButtonKeyEnter->setText(QCoreApplication::translate("QNumericPadClass", "\347\241\256\350\256\244", nullptr));
        pushButtonKeyCancel->setText(QCoreApplication::translate("QNumericPadClass", "\345\217\226\346\266\210", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QNumericPadClass: public Ui_QNumericPadClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QNUMERICPAD_H
