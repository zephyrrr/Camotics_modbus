#ifndef JIAODINGWEIFORM_H
#define JIAODINGWEIFORM_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include "BaseChildWindow.h"
#include "../component/tool4zeroandhalfaxis.h"


namespace Ui {
class JiaoDingWeiForm;
}

class JiaoDingWeiForm : public BaseChildWindowWithTools
{
    Q_OBJECT

public:
    explicit JiaoDingWeiForm(QWidget *parent = nullptr);
    ~JiaoDingWeiForm();

private:
    Ui::JiaoDingWeiForm*ui;

public:
    void RunGCode() override;
    QString GetGCode(bool forRun = true) override;

private slots:
    void on_btnX_clicked();
    void on_btnY_clicked();
    void on_btnZ_clicked();
};

#endif
