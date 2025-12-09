/********************************************************************************
** Form generated from reading UI file 'tool4loadsave.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TOOL4LOADSAVE_H
#define UI_TOOL4LOADSAVE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Tool4LoadSaveClass
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *btnLoad;
    QPushButton *btnSave;

    void setupUi(QWidget *Tool4LoadSaveClass)
    {
        if (Tool4LoadSaveClass->objectName().isEmpty())
            Tool4LoadSaveClass->setObjectName(QString::fromUtf8("Tool4LoadSaveClass"));
        Tool4LoadSaveClass->resize(360, 180);
        Tool4LoadSaveClass->setStyleSheet(QString::fromUtf8(""));
        verticalLayout = new QVBoxLayout(Tool4LoadSaveClass);
        verticalLayout->setSpacing(5);
        verticalLayout->setContentsMargins(5, 5, 5, 5);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(5);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        btnLoad = new QPushButton(Tool4LoadSaveClass);
        btnLoad->setObjectName(QString::fromUtf8("btnLoad"));

        horizontalLayout->addWidget(btnLoad);

        btnSave = new QPushButton(Tool4LoadSaveClass);
        btnSave->setObjectName(QString::fromUtf8("btnSave"));

        horizontalLayout->addWidget(btnSave);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(Tool4LoadSaveClass);

        QMetaObject::connectSlotsByName(Tool4LoadSaveClass);
    } // setupUi

    void retranslateUi(QWidget *Tool4LoadSaveClass)
    {
        Tool4LoadSaveClass->setWindowTitle(QCoreApplication::translate("Tool4LoadSaveClass", "Tool4LoadSave", nullptr));
        btnLoad->setText(QCoreApplication::translate("Tool4LoadSaveClass", "\350\257\273\345\217\226", nullptr));
        btnSave->setText(QCoreApplication::translate("Tool4LoadSaveClass", "\344\277\235\345\255\230", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Tool4LoadSaveClass: public Ui_Tool4LoadSaveClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TOOL4LOADSAVE_H
