/********************************************************************************
** Form generated from reading UI file 'regwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REGWINDOW_H
#define UI_REGWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RegWindow
{
public:
    QGridLayout *gridLayout;
    QTabWidget *tabWidget;
    QWidget *tab_17;
    QVBoxLayout *verticalLayoutTab17;
    QWidget *widget17;
    QWidget *tab;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout1;
    QPushButton *btnStart;
    QPushButton *btnStop;
    QLineEdit *lineEditConnectionIndex;
    QCheckBox *ckbVerbosity;
    QLineEdit *lineEditReadRegAddr;
    QLineEdit *lineEditReadRegNum;
    QPushButton *btnReadOnce;
    QPushButton *btnWriteReg;
    QTableView *tblRegisters;
    QWidget *tab_2;
    QHBoxLayout *horizontalLayoutTab2;
    QVBoxLayout *verticalLayoutTab2_1;
    QWidget *widget2;
    QHBoxLayout *horizontalLayout2_2;
    QPushButton *btnStartJog;
    QPushButton *btnStopJog;
    QVBoxLayout *verticalLayoutTab2_2;
    QWidget *tab_16;
    QWidget *tab_3;
    QVBoxLayout *verticalLayoutTab3;
    QWidget *widget3;
    QHBoxLayout *horizontalLayout3;
    QPushButton *btnStartLoc;
    QPushButton *btnStopLoc;
    QWidget *tab_4;
    QVBoxLayout *verticalLayoutTab4;
    QWidget *widget4;
    QHBoxLayout *horizontalLayout4;
    QPushButton *btnStartCmvTouch;
    QPushButton *btnStopCmvTouch;
    QWidget *tab_5;
    QVBoxLayout *verticalLayoutTab5;
    QWidget *widget5;
    QHBoxLayout *horizontalLayout5;
    QPushButton *btnStartCmvToLMT;
    QPushButton *btnStopCmvToLMT;
    QWidget *tab_6;
    QVBoxLayout *verticalLayoutTab6;
    QWidget *widget6;
    QHBoxLayout *horizontalLayout6;
    QPushButton *btnStartCmvFRMLMT;
    QPushButton *btnStopCmvFRMLMT;
    QWidget *tab_7;
    QVBoxLayout *verticalLayoutTab7;
    QWidget *widget7;
    QHBoxLayout *horizontalLayout7;
    QPushButton *btnStartTOZSIG;
    QPushButton *btnStopTOZSIG;
    QWidget *tab_8;
    QVBoxLayout *verticalLayoutTab8;
    QWidget *widget8;
    QHBoxLayout *horizontalLayout8;
    QPushButton *btnStartSPK;
    QPushButton *btnStopSPK;
    QWidget *tab_9;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayoutTab9;
    QWidget *tab_10;
    QWidget *tab_11;
    QVBoxLayout *verticalLayoutTab11;
    QWidget *widget11;
    QHBoxLayout *horizontalLayout11;
    QPushButton *btnSendFangdianIO;
    QWidget *tab_12;
    QVBoxLayout *verticalLayoutTab12;
    QWidget *widget12;
    QHBoxLayout *horizontalLayout12;
    QPushButton *btnSendFuzhuIO;
    QWidget *tab_13;
    QVBoxLayout *verticalLayoutTab13;
    QWidget *widget13;
    QHBoxLayout *horizontalLayout13;
    QPushButton *btnSendKshkz;
    QPushButton *btnSendKshkz2;
    QWidget *tab_14;
    QVBoxLayout *verticalLayoutTab14;
    QWidget *widget14;
    QHBoxLayout *horizontalLayout14;
    QWidget *widget14_2;
    QHBoxLayout *horizontalLayout14_2;
    QPushButton *btnSendZLJBC;
    QWidget *tab_15;
    QVBoxLayout *verticalLayoutTab15;
    QWidget *widget15;

    void setupUi(QDialog *RegWindow)
    {
        if (RegWindow->objectName().isEmpty())
            RegWindow->setObjectName(QString::fromUtf8("RegWindow"));
        RegWindow->resize(1098, 403);
        gridLayout = new QGridLayout(RegWindow);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        tabWidget = new QTabWidget(RegWindow);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tab_17 = new QWidget();
        tab_17->setObjectName(QString::fromUtf8("tab_17"));
        tab_17->setEnabled(true);
        verticalLayoutTab17 = new QVBoxLayout(tab_17);
        verticalLayoutTab17->setObjectName(QString::fromUtf8("verticalLayoutTab17"));
        widget17 = new QWidget(tab_17);
        widget17->setObjectName(QString::fromUtf8("widget17"));

        verticalLayoutTab17->addWidget(widget17);

        tabWidget->addTab(tab_17, QString());
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        verticalLayout = new QVBoxLayout(tab);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout1 = new QHBoxLayout();
        horizontalLayout1->setObjectName(QString::fromUtf8("horizontalLayout1"));
        btnStart = new QPushButton(tab);
        btnStart->setObjectName(QString::fromUtf8("btnStart"));

        horizontalLayout1->addWidget(btnStart);

        btnStop = new QPushButton(tab);
        btnStop->setObjectName(QString::fromUtf8("btnStop"));
        btnStop->setEnabled(false);

        horizontalLayout1->addWidget(btnStop);

        lineEditConnectionIndex = new QLineEdit(tab);
        lineEditConnectionIndex->setObjectName(QString::fromUtf8("lineEditConnectionIndex"));

        horizontalLayout1->addWidget(lineEditConnectionIndex);

        ckbVerbosity = new QCheckBox(tab);
        ckbVerbosity->setObjectName(QString::fromUtf8("ckbVerbosity"));
        ckbVerbosity->setChecked(false);

        horizontalLayout1->addWidget(ckbVerbosity);

        lineEditReadRegAddr = new QLineEdit(tab);
        lineEditReadRegAddr->setObjectName(QString::fromUtf8("lineEditReadRegAddr"));

        horizontalLayout1->addWidget(lineEditReadRegAddr);

        lineEditReadRegNum = new QLineEdit(tab);
        lineEditReadRegNum->setObjectName(QString::fromUtf8("lineEditReadRegNum"));

        horizontalLayout1->addWidget(lineEditReadRegNum);

        btnReadOnce = new QPushButton(tab);
        btnReadOnce->setObjectName(QString::fromUtf8("btnReadOnce"));
        btnReadOnce->setEnabled(true);

        horizontalLayout1->addWidget(btnReadOnce);

        btnWriteReg = new QPushButton(tab);
        btnWriteReg->setObjectName(QString::fromUtf8("btnWriteReg"));

        horizontalLayout1->addWidget(btnWriteReg);

        horizontalLayout1->setStretch(0, 2);
        horizontalLayout1->setStretch(1, 2);
        horizontalLayout1->setStretch(3, 2);
        horizontalLayout1->setStretch(4, 1);
        horizontalLayout1->setStretch(5, 1);
        horizontalLayout1->setStretch(6, 2);
        horizontalLayout1->setStretch(7, 2);

        verticalLayout->addLayout(horizontalLayout1);

        tblRegisters = new QTableView(tab);
        tblRegisters->setObjectName(QString::fromUtf8("tblRegisters"));

        verticalLayout->addWidget(tblRegisters);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        horizontalLayoutTab2 = new QHBoxLayout(tab_2);
        horizontalLayoutTab2->setObjectName(QString::fromUtf8("horizontalLayoutTab2"));
        verticalLayoutTab2_1 = new QVBoxLayout();
        verticalLayoutTab2_1->setObjectName(QString::fromUtf8("verticalLayoutTab2_1"));
        widget2 = new QWidget(tab_2);
        widget2->setObjectName(QString::fromUtf8("widget2"));
        horizontalLayout2_2 = new QHBoxLayout(widget2);
        horizontalLayout2_2->setObjectName(QString::fromUtf8("horizontalLayout2_2"));
        btnStartJog = new QPushButton(widget2);
        btnStartJog->setObjectName(QString::fromUtf8("btnStartJog"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(btnStartJog->sizePolicy().hasHeightForWidth());
        btnStartJog->setSizePolicy(sizePolicy);
        btnStartJog->setFlat(false);

        horizontalLayout2_2->addWidget(btnStartJog);

        btnStopJog = new QPushButton(widget2);
        btnStopJog->setObjectName(QString::fromUtf8("btnStopJog"));
        sizePolicy.setHeightForWidth(btnStopJog->sizePolicy().hasHeightForWidth());
        btnStopJog->setSizePolicy(sizePolicy);

        horizontalLayout2_2->addWidget(btnStopJog);


        verticalLayoutTab2_1->addWidget(widget2);


        horizontalLayoutTab2->addLayout(verticalLayoutTab2_1);

        verticalLayoutTab2_2 = new QVBoxLayout();
        verticalLayoutTab2_2->setObjectName(QString::fromUtf8("verticalLayoutTab2_2"));

        horizontalLayoutTab2->addLayout(verticalLayoutTab2_2);

        tabWidget->addTab(tab_2, QString());
        tab_16 = new QWidget();
        tab_16->setObjectName(QString::fromUtf8("tab_16"));
        tabWidget->addTab(tab_16, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        verticalLayoutTab3 = new QVBoxLayout(tab_3);
        verticalLayoutTab3->setObjectName(QString::fromUtf8("verticalLayoutTab3"));
        widget3 = new QWidget(tab_3);
        widget3->setObjectName(QString::fromUtf8("widget3"));
        horizontalLayout3 = new QHBoxLayout(widget3);
        horizontalLayout3->setObjectName(QString::fromUtf8("horizontalLayout3"));
        btnStartLoc = new QPushButton(widget3);
        btnStartLoc->setObjectName(QString::fromUtf8("btnStartLoc"));
        sizePolicy.setHeightForWidth(btnStartLoc->sizePolicy().hasHeightForWidth());
        btnStartLoc->setSizePolicy(sizePolicy);
        btnStartLoc->setFlat(false);

        horizontalLayout3->addWidget(btnStartLoc);

        btnStopLoc = new QPushButton(widget3);
        btnStopLoc->setObjectName(QString::fromUtf8("btnStopLoc"));
        sizePolicy.setHeightForWidth(btnStopLoc->sizePolicy().hasHeightForWidth());
        btnStopLoc->setSizePolicy(sizePolicy);

        horizontalLayout3->addWidget(btnStopLoc);


        verticalLayoutTab3->addWidget(widget3);

        tabWidget->addTab(tab_3, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        verticalLayoutTab4 = new QVBoxLayout(tab_4);
        verticalLayoutTab4->setObjectName(QString::fromUtf8("verticalLayoutTab4"));
        widget4 = new QWidget(tab_4);
        widget4->setObjectName(QString::fromUtf8("widget4"));
        horizontalLayout4 = new QHBoxLayout(widget4);
        horizontalLayout4->setObjectName(QString::fromUtf8("horizontalLayout4"));
        btnStartCmvTouch = new QPushButton(widget4);
        btnStartCmvTouch->setObjectName(QString::fromUtf8("btnStartCmvTouch"));
        sizePolicy.setHeightForWidth(btnStartCmvTouch->sizePolicy().hasHeightForWidth());
        btnStartCmvTouch->setSizePolicy(sizePolicy);
        btnStartCmvTouch->setFlat(false);

        horizontalLayout4->addWidget(btnStartCmvTouch);

        btnStopCmvTouch = new QPushButton(widget4);
        btnStopCmvTouch->setObjectName(QString::fromUtf8("btnStopCmvTouch"));
        sizePolicy.setHeightForWidth(btnStopCmvTouch->sizePolicy().hasHeightForWidth());
        btnStopCmvTouch->setSizePolicy(sizePolicy);

        horizontalLayout4->addWidget(btnStopCmvTouch);


        verticalLayoutTab4->addWidget(widget4);

        tabWidget->addTab(tab_4, QString());
        tab_5 = new QWidget();
        tab_5->setObjectName(QString::fromUtf8("tab_5"));
        verticalLayoutTab5 = new QVBoxLayout(tab_5);
        verticalLayoutTab5->setObjectName(QString::fromUtf8("verticalLayoutTab5"));
        widget5 = new QWidget(tab_5);
        widget5->setObjectName(QString::fromUtf8("widget5"));
        horizontalLayout5 = new QHBoxLayout(widget5);
        horizontalLayout5->setObjectName(QString::fromUtf8("horizontalLayout5"));
        btnStartCmvToLMT = new QPushButton(widget5);
        btnStartCmvToLMT->setObjectName(QString::fromUtf8("btnStartCmvToLMT"));
        sizePolicy.setHeightForWidth(btnStartCmvToLMT->sizePolicy().hasHeightForWidth());
        btnStartCmvToLMT->setSizePolicy(sizePolicy);
        btnStartCmvToLMT->setFlat(false);

        horizontalLayout5->addWidget(btnStartCmvToLMT);

        btnStopCmvToLMT = new QPushButton(widget5);
        btnStopCmvToLMT->setObjectName(QString::fromUtf8("btnStopCmvToLMT"));
        sizePolicy.setHeightForWidth(btnStopCmvToLMT->sizePolicy().hasHeightForWidth());
        btnStopCmvToLMT->setSizePolicy(sizePolicy);

        horizontalLayout5->addWidget(btnStopCmvToLMT);


        verticalLayoutTab5->addWidget(widget5);

        tabWidget->addTab(tab_5, QString());
        tab_6 = new QWidget();
        tab_6->setObjectName(QString::fromUtf8("tab_6"));
        verticalLayoutTab6 = new QVBoxLayout(tab_6);
        verticalLayoutTab6->setObjectName(QString::fromUtf8("verticalLayoutTab6"));
        widget6 = new QWidget(tab_6);
        widget6->setObjectName(QString::fromUtf8("widget6"));
        horizontalLayout6 = new QHBoxLayout(widget6);
        horizontalLayout6->setObjectName(QString::fromUtf8("horizontalLayout6"));
        btnStartCmvFRMLMT = new QPushButton(widget6);
        btnStartCmvFRMLMT->setObjectName(QString::fromUtf8("btnStartCmvFRMLMT"));
        sizePolicy.setHeightForWidth(btnStartCmvFRMLMT->sizePolicy().hasHeightForWidth());
        btnStartCmvFRMLMT->setSizePolicy(sizePolicy);
        btnStartCmvFRMLMT->setFlat(false);

        horizontalLayout6->addWidget(btnStartCmvFRMLMT);

        btnStopCmvFRMLMT = new QPushButton(widget6);
        btnStopCmvFRMLMT->setObjectName(QString::fromUtf8("btnStopCmvFRMLMT"));
        sizePolicy.setHeightForWidth(btnStopCmvFRMLMT->sizePolicy().hasHeightForWidth());
        btnStopCmvFRMLMT->setSizePolicy(sizePolicy);

        horizontalLayout6->addWidget(btnStopCmvFRMLMT);


        verticalLayoutTab6->addWidget(widget6);

        tabWidget->addTab(tab_6, QString());
        tab_7 = new QWidget();
        tab_7->setObjectName(QString::fromUtf8("tab_7"));
        verticalLayoutTab7 = new QVBoxLayout(tab_7);
        verticalLayoutTab7->setObjectName(QString::fromUtf8("verticalLayoutTab7"));
        widget7 = new QWidget(tab_7);
        widget7->setObjectName(QString::fromUtf8("widget7"));
        horizontalLayout7 = new QHBoxLayout(widget7);
        horizontalLayout7->setObjectName(QString::fromUtf8("horizontalLayout7"));
        btnStartTOZSIG = new QPushButton(widget7);
        btnStartTOZSIG->setObjectName(QString::fromUtf8("btnStartTOZSIG"));
        sizePolicy.setHeightForWidth(btnStartTOZSIG->sizePolicy().hasHeightForWidth());
        btnStartTOZSIG->setSizePolicy(sizePolicy);
        btnStartTOZSIG->setFlat(false);

        horizontalLayout7->addWidget(btnStartTOZSIG);

        btnStopTOZSIG = new QPushButton(widget7);
        btnStopTOZSIG->setObjectName(QString::fromUtf8("btnStopTOZSIG"));
        sizePolicy.setHeightForWidth(btnStopTOZSIG->sizePolicy().hasHeightForWidth());
        btnStopTOZSIG->setSizePolicy(sizePolicy);

        horizontalLayout7->addWidget(btnStopTOZSIG);


        verticalLayoutTab7->addWidget(widget7);

        tabWidget->addTab(tab_7, QString());
        tab_8 = new QWidget();
        tab_8->setObjectName(QString::fromUtf8("tab_8"));
        verticalLayoutTab8 = new QVBoxLayout(tab_8);
        verticalLayoutTab8->setObjectName(QString::fromUtf8("verticalLayoutTab8"));
        widget8 = new QWidget(tab_8);
        widget8->setObjectName(QString::fromUtf8("widget8"));
        horizontalLayout8 = new QHBoxLayout(widget8);
        horizontalLayout8->setObjectName(QString::fromUtf8("horizontalLayout8"));
        btnStartSPK = new QPushButton(widget8);
        btnStartSPK->setObjectName(QString::fromUtf8("btnStartSPK"));
        sizePolicy.setHeightForWidth(btnStartSPK->sizePolicy().hasHeightForWidth());
        btnStartSPK->setSizePolicy(sizePolicy);
        btnStartSPK->setFlat(false);

        horizontalLayout8->addWidget(btnStartSPK);

        btnStopSPK = new QPushButton(widget8);
        btnStopSPK->setObjectName(QString::fromUtf8("btnStopSPK"));
        sizePolicy.setHeightForWidth(btnStopSPK->sizePolicy().hasHeightForWidth());
        btnStopSPK->setSizePolicy(sizePolicy);

        horizontalLayout8->addWidget(btnStopSPK);


        verticalLayoutTab8->addWidget(widget8);

        tabWidget->addTab(tab_8, QString());
        tab_9 = new QWidget();
        tab_9->setObjectName(QString::fromUtf8("tab_9"));
        gridLayoutWidget = new QWidget(tab_9);
        gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(30, 20, 371, 171));
        gridLayoutTab9 = new QGridLayout(gridLayoutWidget);
        gridLayoutTab9->setObjectName(QString::fromUtf8("gridLayoutTab9"));
        gridLayoutTab9->setContentsMargins(0, 0, 0, 0);
        tabWidget->addTab(tab_9, QString());
        tab_10 = new QWidget();
        tab_10->setObjectName(QString::fromUtf8("tab_10"));
        tabWidget->addTab(tab_10, QString());
        tab_11 = new QWidget();
        tab_11->setObjectName(QString::fromUtf8("tab_11"));
        verticalLayoutTab11 = new QVBoxLayout(tab_11);
        verticalLayoutTab11->setObjectName(QString::fromUtf8("verticalLayoutTab11"));
        widget11 = new QWidget(tab_11);
        widget11->setObjectName(QString::fromUtf8("widget11"));
        horizontalLayout11 = new QHBoxLayout(widget11);
        horizontalLayout11->setObjectName(QString::fromUtf8("horizontalLayout11"));
        btnSendFangdianIO = new QPushButton(widget11);
        btnSendFangdianIO->setObjectName(QString::fromUtf8("btnSendFangdianIO"));
        sizePolicy.setHeightForWidth(btnSendFangdianIO->sizePolicy().hasHeightForWidth());
        btnSendFangdianIO->setSizePolicy(sizePolicy);
        btnSendFangdianIO->setFlat(false);

        horizontalLayout11->addWidget(btnSendFangdianIO);


        verticalLayoutTab11->addWidget(widget11);

        tabWidget->addTab(tab_11, QString());
        tab_12 = new QWidget();
        tab_12->setObjectName(QString::fromUtf8("tab_12"));
        verticalLayoutTab12 = new QVBoxLayout(tab_12);
        verticalLayoutTab12->setObjectName(QString::fromUtf8("verticalLayoutTab12"));
        widget12 = new QWidget(tab_12);
        widget12->setObjectName(QString::fromUtf8("widget12"));
        horizontalLayout12 = new QHBoxLayout(widget12);
        horizontalLayout12->setObjectName(QString::fromUtf8("horizontalLayout12"));
        btnSendFuzhuIO = new QPushButton(widget12);
        btnSendFuzhuIO->setObjectName(QString::fromUtf8("btnSendFuzhuIO"));
        sizePolicy.setHeightForWidth(btnSendFuzhuIO->sizePolicy().hasHeightForWidth());
        btnSendFuzhuIO->setSizePolicy(sizePolicy);
        btnSendFuzhuIO->setFlat(false);

        horizontalLayout12->addWidget(btnSendFuzhuIO);


        verticalLayoutTab12->addWidget(widget12);

        tabWidget->addTab(tab_12, QString());
        tab_13 = new QWidget();
        tab_13->setObjectName(QString::fromUtf8("tab_13"));
        verticalLayoutTab13 = new QVBoxLayout(tab_13);
        verticalLayoutTab13->setObjectName(QString::fromUtf8("verticalLayoutTab13"));
        widget13 = new QWidget(tab_13);
        widget13->setObjectName(QString::fromUtf8("widget13"));
        horizontalLayout13 = new QHBoxLayout(widget13);
        horizontalLayout13->setObjectName(QString::fromUtf8("horizontalLayout13"));
        btnSendKshkz = new QPushButton(widget13);
        btnSendKshkz->setObjectName(QString::fromUtf8("btnSendKshkz"));
        sizePolicy.setHeightForWidth(btnSendKshkz->sizePolicy().hasHeightForWidth());
        btnSendKshkz->setSizePolicy(sizePolicy);
        btnSendKshkz->setFlat(false);

        horizontalLayout13->addWidget(btnSendKshkz);

        btnSendKshkz2 = new QPushButton(widget13);
        btnSendKshkz2->setObjectName(QString::fromUtf8("btnSendKshkz2"));
        sizePolicy.setHeightForWidth(btnSendKshkz2->sizePolicy().hasHeightForWidth());
        btnSendKshkz2->setSizePolicy(sizePolicy);
        btnSendKshkz2->setFlat(false);

        horizontalLayout13->addWidget(btnSendKshkz2);


        verticalLayoutTab13->addWidget(widget13);

        tabWidget->addTab(tab_13, QString());
        tab_14 = new QWidget();
        tab_14->setObjectName(QString::fromUtf8("tab_14"));
        verticalLayoutTab14 = new QVBoxLayout(tab_14);
        verticalLayoutTab14->setObjectName(QString::fromUtf8("verticalLayoutTab14"));
        widget14 = new QWidget(tab_14);
        widget14->setObjectName(QString::fromUtf8("widget14"));
        horizontalLayout14 = new QHBoxLayout(widget14);
        horizontalLayout14->setObjectName(QString::fromUtf8("horizontalLayout14"));

        verticalLayoutTab14->addWidget(widget14);

        widget14_2 = new QWidget(tab_14);
        widget14_2->setObjectName(QString::fromUtf8("widget14_2"));
        horizontalLayout14_2 = new QHBoxLayout(widget14_2);
        horizontalLayout14_2->setObjectName(QString::fromUtf8("horizontalLayout14_2"));
        btnSendZLJBC = new QPushButton(widget14_2);
        btnSendZLJBC->setObjectName(QString::fromUtf8("btnSendZLJBC"));
        sizePolicy.setHeightForWidth(btnSendZLJBC->sizePolicy().hasHeightForWidth());
        btnSendZLJBC->setSizePolicy(sizePolicy);
        btnSendZLJBC->setFlat(false);

        horizontalLayout14_2->addWidget(btnSendZLJBC);


        verticalLayoutTab14->addWidget(widget14_2);

        tabWidget->addTab(tab_14, QString());
        tab_15 = new QWidget();
        tab_15->setObjectName(QString::fromUtf8("tab_15"));
        verticalLayoutTab15 = new QVBoxLayout(tab_15);
        verticalLayoutTab15->setObjectName(QString::fromUtf8("verticalLayoutTab15"));
        widget15 = new QWidget(tab_15);
        widget15->setObjectName(QString::fromUtf8("widget15"));

        verticalLayoutTab15->addWidget(widget15);

        tabWidget->addTab(tab_15, QString());

        gridLayout->addWidget(tabWidget, 0, 0, 1, 1);


        retranslateUi(RegWindow);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(RegWindow);
    } // setupUi

    void retranslateUi(QDialog *RegWindow)
    {
        RegWindow->setWindowTitle(QCoreApplication::translate("RegWindow", "Dialog", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_17), QCoreApplication::translate("RegWindow", "MB", nullptr));
        btnStart->setText(QCoreApplication::translate("RegWindow", "Connect", nullptr));
        btnStop->setText(QCoreApplication::translate("RegWindow", "Disconnect", nullptr));
        lineEditConnectionIndex->setText(QCoreApplication::translate("RegWindow", "0", nullptr));
        ckbVerbosity->setText(QCoreApplication::translate("RegWindow", "Verbosity", nullptr));
        lineEditReadRegAddr->setText(QCoreApplication::translate("RegWindow", "0", nullptr));
        lineEditReadRegNum->setText(QCoreApplication::translate("RegWindow", "2", nullptr));
        btnReadOnce->setText(QCoreApplication::translate("RegWindow", "Read Reg", nullptr));
        btnWriteReg->setText(QCoreApplication::translate("RegWindow", "Write Reg", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("RegWindow", "JCQ", nullptr));
        btnStartJog->setText(QCoreApplication::translate("RegWindow", "QDJ", nullptr));
        btnStopJog->setText(QCoreApplication::translate("RegWindow", "TCJ", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("RegWindow", "JOG", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_16), QCoreApplication::translate("RegWindow", "DDYD", nullptr));
        btnStartLoc->setText(QCoreApplication::translate("RegWindow", "QDDW", nullptr));
        btnStopLoc->setText(QCoreApplication::translate("RegWindow", "TCDW", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QCoreApplication::translate("RegWindow", "LOC", nullptr));
        btnStartCmvTouch->setText(QCoreApplication::translate("RegWindow", "QDDD", nullptr));
        btnStopCmvTouch->setText(QCoreApplication::translate("RegWindow", "JSDD", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QCoreApplication::translate("RegWindow", "CMV_TOUCH", nullptr));
        btnStartCmvToLMT->setText(QCoreApplication::translate("RegWindow", "QD", nullptr));
        btnStopCmvToLMT->setText(QCoreApplication::translate("RegWindow", "JS", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_5), QCoreApplication::translate("RegWindow", "CMV_TOLMT", nullptr));
        btnStartCmvFRMLMT->setText(QCoreApplication::translate("RegWindow", "QD", nullptr));
        btnStopCmvFRMLMT->setText(QCoreApplication::translate("RegWindow", "JS", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_6), QCoreApplication::translate("RegWindow", "CMV_FRMLMT", nullptr));
        btnStartTOZSIG->setText(QCoreApplication::translate("RegWindow", "QD", nullptr));
        btnStopTOZSIG->setText(QCoreApplication::translate("RegWindow", "JS", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_7), QCoreApplication::translate("RegWindow", "CMV_TOZSIG", nullptr));
        btnStartSPK->setText(QCoreApplication::translate("RegWindow", "QD", nullptr));
        btnStopSPK->setText(QCoreApplication::translate("RegWindow", "JS", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_8), QCoreApplication::translate("RegWindow", "SPK", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_9), QCoreApplication::translate("RegWindow", "JC", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_10), QCoreApplication::translate("RegWindow", "SKH", nullptr));
        btnSendFangdianIO->setText(QCoreApplication::translate("RegWindow", "FS", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_11), QCoreApplication::translate("RegWindow", "FDI", nullptr));
        btnSendFuzhuIO->setText(QCoreApplication::translate("RegWindow", "FS", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_12), QCoreApplication::translate("RegWindow", "FZI", nullptr));
        btnSendKshkz->setText(QCoreApplication::translate("RegWindow", "FS", nullptr));
        btnSendKshkz2->setText(QCoreApplication::translate("RegWindow", "DQ", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_13), QCoreApplication::translate("RegWindow", "SKHKZSJ", nullptr));
        btnSendZLJBC->setText(QCoreApplication::translate("RegWindow", "FS", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_14), QCoreApplication::translate("RegWindow", "BC", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_15), QCoreApplication::translate("RegWindow", "Modbus File", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RegWindow: public Ui_RegWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGWINDOW_H
