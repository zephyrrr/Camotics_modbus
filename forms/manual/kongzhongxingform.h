#ifndef KONGZHONGXINGFORM_H
#define KONGZHONGXINGFORM_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include "BaseChildWindow.h"
#include "../component/tool4zeroandhalfaxis.h"

namespace Ui {
class KongZhongXingForm;
}

class KongZhongXingForm : public BaseChildWindowWithTools
{
    Q_OBJECT

public:
    explicit KongZhongXingForm(QWidget *parent = nullptr);
    ~KongZhongXingForm();

private:
    Ui::KongZhongXingForm*ui;


public:
    void RunGCode() override;
    QString GetGCode(bool forRun = true) override;

private slots:
    void on_btnX_clicked();
    void on_btnY_clicked();
    void on_btnZ_clicked();
};

#endif
