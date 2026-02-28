/********************************************************************************
** Form generated from reading UI file 'nceditform.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NCEDITFORM_H
#define UI_NCEDITFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "camotics/qt/FileTabManager.h"

QT_BEGIN_NAMESPACE

class Ui_NcEditForm
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QToolButton *btnNew;
    QToolButton *btnOpen;
    QToolButton *btnSave;
    QToolButton *btnSaveTo;
    QToolButton *btnImport;
    QToolButton *btnExport;
    QToolButton *btnClose;
    QSpacerItem *horizontalSpacer;
    CAMotics::FileTabManager *fileTabManager;
    QHBoxLayout *horizontalLayout_2;
    QToolButton *btnUndo;
    QToolButton *btnRedo;
    QToolButton *btnCut;
    QToolButton *btnCopy;
    QToolButton *btnPaste;
    QToolButton *btnDelete;
    QToolButton *btnSelectAll;
    QToolButton *btnFind;
    QToolButton *btnFindNext;
    QSpacerItem *horizontalSpacer_2;
    QToolButton *btnKeyboard;

    void setupUi(QWidget *NcEditForm)
    {
        if (NcEditForm->objectName().isEmpty())
            NcEditForm->setObjectName(QString::fromUtf8("NcEditForm"));
        NcEditForm->resize(915, 486);
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(NcEditForm->sizePolicy().hasHeightForWidth());
        NcEditForm->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(NcEditForm);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        btnNew = new QToolButton(NcEditForm);
        btnNew->setObjectName(QString::fromUtf8("btnNew"));
        QFont font;
        font.setPointSize(20);
        btnNew->setFont(font);
        btnNew->setIconSize(QSize(64, 64));

        horizontalLayout->addWidget(btnNew);

        btnOpen = new QToolButton(NcEditForm);
        btnOpen->setObjectName(QString::fromUtf8("btnOpen"));
        btnOpen->setFont(font);
        btnOpen->setIconSize(QSize(64, 64));

        horizontalLayout->addWidget(btnOpen);

        btnSave = new QToolButton(NcEditForm);
        btnSave->setObjectName(QString::fromUtf8("btnSave"));
        btnSave->setFont(font);
        btnSave->setIconSize(QSize(64, 64));

        horizontalLayout->addWidget(btnSave);

        btnSaveTo = new QToolButton(NcEditForm);
        btnSaveTo->setObjectName(QString::fromUtf8("btnSaveTo"));
        btnSaveTo->setFont(font);
        btnSaveTo->setIconSize(QSize(64, 64));

        horizontalLayout->addWidget(btnSaveTo);

        btnImport = new QToolButton(NcEditForm);
        btnImport->setObjectName(QString::fromUtf8("btnImport"));
        btnImport->setFont(font);
        btnImport->setIconSize(QSize(64, 64));

        horizontalLayout->addWidget(btnImport);

        btnExport = new QToolButton(NcEditForm);
        btnExport->setObjectName(QString::fromUtf8("btnExport"));
        btnExport->setFont(font);
        btnExport->setIconSize(QSize(64, 64));

        horizontalLayout->addWidget(btnExport);

        btnClose = new QToolButton(NcEditForm);
        btnClose->setObjectName(QString::fromUtf8("btnClose"));
        btnClose->setFont(font);
        btnClose->setIconSize(QSize(64, 64));

        horizontalLayout->addWidget(btnClose);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);

        fileTabManager = new CAMotics::FileTabManager(NcEditForm);
        fileTabManager->setObjectName(QString::fromUtf8("fileTabManager"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(fileTabManager->sizePolicy().hasHeightForWidth());
        fileTabManager->setSizePolicy(sizePolicy1);
        fileTabManager->setMinimumSize(QSize(0, 0));
        fileTabManager->setMaximumSize(QSize(16777215, 16777215));
        fileTabManager->setTabPosition(QTabWidget::North);
        fileTabManager->setTabShape(QTabWidget::Rounded);
        fileTabManager->setElideMode(Qt::ElideNone);
        fileTabManager->setDocumentMode(false);
        fileTabManager->setTabsClosable(false);
        fileTabManager->setMovable(false);
        fileTabManager->setTabBarAutoHide(false);

        verticalLayout->addWidget(fileTabManager);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        btnUndo = new QToolButton(NcEditForm);
        btnUndo->setObjectName(QString::fromUtf8("btnUndo"));

        horizontalLayout_2->addWidget(btnUndo);

        btnRedo = new QToolButton(NcEditForm);
        btnRedo->setObjectName(QString::fromUtf8("btnRedo"));

        horizontalLayout_2->addWidget(btnRedo);

        btnCut = new QToolButton(NcEditForm);
        btnCut->setObjectName(QString::fromUtf8("btnCut"));

        horizontalLayout_2->addWidget(btnCut);

        btnCopy = new QToolButton(NcEditForm);
        btnCopy->setObjectName(QString::fromUtf8("btnCopy"));

        horizontalLayout_2->addWidget(btnCopy);

        btnPaste = new QToolButton(NcEditForm);
        btnPaste->setObjectName(QString::fromUtf8("btnPaste"));

        horizontalLayout_2->addWidget(btnPaste);

        btnDelete = new QToolButton(NcEditForm);
        btnDelete->setObjectName(QString::fromUtf8("btnDelete"));

        horizontalLayout_2->addWidget(btnDelete);

        btnSelectAll = new QToolButton(NcEditForm);
        btnSelectAll->setObjectName(QString::fromUtf8("btnSelectAll"));

        horizontalLayout_2->addWidget(btnSelectAll);

        btnFind = new QToolButton(NcEditForm);
        btnFind->setObjectName(QString::fromUtf8("btnFind"));

        horizontalLayout_2->addWidget(btnFind);

        btnFindNext = new QToolButton(NcEditForm);
        btnFindNext->setObjectName(QString::fromUtf8("btnFindNext"));

        horizontalLayout_2->addWidget(btnFindNext);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        btnKeyboard = new QToolButton(NcEditForm);
        btnKeyboard->setObjectName(QString::fromUtf8("btnKeyboard"));
        btnKeyboard->setCheckable(true);

        horizontalLayout_2->addWidget(btnKeyboard);


        verticalLayout->addLayout(horizontalLayout_2);


        retranslateUi(NcEditForm);

        fileTabManager->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(NcEditForm);
    } // setupUi

    void retranslateUi(QWidget *NcEditForm)
    {
        NcEditForm->setWindowTitle(QCoreApplication::translate("NcEditForm", "NCEdit", nullptr));
        btnNew->setText(QCoreApplication::translate("NcEditForm", "XJ", nullptr));
        btnOpen->setText(QCoreApplication::translate("NcEditForm", "DK", nullptr));
        btnSave->setText(QCoreApplication::translate("NcEditForm", "BC", nullptr));
        btnSaveTo->setText(QCoreApplication::translate("NcEditForm", "LC", nullptr));
        btnImport->setText(QCoreApplication::translate("NcEditForm", "DR", nullptr));
        btnExport->setText(QCoreApplication::translate("NcEditForm", "DC", nullptr));
        btnClose->setText(QCoreApplication::translate("NcEditForm", "GB", nullptr));
        btnUndo->setText(QCoreApplication::translate("NcEditForm", "Undo", nullptr));
        btnRedo->setText(QCoreApplication::translate("NcEditForm", "Redo", nullptr));
        btnCut->setText(QCoreApplication::translate("NcEditForm", "Cut", nullptr));
        btnCopy->setText(QCoreApplication::translate("NcEditForm", "Copy", nullptr));
        btnPaste->setText(QCoreApplication::translate("NcEditForm", "Paste", nullptr));
        btnDelete->setText(QCoreApplication::translate("NcEditForm", "Delete", nullptr));
        btnSelectAll->setText(QCoreApplication::translate("NcEditForm", "SelectAll", nullptr));
        btnFind->setText(QCoreApplication::translate("NcEditForm", "Find", nullptr));
        btnFindNext->setText(QCoreApplication::translate("NcEditForm", "FindNext", nullptr));
        btnKeyboard->setText(QCoreApplication::translate("NcEditForm", "Keyboard", nullptr));
    } // retranslateUi

};

namespace Ui {
    class NcEditForm: public Ui_NcEditForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NCEDITFORM_H
