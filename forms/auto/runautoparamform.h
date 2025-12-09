#ifndef RUNAUTOPARAMFORM_H
#define RUNAUTOPARAMFORM_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include "BaseChildWindow.h"
#include "../component/tool4zeroandhalfaxis.h"


namespace Ui {
class RunAutoParamForm;
}

class RunAutoParamForm : public BaseChildWindow
{
    Q_OBJECT

public:
    explicit RunAutoParamForm(QWidget *parent = nullptr);
    ~RunAutoParamForm();

private:
    Ui::RunAutoParamForm*ui;
    Tool4All* m_tool4ZeroAndHalfAxis = NULL;

protected:
    void showEvent(QShowEvent* event) override;

public:
    void RunGCode() override { GetGCode(); }
    QString GetGCode() override;
    QString GenerateManualData(QString parentName);
    QString GenerateManualData_V1(QString parentName);
};

#endif // RUNAUTOPARAMFORM_H
