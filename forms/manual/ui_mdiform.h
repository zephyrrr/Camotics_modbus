/********************************************************************************
** Form generated from reading UI file 'mdiform.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MDIFORM_H
#define UI_MDIFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MdiForm
{
public:
    QVBoxLayout *verticalLayout;
    QVBoxLayout *editorLayout;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *btnKeyboard;

    void setupUi(QWidget *MdiForm)
    {
        if (MdiForm->objectName().isEmpty())
            MdiForm->setObjectName(QString::fromUtf8("MdiForm"));
        MdiForm->resize(637, 654);
        verticalLayout = new QVBoxLayout(MdiForm);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        editorLayout = new QVBoxLayout();
        editorLayout->setObjectName(QString::fromUtf8("editorLayout"));

        verticalLayout->addLayout(editorLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        btnKeyboard = new QPushButton(MdiForm);
        btnKeyboard->setObjectName(QString::fromUtf8("btnKeyboard"));
        btnKeyboard->setCheckable(true);

        horizontalLayout_2->addWidget(btnKeyboard);


        verticalLayout->addLayout(horizontalLayout_2);


        retranslateUi(MdiForm);

        QMetaObject::connectSlotsByName(MdiForm);
    } // setupUi

    void retranslateUi(QWidget *MdiForm)
    {
        MdiForm->setWindowTitle(QCoreApplication::translate("MdiForm", "Form", nullptr));
        btnKeyboard->setText(QCoreApplication::translate("MdiForm", "JP", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MdiForm: public Ui_MdiForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MDIFORM_H
