/********************************************************************************
** Form generated from reading UI file 'luojubuchangform.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LUOJUBUCHANGFORM_H
#define UI_LUOJUBUCHANGFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LuoJuBuChangForm
{
public:
    QVBoxLayout *verticalLayout1;
    QVBoxLayout *verticalLayoutTable;
    QHBoxLayout *horizontalLayout;
    QPushButton *btnExport;
    QPushButton *btnImport;
    QSpacerItem *horizontalSpacer;

    void setupUi(QWidget *LuoJuBuChangForm)
    {
        if (LuoJuBuChangForm->objectName().isEmpty())
            LuoJuBuChangForm->setObjectName(QString::fromUtf8("LuoJuBuChangForm"));
        LuoJuBuChangForm->resize(637, 654);
        verticalLayout1 = new QVBoxLayout(LuoJuBuChangForm);
        verticalLayout1->setObjectName(QString::fromUtf8("verticalLayout1"));
        verticalLayoutTable = new QVBoxLayout();
        verticalLayoutTable->setObjectName(QString::fromUtf8("verticalLayoutTable"));

        verticalLayout1->addLayout(verticalLayoutTable);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);
        btnExport = new QPushButton(LuoJuBuChangForm);
        btnExport->setObjectName(QString::fromUtf8("btnExport"));

        horizontalLayout->addWidget(btnExport);

        btnImport = new QPushButton(LuoJuBuChangForm);
        btnImport->setObjectName(QString::fromUtf8("btnImport"));

        horizontalLayout->addWidget(btnImport);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout1->addLayout(horizontalLayout);


        retranslateUi(LuoJuBuChangForm);

        QMetaObject::connectSlotsByName(LuoJuBuChangForm);
    } // setupUi

    void retranslateUi(QWidget *LuoJuBuChangForm)
    {
        LuoJuBuChangForm->setWindowTitle(QCoreApplication::translate("LuoJuBuChangForm", "Form", nullptr));
        btnExport->setText(QCoreApplication::translate("LuoJuBuChangForm", "DC", nullptr));
        btnImport->setText(QCoreApplication::translate("LuoJuBuChangForm", "DR", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LuoJuBuChangForm: public Ui_LuoJuBuChangForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LUOJUBUCHANGFORM_H
