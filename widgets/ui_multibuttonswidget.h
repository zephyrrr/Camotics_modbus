/********************************************************************************
** Form generated from reading UI file 'multibuttonswidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MULTIBUTTONSWIDGET_H
#define UI_MULTIBUTTONSWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MultiButtonsWidgetClass
{
public:
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;

    void setupUi(QWidget *MultiButtonsWidgetClass)
    {
        if (MultiButtonsWidgetClass->objectName().isEmpty())
            MultiButtonsWidgetClass->setObjectName(QString::fromUtf8("MultiButtonsWidgetClass"));
        MultiButtonsWidgetClass->resize(600, 400);
        horizontalLayout = new QHBoxLayout(MultiButtonsWidgetClass);
        horizontalLayout->setSpacing(2);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        retranslateUi(MultiButtonsWidgetClass);

        QMetaObject::connectSlotsByName(MultiButtonsWidgetClass);
    } // setupUi

    void retranslateUi(QWidget *MultiButtonsWidgetClass)
    {
        MultiButtonsWidgetClass->setWindowTitle(QCoreApplication::translate("MultiButtonsWidgetClass", "MultiButtonsWidget", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MultiButtonsWidgetClass: public Ui_MultiButtonsWidgetClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MULTIBUTTONSWIDGET_H
