/********************************************************************************
** Form generated from reading UI file 'widget_cp_ln.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_CP_LN_H
#define UI_WIDGET_CP_LN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CPLNWidget
{
public:
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QRadioButton *radioButton;
    QRadioButton *radioButton_2;
    QRadioButton *radioButton_3;
    QPushButton *btnOk;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QString::fromUtf8("Dialog"));
        Dialog->resize(340, 190);
        verticalLayoutWidget = new QWidget(Dialog);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(0, 0, 331, 141));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(verticalLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout->addWidget(label);

        radioButton = new QRadioButton(verticalLayoutWidget);
        radioButton->setObjectName(QString::fromUtf8("radioButton"));

        verticalLayout->addWidget(radioButton);

        radioButton_2 = new QRadioButton(verticalLayoutWidget);
        radioButton_2->setObjectName(QString::fromUtf8("radioButton_2"));

        verticalLayout->addWidget(radioButton_2);

        radioButton_3 = new QRadioButton(verticalLayoutWidget);
        radioButton_3->setObjectName(QString::fromUtf8("radioButton_3"));

        verticalLayout->addWidget(radioButton_3);

        btnOk = new QPushButton(Dialog);
        btnOk->setObjectName(QString::fromUtf8("btnOk"));
        btnOk->setGeometry(QRect(60, 160, 80, 24));

        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QCoreApplication::translate("CPLNWidget", "Form", nullptr));
        label->setText(QCoreApplication::translate("CPLNWidget", "\346\221\207\345\212\250\344\274\272\346\234\215\346\250\241\345\274\217", nullptr));
        radioButton->setText(QCoreApplication::translate("CPLNWidget", "\350\277\233\347\273\231\345\222\214\345\233\236\351\200\200,\344\274\272\346\234\215\350\275\264\345\222\214\346\221\207\345\212\250\350\275\264\350\201\224\345\212\250\344\274\272\346\234\215", nullptr));
        radioButton_2->setText(QCoreApplication::translate("CPLNWidget", "\345\233\236\351\200\200\346\227\266,\344\274\272\346\234\215\350\275\264\346\255\243\345\270\270\351\200\237\345\272\246\343\200\201\346\221\207\345\212\250\345\201\234\345\234\250\345\216\237\345\234\260\344\270\215\345\212\250\357\274\233\350\277\233\347\273\231\346\227\266\350\201\224\345\212\250", nullptr));
        radioButton_3->setText(QCoreApplication::translate("CPLNWidget", "\344\270\200\346\227\246\346\243\200\346\265\213\345\210\260\345\233\236\351\200\200,\346\221\207\345\212\250\345\220\257\345\212\250\345\233\236\344\270\255\345\277\203", nullptr));
        btnOk->setText(QCoreApplication::translate("CPLNWidget", "\347\241\256\345\256\232", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CPLNWidget: public Ui_CPLNWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_CP_LN_H
