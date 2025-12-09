#ifndef FANGDIANFORM_H
#define FANGDIANFORM_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include "BaseChildWindow.h"
#include "../component/tool4zeroandhalfaxis.h"

namespace Ui {
class FangDianForm;
}

class FangDianCanShuSingleForm;

class FangDianForm : public BaseChildWindowWithTools
{
    Q_OBJECT

public:
    explicit FangDianForm(QWidget *parent = nullptr);
    ~FangDianForm();

private:
    Ui::FangDianForm*ui;
    FangDianCanShuSingleForm* m_fangdiancheshiForm = NULL;
    QList<QPushButton*> m_inputBtns;

public:
    void RunGCode() override;

private slots:
    void on_btnX_clicked();
    void on_btnY_clicked();
    void on_btnZ_clicked();
    void on_btnU_clicked();

    void on_inJgtj_textChanged(const QString& text);
};

#endif
