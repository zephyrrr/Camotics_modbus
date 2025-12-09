/********************************************************************************
** Form generated from reading UI file 'fangdiancanshuform.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FANGDIANCANSHUFORM_H
#define UI_FANGDIANCANSHUFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FangDianCanShuForm
{
public:
    QVBoxLayout *verticalLayout1;
    QVBoxLayout *verticalLayoutTable;
    QHBoxLayout *horizontalLayout;
    QPushButton *btnExport;
    QPushButton *btnImport;
    QSpacerItem *horizontalSpacer;

    void setupUi(QWidget *FangDianCanShuForm)
    {
        if (FangDianCanShuForm->objectName().isEmpty())
            FangDianCanShuForm->setObjectName(QString::fromUtf8("FangDianCanShuForm"));
        FangDianCanShuForm->resize(637, 654);
        verticalLayout1 = new QVBoxLayout(FangDianCanShuForm);
        verticalLayout1->setObjectName(QString::fromUtf8("verticalLayout1"));
        verticalLayoutTable = new QVBoxLayout();
        verticalLayoutTable->setObjectName(QString::fromUtf8("verticalLayoutTable"));

        verticalLayout1->addLayout(verticalLayoutTable);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);
        btnExport = new QPushButton(FangDianCanShuForm);
        btnExport->setObjectName(QString::fromUtf8("btnExport"));

        horizontalLayout->addWidget(btnExport);

        btnImport = new QPushButton(FangDianCanShuForm);
        btnImport->setObjectName(QString::fromUtf8("btnImport"));

        horizontalLayout->addWidget(btnImport);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout1->addLayout(horizontalLayout);


        retranslateUi(FangDianCanShuForm);

        QMetaObject::connectSlotsByName(FangDianCanShuForm);
    } // setupUi

    void retranslateUi(QWidget *FangDianCanShuForm)
    {
        FangDianCanShuForm->setWindowTitle(QCoreApplication::translate("FangDianCanShuForm", "Form", nullptr));
        btnExport->setText(QCoreApplication::translate("FangDianCanShuForm", "\345\257\274\345\207\272", nullptr));
        btnImport->setText(QCoreApplication::translate("FangDianCanShuForm", "\345\257\274\345\205\245", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FangDianCanShuForm: public Ui_FangDianCanShuForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FANGDIANCANSHUFORM_H
