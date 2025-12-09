#ifndef ZHUZHONGXINGFORM_H
#define ZHUZHONGXINGFORM_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QTableView>
#include <QStandardItemModel>
#include "BaseChildWindow.h"
#include "../component/tool4zeroandhalfaxis.h"

namespace Ui {
class ZhuZhongXingForm;
}

class ZhuZhongXingForm : public BaseChildWindowWithTools
{
    Q_OBJECT

public:
    explicit ZhuZhongXingForm(QWidget *parent = nullptr);
    ~ZhuZhongXingForm();

private:
    Ui::ZhuZhongXingForm*ui;

public:
    void RunGCode() override;
	QString GetGCode() override;

    static void toggleLineEdit(bool readonly, QLineEdit* inEdit);
    static void toggleButtonEdit(bool checkable, QPushButton* btnP, QPushButton* btnN);
    static void setButton4LineEdit(QPushButton* btnX);
    static void setButton4ButtonEdit(QPushButton* btnX);

    static void ClearTable4Result(QTableView* tableView);
    static QStandardItemModel* SetupTable4Result(QTableView* tableView, QStringList headerNames = {}, int rowCount=3);
    static void AdjustTableViewHeight(QTableView* tableView);
    static void AddTable4Result(QTableView* tableView, QString s);

private slots:
    void on_btnX_clicked();
    void on_btnY_clicked();
    void on_btnZ_clicked();
};

#endif
