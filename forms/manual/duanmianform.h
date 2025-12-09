#ifndef DUANMIANFORM_H
#define DUANMIANFORM_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include "BaseChildWindow.h"
#include "../component/tool4zeroandhalfaxis.h"


namespace Ui {
class DuanMianForm;
}

class DuanMianForm : public BaseChildWindowWithTools
{
    Q_OBJECT

public:
    explicit DuanMianForm(QWidget *parent = nullptr);
    ~DuanMianForm();

private:
    Ui::DuanMianForm *ui;
    QList<QPushButton*> m_inputBtns;

public:
    void RunGCode() override;

private slots:
    void on_btnX_clicked();
    void on_btnY_clicked();
    void on_btnZ_clicked();
    void on_btnU_clicked();
};

#endif // DUANMIANFORM_H
