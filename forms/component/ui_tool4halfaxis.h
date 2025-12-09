/********************************************************************************
** Form generated from reading UI file 'tool4halfaxis.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TOOL4HALFAXIS_H
#define UI_TOOL4HALFAXIS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Tool4HalfAxisClass
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *btnSetXHalf;
    QPushButton *btnSetYHalf;
    QPushButton *btnSetZHalf;

    void setupUi(QWidget *Tool4HalfAxisClass)
    {
        if (Tool4HalfAxisClass->objectName().isEmpty())
            Tool4HalfAxisClass->setObjectName(QString::fromUtf8("Tool4HalfAxisClass"));
        Tool4HalfAxisClass->resize(400, 200);
        Tool4HalfAxisClass->setStyleSheet(QString::fromUtf8(""));
        verticalLayout = new QVBoxLayout(Tool4HalfAxisClass);
        verticalLayout->setSpacing(5);
        verticalLayout->setContentsMargins(5, 5, 5, 5);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(5);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        btnSetXHalf = new QPushButton(Tool4HalfAxisClass);
        btnSetXHalf->setObjectName(QString::fromUtf8("btnSetXHalf"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(btnSetXHalf->sizePolicy().hasHeightForWidth());
        btnSetXHalf->setSizePolicy(sizePolicy);

        horizontalLayout_2->addWidget(btnSetXHalf);

        btnSetYHalf = new QPushButton(Tool4HalfAxisClass);
        btnSetYHalf->setObjectName(QString::fromUtf8("btnSetYHalf"));
        sizePolicy.setHeightForWidth(btnSetYHalf->sizePolicy().hasHeightForWidth());
        btnSetYHalf->setSizePolicy(sizePolicy);

        horizontalLayout_2->addWidget(btnSetYHalf);

        btnSetZHalf = new QPushButton(Tool4HalfAxisClass);
        btnSetZHalf->setObjectName(QString::fromUtf8("btnSetZHalf"));
        sizePolicy.setHeightForWidth(btnSetZHalf->sizePolicy().hasHeightForWidth());
        btnSetZHalf->setSizePolicy(sizePolicy);

        horizontalLayout_2->addWidget(btnSetZHalf);


        verticalLayout->addLayout(horizontalLayout_2);


        retranslateUi(Tool4HalfAxisClass);

        QMetaObject::connectSlotsByName(Tool4HalfAxisClass);
    } // setupUi

    void retranslateUi(QWidget *Tool4HalfAxisClass)
    {
        Tool4HalfAxisClass->setWindowTitle(QCoreApplication::translate("Tool4HalfAxisClass", "Tool4HalfAxis", nullptr));
        btnSetXHalf->setText(QCoreApplication::translate("Tool4HalfAxisClass", "X 1/2", nullptr));
        btnSetYHalf->setText(QCoreApplication::translate("Tool4HalfAxisClass", "Y 1/2", nullptr));
        btnSetZHalf->setText(QCoreApplication::translate("Tool4HalfAxisClass", "Z 1/2", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Tool4HalfAxisClass: public Ui_Tool4HalfAxisClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TOOL4HALFAXIS_H
