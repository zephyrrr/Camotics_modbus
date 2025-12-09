#ifndef WEIZHIYIDONGFORM_H
#define WEIZHIYIDONGFORM_H

#include <QWidget>
#include "BaseChildWindow.h"
#include "../component/tool4zeroandhalfaxis.h"

namespace Ui {
class WeiZhiYiDongForm;
}

class WeiZhiYiDongForm : public BaseChildWindowWithTools
{
    Q_OBJECT

public:
    explicit WeiZhiYiDongForm(QWidget *parent = nullptr);
    ~WeiZhiYiDongForm();

private:
    Ui::WeiZhiYiDongForm *ui;

public:
    void RunGCode() override;
    void ClearInputWidgets(QWidget* parent = NULL) override;

private slots:
    void on_btnX_clicked();
    void on_btnY_clicked();
    void on_btnZ_clicked();
    void on_btnU_clicked();

    void on_btnX_2_clicked();
    void on_btnY_2_clicked();
    void on_btnZ_2_clicked();
    void on_btnU_2_clicked();

    void on_btnX_3_clicked();
    void on_btnY_3_clicked();
    void on_btnZ_3_clicked();
    void on_btnU_3_clicked();
};

#endif // WEIZHIYIDONGFORM_H
