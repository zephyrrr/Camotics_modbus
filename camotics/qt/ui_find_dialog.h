/********************************************************************************
** Form generated from reading UI file 'find_dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FIND_DIALOG_H
#define UI_FIND_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_FindDialog
{
public:
    QVBoxLayout *verticalLayout;
    QFormLayout *formLayout;
    QLabel *label;
    QLineEdit *findLineEdit;
    QLabel *replaceLabel;
    QLineEdit *replaceLineEdit;
    QCheckBox *regexCheckBox;
    QCheckBox *caseSensitiveCheckBox;
    QCheckBox *wholeWordsCheckBox;
    QLabel *messageLabel;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *replaceButton;
    QPushButton *replaceAllButton;
    QPushButton *findButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *FindDialog)
    {
        if (FindDialog->objectName().isEmpty())
            FindDialog->setObjectName(QString::fromUtf8("FindDialog"));
        FindDialog->resize(400, 230);
        FindDialog->setMinimumSize(QSize(0, 230));
        FindDialog->setMaximumSize(QSize(1000, 230));
        FindDialog->setModal(true);
        verticalLayout = new QVBoxLayout(FindDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        formLayout = new QFormLayout();
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        label = new QLabel(FindDialog);
        label->setObjectName(QString::fromUtf8("label"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        findLineEdit = new QLineEdit(FindDialog);
        findLineEdit->setObjectName(QString::fromUtf8("findLineEdit"));

        formLayout->setWidget(0, QFormLayout::FieldRole, findLineEdit);

        replaceLabel = new QLabel(FindDialog);
        replaceLabel->setObjectName(QString::fromUtf8("replaceLabel"));

        formLayout->setWidget(1, QFormLayout::LabelRole, replaceLabel);

        replaceLineEdit = new QLineEdit(FindDialog);
        replaceLineEdit->setObjectName(QString::fromUtf8("replaceLineEdit"));

        formLayout->setWidget(1, QFormLayout::FieldRole, replaceLineEdit);

        regexCheckBox = new QCheckBox(FindDialog);
        regexCheckBox->setObjectName(QString::fromUtf8("regexCheckBox"));

        formLayout->setWidget(2, QFormLayout::FieldRole, regexCheckBox);

        caseSensitiveCheckBox = new QCheckBox(FindDialog);
        caseSensitiveCheckBox->setObjectName(QString::fromUtf8("caseSensitiveCheckBox"));

        formLayout->setWidget(3, QFormLayout::FieldRole, caseSensitiveCheckBox);

        wholeWordsCheckBox = new QCheckBox(FindDialog);
        wholeWordsCheckBox->setObjectName(QString::fromUtf8("wholeWordsCheckBox"));

        formLayout->setWidget(4, QFormLayout::FieldRole, wholeWordsCheckBox);


        verticalLayout->addLayout(formLayout);

        messageLabel = new QLabel(FindDialog);
        messageLabel->setObjectName(QString::fromUtf8("messageLabel"));

        verticalLayout->addWidget(messageLabel);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        replaceButton = new QPushButton(FindDialog);
        replaceButton->setObjectName(QString::fromUtf8("replaceButton"));

        horizontalLayout->addWidget(replaceButton);

        replaceAllButton = new QPushButton(FindDialog);
        replaceAllButton->setObjectName(QString::fromUtf8("replaceAllButton"));

        horizontalLayout->addWidget(replaceAllButton);

        findButton = new QPushButton(FindDialog);
        findButton->setObjectName(QString::fromUtf8("findButton"));

        horizontalLayout->addWidget(findButton);

        cancelButton = new QPushButton(FindDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        horizontalLayout->addWidget(cancelButton);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(FindDialog);

        findButton->setDefault(true);


        QMetaObject::connectSlotsByName(FindDialog);
    } // setupUi

    void retranslateUi(QDialog *FindDialog)
    {
        FindDialog->setWindowTitle(QCoreApplication::translate("FindDialog", "Find", nullptr));
        label->setText(QCoreApplication::translate("FindDialog", "Find:", nullptr));
        replaceLabel->setText(QCoreApplication::translate("FindDialog", "Replace:", nullptr));
        regexCheckBox->setText(QCoreApplication::translate("FindDialog", "Regular expression", nullptr));
        caseSensitiveCheckBox->setText(QCoreApplication::translate("FindDialog", "Case sensitive", nullptr));
        wholeWordsCheckBox->setText(QCoreApplication::translate("FindDialog", "Match whole words", nullptr));
        messageLabel->setText(QString());
        replaceButton->setText(QCoreApplication::translate("FindDialog", "Replace", nullptr));
        replaceAllButton->setText(QCoreApplication::translate("FindDialog", "Replace All", nullptr));
        findButton->setText(QCoreApplication::translate("FindDialog", "Find", nullptr));
        cancelButton->setText(QCoreApplication::translate("FindDialog", "Done", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FindDialog: public Ui_FindDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FIND_DIALOG_H
