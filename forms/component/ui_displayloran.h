/********************************************************************************
** Form generated from reading UI file 'displayloran.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DISPLAYLORAN_H
#define UI_DISPLAYLORAN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_displayloranClass
{
public:
    QHBoxLayout *horizontalLayout;
    QGraphicsView *graphicsView;

    void setupUi(QWidget *displayloranClass)
    {
        if (displayloranClass->objectName().isEmpty())
            displayloranClass->setObjectName(QString::fromUtf8("displayloranClass"));
        displayloranClass->resize(100, 100);
        horizontalLayout = new QHBoxLayout(displayloranClass);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        graphicsView = new QGraphicsView(displayloranClass);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));

        horizontalLayout->addWidget(graphicsView);


        retranslateUi(displayloranClass);

        QMetaObject::connectSlotsByName(displayloranClass);
    } // setupUi

    void retranslateUi(QWidget *displayloranClass)
    {
        displayloranClass->setWindowTitle(QCoreApplication::translate("displayloranClass", "displayloran", nullptr));
    } // retranslateUi

};

namespace Ui {
    class displayloranClass: public Ui_displayloranClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DISPLAYLORAN_H
