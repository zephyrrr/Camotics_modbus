/********************************************************************************
** Form generated from reading UI file 'ncrun.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NCRUN_H
#define UI_NCRUN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_NcRunForm
{
public:
    QVBoxLayout *verticalLayout;

    void setupUi(QWidget *NcRunForm)
    {
        if (NcRunForm->objectName().isEmpty())
            NcRunForm->setObjectName(QString::fromUtf8("NcRunForm"));
        NcRunForm->resize(915, 486);
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(NcRunForm->sizePolicy().hasHeightForWidth());
        NcRunForm->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(NcRunForm);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));

        retranslateUi(NcRunForm);

        QMetaObject::connectSlotsByName(NcRunForm);
    } // setupUi

    void retranslateUi(QWidget *NcRunForm)
    {
        NcRunForm->setWindowTitle(QCoreApplication::translate("NcRunForm", "NcRun", nullptr));
    } // retranslateUi

};

namespace Ui {
    class NcRunForm: public Ui_NcRunForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NCRUN_H
