#pragma once

#include <QPointer>
#include <QDialog>
#include "ui_qnumericpad.h"

class QNumericPad : public QDialog
{
	Q_OBJECT

public:
	QNumericPad(QWidget *parent = nullptr);
    
	~QNumericPad();
    QWidget* getReceiver();
    void setReceiver(QWidget* receiver);
    QString getText() { return m_text; }

    static void setButtonDownModeWhenInTouch(QWidget* parent, QAbstractButton* button=NULL);

private:
	Ui::QNumericPadClass ui;
    QPointer<QWidget> m_receiver = NULL;
    QString m_text;
private slots:
    void on_pushButtonKey0_clicked();

    void on_pushButtonKey1_clicked();

    void on_pushButtonKey2_clicked();

    void on_pushButtonKey3_clicked();

    void on_pushButtonKey4_clicked();

    void on_pushButtonKey5_clicked();

    void on_pushButtonKey6_clicked();

    void on_pushButtonKey7_clicked();

    void on_pushButtonKey8_clicked();

    void on_pushButtonKey9_clicked();

    void on_pushButtonKeyDot_clicked();

    void on_pushButtonKeyAdd_clicked();
    void on_pushButtonKeySub_clicked();
    void on_pushButtonKeyHalf_clicked();

    void on_pushButtonKeyBack_clicked();

    void on_pushButtonKeyEnter_clicked();
    void on_pushButtonKeyDel_clicked();
    void on_pushButtonKeyCancel_clicked();
};
