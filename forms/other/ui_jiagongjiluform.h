/********************************************************************************
** Form generated from reading UI file 'jiagongjiluform.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_JIAGONGJILUFORM_H
#define UI_JIAGONGJILUFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_JiaGongJiLuFormClass
{
public:
    QHBoxLayout *horizontalLayout;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayoutTop;
    QVBoxLayout *verticalLayout;
    QTableView *tableView;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *prevPageButton;
    QLabel *pageLabel;
    QPushButton *nextPageButton;
    QVBoxLayout *verticalLayoutDetail;
    QTableView *tableViewDetail;

    void setupUi(QWidget *JiaGongJiLuFormClass)
    {
        if (JiaGongJiLuFormClass->objectName().isEmpty())
            JiaGongJiLuFormClass->setObjectName(QString::fromUtf8("JiaGongJiLuFormClass"));
        JiaGongJiLuFormClass->resize(600, 400);
        horizontalLayout = new QHBoxLayout(JiaGongJiLuFormClass);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        centralWidget = new QWidget(JiaGongJiLuFormClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        horizontalLayoutTop = new QHBoxLayout(centralWidget);
        horizontalLayoutTop->setSpacing(6);
        horizontalLayoutTop->setContentsMargins(11, 11, 11, 11);
        horizontalLayoutTop->setObjectName(QString::fromUtf8("horizontalLayoutTop"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        tableView = new QTableView(centralWidget);
        tableView->setObjectName(QString::fromUtf8("tableView"));
        tableView->setAlternatingRowColors(true);
        tableView->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
        tableView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);

        verticalLayout->addWidget(tableView);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        prevPageButton = new QPushButton(centralWidget);
        prevPageButton->setObjectName(QString::fromUtf8("prevPageButton"));
        prevPageButton->setEnabled(false);

        horizontalLayout_2->addWidget(prevPageButton);

        pageLabel = new QLabel(centralWidget);
        pageLabel->setObjectName(QString::fromUtf8("pageLabel"));
        pageLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        horizontalLayout_2->addWidget(pageLabel);

        nextPageButton = new QPushButton(centralWidget);
        nextPageButton->setObjectName(QString::fromUtf8("nextPageButton"));
        nextPageButton->setEnabled(false);

        horizontalLayout_2->addWidget(nextPageButton);


        verticalLayout->addLayout(horizontalLayout_2);


        horizontalLayoutTop->addLayout(verticalLayout);

        verticalLayoutDetail = new QVBoxLayout();
        verticalLayoutDetail->setSpacing(6);
        verticalLayoutDetail->setObjectName(QString::fromUtf8("verticalLayoutDetail"));
        tableViewDetail = new QTableView(centralWidget);
        tableViewDetail->setObjectName(QString::fromUtf8("tableViewDetail"));
        tableViewDetail->setAlternatingRowColors(true);
        tableViewDetail->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
        tableViewDetail->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);

        verticalLayoutDetail->addWidget(tableViewDetail);


        horizontalLayoutTop->addLayout(verticalLayoutDetail);

        horizontalLayoutTop->setStretch(0, 3);
        horizontalLayoutTop->setStretch(1, 1);

        horizontalLayout->addWidget(centralWidget);


        retranslateUi(JiaGongJiLuFormClass);

        QMetaObject::connectSlotsByName(JiaGongJiLuFormClass);
    } // setupUi

    void retranslateUi(QWidget *JiaGongJiLuFormClass)
    {
        JiaGongJiLuFormClass->setWindowTitle(QCoreApplication::translate("JiaGongJiLuFormClass", "jiagongjilu", nullptr));
        prevPageButton->setText(QCoreApplication::translate("JiaGongJiLuFormClass", "\344\270\212\344\270\200\351\241\265", nullptr));
        pageLabel->setText(QCoreApplication::translate("JiaGongJiLuFormClass", "\345\212\240\345\267\245\350\256\260\345\275\225", nullptr));
        nextPageButton->setText(QCoreApplication::translate("JiaGongJiLuFormClass", "\344\270\213\344\270\200\351\241\265", nullptr));
    } // retranslateUi

};

namespace Ui {
    class JiaGongJiLuFormClass: public Ui_JiaGongJiLuFormClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_JIAGONGJILUFORM_H
