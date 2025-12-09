#pragma once

#include <QWidget>
#include <QStackedWidget>
#include "BaseChildWindow.h"
#include "widgets/spreadsheet.h"
#include "widgets/buttonedittablewidget.h"
#include "../component/tool4zeroandhalfaxis.h"
#include "../other/fangdiancheshiform.h"
#include "../run/runmanual.h"

class RunAutoOne : public RunManual
{
	Q_OBJECT

public:
	RunAutoOne(QWidget* parent = nullptr);
	~RunAutoOne();

protected:
	void showEvent(QShowEvent* event) override;
};
