#ifndef SANDIANDINGWEIFORM_H
#define SANDIANDINGWEIFORM_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include "BaseChildWindow.h"
#include "../component/tool4zeroandhalfaxis.h"


namespace Ui {
class SanDianDingWeiForm;
}

class SanDianDingWeiForm : public BaseChildWindowWithTools
{
    Q_OBJECT

public:
    explicit SanDianDingWeiForm(QWidget *parent = nullptr);
    ~SanDianDingWeiForm();

private:
    Ui::SanDianDingWeiForm*ui;

public:
    void RunGCode() override;

private slots:
    void on_btnX_clicked();
    void on_btnA1_clicked();
    void on_btnA2_clicked();
    void on_btnA3_clicked();
    void on_btnZ_clicked();
};

#endif
