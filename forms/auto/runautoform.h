#ifndef RUNAUTOFORM_H
#define RUNAUTOFORM_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include "BaseChildWindow.h"
#include "../component/tool4zeroandhalfaxis.h"


namespace Ui {
class RunAutoForm;
}

class RunAutoParamForm;
class RunAutoOne;
class RunAutoMulti;

class RunAutoForm : public BaseChildWindow
{
    Q_OBJECT

public:
    explicit RunAutoForm(QWidget *parent = nullptr);
    ~RunAutoForm();

    void RunGCode() override;
    void SetMachine(NCMachine* ncMachine) override;
private:
    Ui::RunAutoForm*ui;
    QMetaObject::Connection tabChangedConnection;

public:
    RunAutoParamForm* windowA;
    RunAutoMulti* windowB;
    RunAutoOne* windowC;
};

#endif // RUNAUTOFORM_H
