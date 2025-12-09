/********************************************************************************
** Form generated from reading UI file 'tool4zeroandhalfaxis.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TOOL4ZEROANDHALFAXIS_H
#define UI_TOOL4ZEROANDHALFAXIS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Tool4AllClass
{
public:
    QVBoxLayout *verticalLayout_2;
    QFrame *frame;
    QVBoxLayout *verticalLayout;

    void setupUi(QWidget *Tool4AllClass)
    {
        if (Tool4AllClass->objectName().isEmpty())
            Tool4AllClass->setObjectName(QString::fromUtf8("Tool4AllClass"));
        Tool4AllClass->resize(360, 540);
        Tool4AllClass->setStyleSheet(QString::fromUtf8(""));
        verticalLayout_2 = new QVBoxLayout(Tool4AllClass);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        frame = new QFrame(Tool4AllClass);
        frame->setObjectName(QString::fromUtf8("frame"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy);
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));

        verticalLayout_2->addWidget(frame);


        retranslateUi(Tool4AllClass);

        QMetaObject::connectSlotsByName(Tool4AllClass);
    } // setupUi

    void retranslateUi(QWidget *Tool4AllClass)
    {
        Tool4AllClass->setWindowTitle(QCoreApplication::translate("Tool4AllClass", "Tool4ZeroAndHalfAxis", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Tool4AllClass: public Ui_Tool4AllClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TOOL4ZEROANDHALFAXIS_H
