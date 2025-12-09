#pragma once

#include <QWidget>
#include <QMessageBox>
#include "BaseChildWindow.h"
#include "widgets/spreadsheet.h"
#include "widgets/buttonedittablewidget.h"
#include "utils/DataTable.h"
#include "../component/tool4zeroandhalfaxis.h"
#include "../run/runmanualmulti.h"

class RunAutoMulti : public RunManualMulti
{
	Q_OBJECT

public:
	RunAutoMulti(QWidget* parent = nullptr);
	~RunAutoMulti();

protected:
	void showEvent(QShowEvent* event) override;
};
