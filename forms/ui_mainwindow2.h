/********************************************************************************
** Form generated from reading UI file 'mainwindow2.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW2_H
#define UI_MAINWINDOW2_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QtWin2
{
public:
    QAction *actionRun;
    QAction *actionCancel;
    QAction *actionDebug;
    QAction *actionTouchSense;
    QAction *actionTouchSense2;
    QAction *actionLoadStylesheet;
    QAction *actionPythonScript;
    QAction *actionPowerOn;
    QAction *actionPowerOff;
    QAction *actionSaveScreen;
    QAction *actionSaveGCode;
    QWidget *mainWidget;
    QHBoxLayout *horizontalLayout_1;
    QFrame *leftWidget;
    QVBoxLayout *verticalLayout_left;
    QFrame *topinfobar;
    QVBoxLayout *verticalLayout_topinfobar;
    QFrame *topinfobar_shortcut;
    QHBoxLayout *horizontalLayout_topinfobar_shortcut;
    QPushButton *btnCoor;
    QPushButton *btnT0;
    QPushButton *btnT1;
    QPushButton *btnST;
    QPushButton *btnYouWei;
    QPushButton *btnYouWen;
    QPushButton *btnHuoYan;
    QPushButton *btnFloat;
    QPushButton *btnJgtj;
    QPushButton *btnJgtjGt;
    QSpacerItem *horizontalSpacer1;
    QHBoxLayout *horizontalLayout_topinfobar_center;
    QVBoxLayout *verticalLayout_pos;
    QFrame *topinfobar_pos;
    QVBoxLayout *verticalLayout;
    QLabel *xLabel;
    QLabel *yLabel;
    QLabel *zLabel;
    QFrame *topinfobar_pos_uw;
    QHBoxLayout *horizontalLayout_2;
    QLabel *uLabel;
    QLabel *wLabel;
    QVBoxLayout *verticalLayout_loran;
    QFrame *loran;
    QLabel *label_loran;
    QVBoxLayout *verticalLayout_displayz;
    QFrame *displayz;
    QLabel *label_displayz;
    QVBoxLayout *verticalLayout_numbers;
    QFrame *frame_numbers;
    QGridLayout *gridLayout;
    QLabel *labelLength0;
    QLabel *labelLength;
    QLabel *labelCNo0;
    QLabel *labelCNo;
    QLabel *labelRemnantLength0;
    QLabel *labelRemnantLength;
    QLabel *labelTNo0;
    QLabel *labelTNo;
    QLabel *labelTotalTime0;
    QLabel *labelTotalTime;
    QLabel *labelProcessSpeed0;
    QLabel *labelProcessSpeed;
    QLabel *labelProcessTime0;
    QLabel *labelProcessTime;
    QLabel *labelProcessSpeed20;
    QLabel *labelProcessSpeed2;
    QLabel *labelRemnantTime0;
    QLabel *labelRemnantTime;
    QLabel *labelG04RemnantTime0;
    QLabel *labelG04RemnantTime;
    QFrame *frame_files;
    QHBoxLayout *horizontalLayout_filename;
    QLabel *labelFileName0;
    QToolButton *btnFileName;
    QLabel *lblNowDate;
    QLabel *lblNowTime;
    QStackedWidget *stackedChildWidget;
    QHBoxLayout *horizontalLayout_info;
    QLabel *lblResultMessage;
    QWidget *widgetInfoIcons2;
    QHBoxLayout *horizontalLayout_infoicons2;
    QLabel *labelManufactoryIcon;
    QWidget *widgetInfoIcons;
    QHBoxLayout *horizontalLayout_infoicons;
    QLabel *labelInfoLianjie;
    QLabel *labelInfoYouBang;
    QLabel *labelInfoJogSpeed;
    QLabel *labelInfoDiandong;
    QLabel *labelInfoFangdian;
    QLabel *labelInfoDuanLu;
    QLabel *labelInfoYouWen;
    QLabel *labelInfoYouWei;
    QLabel *labelInfoZhaoMing;
    QSpacerItem *horizontalSpacer2;
    QGroupBox *groupBoxMenus;
    QVBoxLayout *verticalLayout_groupBoxMenus;
    QStatusBar *statusbar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *QtWin2)
    {
        if (QtWin2->objectName().isEmpty())
            QtWin2->setObjectName(QString::fromUtf8("QtWin2"));
        QtWin2->resize(1920, 1080);
        actionRun = new QAction(QtWin2);
        actionRun->setObjectName(QString::fromUtf8("actionRun"));
        actionCancel = new QAction(QtWin2);
        actionCancel->setObjectName(QString::fromUtf8("actionCancel"));
        actionDebug = new QAction(QtWin2);
        actionDebug->setObjectName(QString::fromUtf8("actionDebug"));
        actionTouchSense = new QAction(QtWin2);
        actionTouchSense->setObjectName(QString::fromUtf8("actionTouchSense"));
        actionTouchSense2 = new QAction(QtWin2);
        actionTouchSense2->setObjectName(QString::fromUtf8("actionTouchSense2"));
        actionTouchSense2->setCheckable(true);
        actionLoadStylesheet = new QAction(QtWin2);
        actionLoadStylesheet->setObjectName(QString::fromUtf8("actionLoadStylesheet"));
        actionPythonScript = new QAction(QtWin2);
        actionPythonScript->setObjectName(QString::fromUtf8("actionPythonScript"));
        actionPowerOn = new QAction(QtWin2);
        actionPowerOn->setObjectName(QString::fromUtf8("actionPowerOn"));
        actionPowerOff = new QAction(QtWin2);
        actionPowerOff->setObjectName(QString::fromUtf8("actionPowerOff"));
        actionSaveScreen = new QAction(QtWin2);
        actionSaveScreen->setObjectName(QString::fromUtf8("actionSaveScreen"));
        actionSaveGCode = new QAction(QtWin2);
        actionSaveGCode->setObjectName(QString::fromUtf8("actionSaveGCode"));
        mainWidget = new QWidget(QtWin2);
        mainWidget->setObjectName(QString::fromUtf8("mainWidget"));
        horizontalLayout_1 = new QHBoxLayout(mainWidget);
        horizontalLayout_1->setSpacing(0);
        horizontalLayout_1->setObjectName(QString::fromUtf8("horizontalLayout_1"));
        horizontalLayout_1->setContentsMargins(0, 0, 0, 0);
        leftWidget = new QFrame(mainWidget);
        leftWidget->setObjectName(QString::fromUtf8("leftWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(leftWidget->sizePolicy().hasHeightForWidth());
        leftWidget->setSizePolicy(sizePolicy);
        verticalLayout_left = new QVBoxLayout(leftWidget);
        verticalLayout_left->setSpacing(0);
        verticalLayout_left->setObjectName(QString::fromUtf8("verticalLayout_left"));
        verticalLayout_left->setContentsMargins(5, 5, 5, 5);
        topinfobar = new QFrame(leftWidget);
        topinfobar->setObjectName(QString::fromUtf8("topinfobar"));
        verticalLayout_topinfobar = new QVBoxLayout(topinfobar);
        verticalLayout_topinfobar->setSpacing(0);
        verticalLayout_topinfobar->setObjectName(QString::fromUtf8("verticalLayout_topinfobar"));
        verticalLayout_topinfobar->setContentsMargins(0, 0, 0, 0);
        topinfobar_shortcut = new QFrame(topinfobar);
        topinfobar_shortcut->setObjectName(QString::fromUtf8("topinfobar_shortcut"));
        horizontalLayout_topinfobar_shortcut = new QHBoxLayout(topinfobar_shortcut);
        horizontalLayout_topinfobar_shortcut->setSpacing(0);
        horizontalLayout_topinfobar_shortcut->setObjectName(QString::fromUtf8("horizontalLayout_topinfobar_shortcut"));
        horizontalLayout_topinfobar_shortcut->setContentsMargins(0, 2, 0, 0);
        btnCoor = new QPushButton(topinfobar_shortcut);
        btnCoor->setObjectName(QString::fromUtf8("btnCoor"));

        horizontalLayout_topinfobar_shortcut->addWidget(btnCoor);

        btnT0 = new QPushButton(topinfobar_shortcut);
        btnT0->setObjectName(QString::fromUtf8("btnT0"));

        horizontalLayout_topinfobar_shortcut->addWidget(btnT0);

        btnT1 = new QPushButton(topinfobar_shortcut);
        btnT1->setObjectName(QString::fromUtf8("btnT1"));

        horizontalLayout_topinfobar_shortcut->addWidget(btnT1);

        btnST = new QPushButton(topinfobar_shortcut);
        btnST->setObjectName(QString::fromUtf8("btnST"));

        horizontalLayout_topinfobar_shortcut->addWidget(btnST);

        btnYouWei = new QPushButton(topinfobar_shortcut);
        btnYouWei->setObjectName(QString::fromUtf8("btnYouWei"));

        horizontalLayout_topinfobar_shortcut->addWidget(btnYouWei);

        btnYouWen = new QPushButton(topinfobar_shortcut);
        btnYouWen->setObjectName(QString::fromUtf8("btnYouWen"));

        horizontalLayout_topinfobar_shortcut->addWidget(btnYouWen);

        btnHuoYan = new QPushButton(topinfobar_shortcut);
        btnHuoYan->setObjectName(QString::fromUtf8("btnHuoYan"));

        horizontalLayout_topinfobar_shortcut->addWidget(btnHuoYan);

        btnFloat = new QPushButton(topinfobar_shortcut);
        btnFloat->setObjectName(QString::fromUtf8("btnFloat"));

        horizontalLayout_topinfobar_shortcut->addWidget(btnFloat);

        btnJgtj = new QPushButton(topinfobar_shortcut);
        btnJgtj->setObjectName(QString::fromUtf8("btnJgtj"));

        horizontalLayout_topinfobar_shortcut->addWidget(btnJgtj);

        btnJgtjGt = new QPushButton(topinfobar_shortcut);
        btnJgtjGt->setObjectName(QString::fromUtf8("btnJgtjGt"));

        horizontalLayout_topinfobar_shortcut->addWidget(btnJgtjGt);

        horizontalSpacer1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_topinfobar_shortcut->addItem(horizontalSpacer1);


        verticalLayout_topinfobar->addWidget(topinfobar_shortcut);

        horizontalLayout_topinfobar_center = new QHBoxLayout();
        horizontalLayout_topinfobar_center->setSpacing(0);
        horizontalLayout_topinfobar_center->setObjectName(QString::fromUtf8("horizontalLayout_topinfobar_center"));
        horizontalLayout_topinfobar_center->setSizeConstraint(QLayout::SetDefaultConstraint);
        verticalLayout_pos = new QVBoxLayout();
        verticalLayout_pos->setSpacing(0);
        verticalLayout_pos->setObjectName(QString::fromUtf8("verticalLayout_pos"));
        verticalLayout_pos->setContentsMargins(0, 0, 0, 0);
        topinfobar_pos = new QFrame(topinfobar);
        topinfobar_pos->setObjectName(QString::fromUtf8("topinfobar_pos"));
        sizePolicy.setHeightForWidth(topinfobar_pos->sizePolicy().hasHeightForWidth());
        topinfobar_pos->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(topinfobar_pos);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        xLabel = new QLabel(topinfobar_pos);
        xLabel->setObjectName(QString::fromUtf8("xLabel"));
        xLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        verticalLayout->addWidget(xLabel);

        yLabel = new QLabel(topinfobar_pos);
        yLabel->setObjectName(QString::fromUtf8("yLabel"));
        yLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        verticalLayout->addWidget(yLabel);

        zLabel = new QLabel(topinfobar_pos);
        zLabel->setObjectName(QString::fromUtf8("zLabel"));
        zLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        verticalLayout->addWidget(zLabel);


        verticalLayout_pos->addWidget(topinfobar_pos);

        topinfobar_pos_uw = new QFrame(topinfobar);
        topinfobar_pos_uw->setObjectName(QString::fromUtf8("topinfobar_pos_uw"));
        horizontalLayout_2 = new QHBoxLayout(topinfobar_pos_uw);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        uLabel = new QLabel(topinfobar_pos_uw);
        uLabel->setObjectName(QString::fromUtf8("uLabel"));
        uLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_2->addWidget(uLabel);

        wLabel = new QLabel(topinfobar_pos_uw);
        wLabel->setObjectName(QString::fromUtf8("wLabel"));
        wLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_2->addWidget(wLabel);


        verticalLayout_pos->addWidget(topinfobar_pos_uw);


        horizontalLayout_topinfobar_center->addLayout(verticalLayout_pos);

        verticalLayout_loran = new QVBoxLayout();
        verticalLayout_loran->setSpacing(0);
        verticalLayout_loran->setObjectName(QString::fromUtf8("verticalLayout_loran"));
        loran = new QFrame(topinfobar);
        loran->setObjectName(QString::fromUtf8("loran"));
        loran->setFrameShape(QFrame::Box);

        verticalLayout_loran->addWidget(loran);

        label_loran = new QLabel(topinfobar);
        label_loran->setObjectName(QString::fromUtf8("label_loran"));
        label_loran->setAlignment(Qt::AlignCenter);

        verticalLayout_loran->addWidget(label_loran);


        horizontalLayout_topinfobar_center->addLayout(verticalLayout_loran);

        verticalLayout_displayz = new QVBoxLayout();
        verticalLayout_displayz->setSpacing(0);
        verticalLayout_displayz->setObjectName(QString::fromUtf8("verticalLayout_displayz"));
        displayz = new QFrame(topinfobar);
        displayz->setObjectName(QString::fromUtf8("displayz"));
        displayz->setFrameShape(QFrame::Box);

        verticalLayout_displayz->addWidget(displayz);

        label_displayz = new QLabel(topinfobar);
        label_displayz->setObjectName(QString::fromUtf8("label_displayz"));
        label_displayz->setAlignment(Qt::AlignCenter);

        verticalLayout_displayz->addWidget(label_displayz);


        horizontalLayout_topinfobar_center->addLayout(verticalLayout_displayz);

        verticalLayout_numbers = new QVBoxLayout();
        verticalLayout_numbers->setSpacing(0);
        verticalLayout_numbers->setObjectName(QString::fromUtf8("verticalLayout_numbers"));
        frame_numbers = new QFrame(topinfobar);
        frame_numbers->setObjectName(QString::fromUtf8("frame_numbers"));
        gridLayout = new QGridLayout(frame_numbers);
        gridLayout->setSpacing(10);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        gridLayout->setContentsMargins(10, 2, 10, 0);
        labelLength0 = new QLabel(frame_numbers);
        labelLength0->setObjectName(QString::fromUtf8("labelLength0"));

        gridLayout->addWidget(labelLength0, 1, 0, 1, 1);

        labelLength = new QLabel(frame_numbers);
        labelLength->setObjectName(QString::fromUtf8("labelLength"));
        labelLength->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(labelLength, 1, 1, 1, 1);

        labelCNo0 = new QLabel(frame_numbers);
        labelCNo0->setObjectName(QString::fromUtf8("labelCNo0"));

        gridLayout->addWidget(labelCNo0, 1, 2, 1, 1);

        labelCNo = new QLabel(frame_numbers);
        labelCNo->setObjectName(QString::fromUtf8("labelCNo"));
        labelCNo->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(labelCNo, 1, 3, 1, 1);

        labelRemnantLength0 = new QLabel(frame_numbers);
        labelRemnantLength0->setObjectName(QString::fromUtf8("labelRemnantLength0"));

        gridLayout->addWidget(labelRemnantLength0, 2, 0, 1, 1);

        labelRemnantLength = new QLabel(frame_numbers);
        labelRemnantLength->setObjectName(QString::fromUtf8("labelRemnantLength"));
        labelRemnantLength->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(labelRemnantLength, 2, 1, 1, 1);

        labelTNo0 = new QLabel(frame_numbers);
        labelTNo0->setObjectName(QString::fromUtf8("labelTNo0"));

        gridLayout->addWidget(labelTNo0, 2, 2, 1, 1);

        labelTNo = new QLabel(frame_numbers);
        labelTNo->setObjectName(QString::fromUtf8("labelTNo"));
        labelTNo->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(labelTNo, 2, 3, 1, 1);

        labelTotalTime0 = new QLabel(frame_numbers);
        labelTotalTime0->setObjectName(QString::fromUtf8("labelTotalTime0"));

        gridLayout->addWidget(labelTotalTime0, 3, 0, 1, 1);

        labelTotalTime = new QLabel(frame_numbers);
        labelTotalTime->setObjectName(QString::fromUtf8("labelTotalTime"));
        labelTotalTime->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(labelTotalTime, 3, 1, 1, 1);

        labelProcessSpeed0 = new QLabel(frame_numbers);
        labelProcessSpeed0->setObjectName(QString::fromUtf8("labelProcessSpeed0"));

        gridLayout->addWidget(labelProcessSpeed0, 3, 2, 1, 1);

        labelProcessSpeed = new QLabel(frame_numbers);
        labelProcessSpeed->setObjectName(QString::fromUtf8("labelProcessSpeed"));
        labelProcessSpeed->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(labelProcessSpeed, 3, 3, 1, 1);

        labelProcessTime0 = new QLabel(frame_numbers);
        labelProcessTime0->setObjectName(QString::fromUtf8("labelProcessTime0"));

        gridLayout->addWidget(labelProcessTime0, 4, 0, 1, 1);

        labelProcessTime = new QLabel(frame_numbers);
        labelProcessTime->setObjectName(QString::fromUtf8("labelProcessTime"));
        labelProcessTime->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(labelProcessTime, 4, 1, 1, 1);

        labelProcessSpeed20 = new QLabel(frame_numbers);
        labelProcessSpeed20->setObjectName(QString::fromUtf8("labelProcessSpeed20"));

        gridLayout->addWidget(labelProcessSpeed20, 4, 2, 1, 1);

        labelProcessSpeed2 = new QLabel(frame_numbers);
        labelProcessSpeed2->setObjectName(QString::fromUtf8("labelProcessSpeed2"));
        labelProcessSpeed2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(labelProcessSpeed2, 4, 3, 1, 1);

        labelRemnantTime0 = new QLabel(frame_numbers);
        labelRemnantTime0->setObjectName(QString::fromUtf8("labelRemnantTime0"));

        gridLayout->addWidget(labelRemnantTime0, 5, 0, 1, 1);

        labelRemnantTime = new QLabel(frame_numbers);
        labelRemnantTime->setObjectName(QString::fromUtf8("labelRemnantTime"));
        labelRemnantTime->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(labelRemnantTime, 5, 1, 1, 1);

        labelG04RemnantTime0 = new QLabel(frame_numbers);
        labelG04RemnantTime0->setObjectName(QString::fromUtf8("labelG04RemnantTime0"));

        gridLayout->addWidget(labelG04RemnantTime0, 5, 2, 1, 1);

        labelG04RemnantTime = new QLabel(frame_numbers);
        labelG04RemnantTime->setObjectName(QString::fromUtf8("labelG04RemnantTime"));
        labelG04RemnantTime->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(labelG04RemnantTime, 5, 3, 1, 1);

        gridLayout->setColumnStretch(0, 1);
        gridLayout->setColumnStretch(1, 1);
        gridLayout->setColumnStretch(2, 1);
        gridLayout->setColumnStretch(3, 1);

        verticalLayout_numbers->addWidget(frame_numbers);

        frame_files = new QFrame(topinfobar);
        frame_files->setObjectName(QString::fromUtf8("frame_files"));
        horizontalLayout_filename = new QHBoxLayout(frame_files);
        horizontalLayout_filename->setSpacing(0);
        horizontalLayout_filename->setObjectName(QString::fromUtf8("horizontalLayout_filename"));
        horizontalLayout_filename->setContentsMargins(0, 0, 0, 0);
        labelFileName0 = new QLabel(frame_files);
        labelFileName0->setObjectName(QString::fromUtf8("labelFileName0"));

        horizontalLayout_filename->addWidget(labelFileName0);

        btnFileName = new QToolButton(frame_files);
        btnFileName->setObjectName(QString::fromUtf8("btnFileName"));
        sizePolicy.setHeightForWidth(btnFileName->sizePolicy().hasHeightForWidth());
        btnFileName->setSizePolicy(sizePolicy);

        horizontalLayout_filename->addWidget(btnFileName);

        lblNowDate = new QLabel(frame_files);
        lblNowDate->setObjectName(QString::fromUtf8("lblNowDate"));

        horizontalLayout_filename->addWidget(lblNowDate);

        lblNowTime = new QLabel(frame_files);
        lblNowTime->setObjectName(QString::fromUtf8("lblNowTime"));

        horizontalLayout_filename->addWidget(lblNowTime);


        verticalLayout_numbers->addWidget(frame_files);


        horizontalLayout_topinfobar_center->addLayout(verticalLayout_numbers);


        verticalLayout_topinfobar->addLayout(horizontalLayout_topinfobar_center);


        verticalLayout_left->addWidget(topinfobar);

        stackedChildWidget = new QStackedWidget(leftWidget);
        stackedChildWidget->setObjectName(QString::fromUtf8("stackedChildWidget"));
        sizePolicy.setHeightForWidth(stackedChildWidget->sizePolicy().hasHeightForWidth());
        stackedChildWidget->setSizePolicy(sizePolicy);

        verticalLayout_left->addWidget(stackedChildWidget);

        horizontalLayout_info = new QHBoxLayout();
        horizontalLayout_info->setSpacing(0);
        horizontalLayout_info->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_info->setObjectName(QString::fromUtf8("horizontalLayout_info"));
        lblResultMessage = new QLabel(leftWidget);
        lblResultMessage->setObjectName(QString::fromUtf8("lblResultMessage"));
        lblResultMessage->setEnabled(true);
        lblResultMessage->setAcceptDrops(false);

        horizontalLayout_info->addWidget(lblResultMessage);

        widgetInfoIcons2 = new QWidget(leftWidget);
        widgetInfoIcons2->setObjectName(QString::fromUtf8("widgetInfoIcons2"));
        horizontalLayout_infoicons2 = new QHBoxLayout(widgetInfoIcons2);
        horizontalLayout_infoicons2->setSpacing(0);
        horizontalLayout_infoicons2->setObjectName(QString::fromUtf8("horizontalLayout_infoicons2"));
        horizontalLayout_infoicons2->setContentsMargins(0, 0, 0, 0);
        labelManufactoryIcon = new QLabel(widgetInfoIcons2);
        labelManufactoryIcon->setObjectName(QString::fromUtf8("labelManufactoryIcon"));

        horizontalLayout_infoicons2->addWidget(labelManufactoryIcon);


        horizontalLayout_info->addWidget(widgetInfoIcons2);

        widgetInfoIcons = new QWidget(leftWidget);
        widgetInfoIcons->setObjectName(QString::fromUtf8("widgetInfoIcons"));
        horizontalLayout_infoicons = new QHBoxLayout(widgetInfoIcons);
        horizontalLayout_infoicons->setSpacing(5);
        horizontalLayout_infoicons->setObjectName(QString::fromUtf8("horizontalLayout_infoicons"));
        horizontalLayout_infoicons->setContentsMargins(10, 0, 10, 0);
        labelInfoLianjie = new QLabel(widgetInfoIcons);
        labelInfoLianjie->setObjectName(QString::fromUtf8("labelInfoLianjie"));
        labelInfoLianjie->setEnabled(false);

        horizontalLayout_infoicons->addWidget(labelInfoLianjie);

        labelInfoYouBang = new QLabel(widgetInfoIcons);
        labelInfoYouBang->setObjectName(QString::fromUtf8("labelInfoYouBang"));
        labelInfoYouBang->setEnabled(false);

        horizontalLayout_infoicons->addWidget(labelInfoYouBang);

        labelInfoJogSpeed = new QLabel(widgetInfoIcons);
        labelInfoJogSpeed->setObjectName(QString::fromUtf8("labelInfoJogSpeed"));
        labelInfoJogSpeed->setEnabled(false);

        horizontalLayout_infoicons->addWidget(labelInfoJogSpeed);

        labelInfoDiandong = new QLabel(widgetInfoIcons);
        labelInfoDiandong->setObjectName(QString::fromUtf8("labelInfoDiandong"));
        labelInfoDiandong->setEnabled(false);

        horizontalLayout_infoicons->addWidget(labelInfoDiandong);

        labelInfoFangdian = new QLabel(widgetInfoIcons);
        labelInfoFangdian->setObjectName(QString::fromUtf8("labelInfoFangdian"));
        labelInfoFangdian->setEnabled(false);

        horizontalLayout_infoicons->addWidget(labelInfoFangdian);

        labelInfoDuanLu = new QLabel(widgetInfoIcons);
        labelInfoDuanLu->setObjectName(QString::fromUtf8("labelInfoDuanLu"));

        horizontalLayout_infoicons->addWidget(labelInfoDuanLu);

        labelInfoYouWen = new QLabel(widgetInfoIcons);
        labelInfoYouWen->setObjectName(QString::fromUtf8("labelInfoYouWen"));

        horizontalLayout_infoicons->addWidget(labelInfoYouWen);

        labelInfoYouWei = new QLabel(widgetInfoIcons);
        labelInfoYouWei->setObjectName(QString::fromUtf8("labelInfoYouWei"));

        horizontalLayout_infoicons->addWidget(labelInfoYouWei);

        labelInfoZhaoMing = new QLabel(widgetInfoIcons);
        labelInfoZhaoMing->setObjectName(QString::fromUtf8("labelInfoZhaoMing"));

        horizontalLayout_infoicons->addWidget(labelInfoZhaoMing);

        horizontalSpacer2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_infoicons->addItem(horizontalSpacer2);


        horizontalLayout_info->addWidget(widgetInfoIcons);

        horizontalLayout_info->setStretch(0, 6);
        horizontalLayout_info->setStretch(1, 1);
        horizontalLayout_info->setStretch(2, 5);

        verticalLayout_left->addLayout(horizontalLayout_info);


        horizontalLayout_1->addWidget(leftWidget);

        groupBoxMenus = new QGroupBox(mainWidget);
        groupBoxMenus->setObjectName(QString::fromUtf8("groupBoxMenus"));
        sizePolicy.setHeightForWidth(groupBoxMenus->sizePolicy().hasHeightForWidth());
        groupBoxMenus->setSizePolicy(sizePolicy);
        verticalLayout_groupBoxMenus = new QVBoxLayout(groupBoxMenus);
        verticalLayout_groupBoxMenus->setSpacing(0);
        verticalLayout_groupBoxMenus->setObjectName(QString::fromUtf8("verticalLayout_groupBoxMenus"));
        verticalLayout_groupBoxMenus->setContentsMargins(0, 2, 0, 0);

        horizontalLayout_1->addWidget(groupBoxMenus);

        horizontalLayout_1->setStretch(0, 7);
        horizontalLayout_1->setStretch(1, 1);
        QtWin2->setCentralWidget(mainWidget);
        statusbar = new QStatusBar(QtWin2);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        statusbar->setSizeGripEnabled(false);
        QtWin2->setStatusBar(statusbar);
        toolBar = new QToolBar(QtWin2);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        QtWin2->addToolBar(Qt::TopToolBarArea, toolBar);

        toolBar->addAction(actionRun);
        toolBar->addAction(actionCancel);
        toolBar->addSeparator();
        toolBar->addAction(actionDebug);
        toolBar->addAction(actionTouchSense);
        toolBar->addAction(actionTouchSense2);
        toolBar->addAction(actionLoadStylesheet);
        toolBar->addAction(actionPythonScript);
        toolBar->addAction(actionPowerOn);
        toolBar->addAction(actionPowerOff);
        toolBar->addAction(actionSaveScreen);
        toolBar->addAction(actionSaveGCode);

        retranslateUi(QtWin2);

        QMetaObject::connectSlotsByName(QtWin2);
    } // setupUi

    void retranslateUi(QMainWindow *QtWin2)
    {
        QtWin2->setWindowTitle(QCoreApplication::translate("QtWin2", "EDM", nullptr));
        actionRun->setText(QCoreApplication::translate("QtWin2", "\350\277\220\350\241\214", nullptr));
        actionCancel->setText(QCoreApplication::translate("QtWin2", "\345\217\226\346\266\210", nullptr));
        actionDebug->setText(QCoreApplication::translate("QtWin2", "\350\260\203\350\257\225", nullptr));
        actionTouchSense->setText(QCoreApplication::translate("QtWin2", "\347\237\255\350\267\2571", nullptr));
        actionTouchSense2->setText(QCoreApplication::translate("QtWin2", "\347\237\255\350\267\2572", nullptr));
        actionLoadStylesheet->setText(QCoreApplication::translate("QtWin2", "SS", nullptr));
        actionPythonScript->setText(QCoreApplication::translate("QtWin2", "Script", nullptr));
        actionPowerOn->setText(QCoreApplication::translate("QtWin2", "PowerOn", nullptr));
        actionPowerOff->setText(QCoreApplication::translate("QtWin2", "PowerOff", nullptr));
        actionSaveScreen->setText(QCoreApplication::translate("QtWin2", "SendLog", nullptr));
        actionSaveGCode->setText(QCoreApplication::translate("QtWin2", "SaveGCode", nullptr));
        btnCoor->setText(QCoreApplication::translate("QtWin2", "G54", nullptr));
        btnT0->setText(QCoreApplication::translate("QtWin2", "T0", nullptr));
        btnT1->setText(QCoreApplication::translate("QtWin2", "T1", nullptr));
        btnST->setText(QCoreApplication::translate("QtWin2", "ST", nullptr));
        btnYouWei->setText(QCoreApplication::translate("QtWin2", "\346\262\271\344\275\215", nullptr));
        btnYouWen->setText(QCoreApplication::translate("QtWin2", "\346\262\271\346\270\251", nullptr));
        btnHuoYan->setText(QCoreApplication::translate("QtWin2", "\347\201\253\347\204\260", nullptr));
        btnFloat->setText(QCoreApplication::translate("QtWin2", "FLOAT", nullptr));
        btnJgtj->setText(QCoreApplication::translate("QtWin2", "\345\212\240\345\267\245\346\235\241\344\273\266", nullptr));
        btnJgtjGt->setText(QCoreApplication::translate("QtWin2", "\351\253\230\350\267\263", nullptr));
#if QT_CONFIG(tooltip)
        xLabel->setToolTip(QCoreApplication::translate("QtWin2", "X", nullptr));
#endif // QT_CONFIG(tooltip)
        xLabel->setText(QCoreApplication::translate("QtWin2", "+00000.000", nullptr));
#if QT_CONFIG(tooltip)
        yLabel->setToolTip(QCoreApplication::translate("QtWin2", "Y", nullptr));
#endif // QT_CONFIG(tooltip)
        yLabel->setText(QCoreApplication::translate("QtWin2", "+00000.000", nullptr));
#if QT_CONFIG(tooltip)
        zLabel->setToolTip(QCoreApplication::translate("QtWin2", "Z", nullptr));
#endif // QT_CONFIG(tooltip)
        zLabel->setText(QCoreApplication::translate("QtWin2", "-00000.000", nullptr));
        uLabel->setText(QCoreApplication::translate("QtWin2", "+00000.000", nullptr));
        wLabel->setText(QCoreApplication::translate("QtWin2", "+00000.000", nullptr));
        label_loran->setText(QCoreApplication::translate("QtWin2", "\345\271\263\345\212\250\350\275\250\350\277\271", nullptr));
        label_displayz->setText(QCoreApplication::translate("QtWin2", "\347\224\265\345\216\213\346\225\210\347\216\207\345\233\276", nullptr));
        labelLength0->setText(QCoreApplication::translate("QtWin2", "\345\205\250\351\225\277(mm)", nullptr));
        labelLength->setText(QCoreApplication::translate("QtWin2", "0.0000", nullptr));
        labelCNo0->setText(QCoreApplication::translate("QtWin2", "\346\235\241\344\273\266\345\217\267", nullptr));
        labelCNo->setText(QCoreApplication::translate("QtWin2", "C000", nullptr));
        labelRemnantLength0->setText(QCoreApplication::translate("QtWin2", "\346\256\213\344\275\231\351\207\217(mm)", nullptr));
        labelRemnantLength->setText(QCoreApplication::translate("QtWin2", "0.0000", nullptr));
        labelTNo0->setText(QCoreApplication::translate("QtWin2", "\345\267\245\345\205\267\345\217\267", nullptr));
        labelTNo->setText(QCoreApplication::translate("QtWin2", "T000", nullptr));
        labelTotalTime0->setText(QCoreApplication::translate("QtWin2", "\346\200\273\347\232\204\345\212\240\345\267\245\346\227\266\351\227\264", nullptr));
        labelTotalTime->setText(QCoreApplication::translate("QtWin2", "00:00:00", nullptr));
        labelProcessSpeed0->setText(QCoreApplication::translate("QtWin2", "\345\212\240\345\267\245\351\200\237\345\272\246", nullptr));
        labelProcessSpeed->setText(QCoreApplication::translate("QtWin2", "0.0000", nullptr));
        labelProcessTime0->setText(QCoreApplication::translate("QtWin2", "\346\255\244\346\256\265\345\212\240\345\267\245\346\227\266\351\227\264", nullptr));
        labelProcessTime->setText(QCoreApplication::translate("QtWin2", "00:00:00", nullptr));
        labelProcessSpeed20->setText(QCoreApplication::translate("QtWin2", "\346\227\213\350\275\254\350\275\264\351\200\237\345\272\246", nullptr));
        labelProcessSpeed2->setText(QCoreApplication::translate("QtWin2", "0.0000", nullptr));
        labelRemnantTime0->setText(QCoreApplication::translate("QtWin2", "\346\255\244\346\256\265\346\256\213\344\275\231\346\227\266\351\227\264", nullptr));
        labelRemnantTime->setText(QCoreApplication::translate("QtWin2", "00:00:00", nullptr));
        labelG04RemnantTime0->setText(QCoreApplication::translate("QtWin2", "G04\346\256\213\344\275\231\346\227\266\351\227\264", nullptr));
        labelG04RemnantTime->setText(QCoreApplication::translate("QtWin2", "00:00:00", nullptr));
        labelFileName0->setText(QCoreApplication::translate("QtWin2", "\346\226\207\344\273\266\345\220\215\357\274\232", nullptr));
        btnFileName->setText(QCoreApplication::translate("QtWin2", "Default", nullptr));
        lblNowDate->setText(QCoreApplication::translate("QtWin2", "2024-02-20", nullptr));
        lblNowTime->setText(QCoreApplication::translate("QtWin2", "09:03:29", nullptr));
        lblResultMessage->setText(QCoreApplication::translate("QtWin2", "ErrorMessage", nullptr));
        labelManufactoryIcon->setText(QString());
        labelInfoLianjie->setText(QString());
        labelInfoYouBang->setText(QString());
        labelInfoJogSpeed->setText(QString());
        labelInfoDiandong->setText(QString());
        labelInfoFangdian->setText(QString());
        labelInfoDuanLu->setText(QString());
        labelInfoYouWen->setText(QString());
        labelInfoYouWei->setText(QString());
        labelInfoZhaoMing->setText(QString());
        groupBoxMenus->setTitle(QCoreApplication::translate("QtWin2", "\350\217\234\345\215\225", nullptr));
        toolBar->setWindowTitle(QCoreApplication::translate("QtWin2", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QtWin2: public Ui_QtWin2 {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW2_H
