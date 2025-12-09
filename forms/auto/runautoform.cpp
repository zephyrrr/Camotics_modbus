#include "runautoform.h"
#include "ui_runautoform.h"
#include <QLabel>
#include <QButtonGroup>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include "../modbus/NCMachine.h"
#include "utils/dataform.h"
#include "utils/pluginutils.h"
#include "BaseMainWindow.h"
#include "utils/DataTable.h"
#include "utils/formutils.h"
#include "runautoparamform.h"
#include "runautoone.h"
#include "runautomulti.h"

RunAutoForm::RunAutoForm(QWidget *parent) :
    BaseChildWindow(parent),
    ui(new Ui::RunAutoForm)
{
    ui->setupUi(this);

    // Create instances of your windows
    windowA = new RunAutoParamForm(this);
    windowB = new RunAutoMulti(this);
    windowC = new RunAutoOne(this);

    // Add the windows to the tab widget
    ui->tabWidget->addTab(windowA, tr("JGJH"));
    ui->tabWidget->addTab(windowB, tr("JGWZ"));
    ui->tabWidget->addTab(windowC, tr("TJXX"));

    tabChangedConnection = connect(ui->tabWidget, &QTabWidget::currentChanged, [this](int index) {
        if (index == 2) {
            QString s = windowA->GetGCode();
            if (s.isEmpty()) {
                ui->tabWidget->setCurrentIndex(0);
                FormUtils::MessageBoxInfo(tr("MYXYSJ！"));
            }
            else {
                windowB->LoadData();
                windowC->LoadData();
            }
        }
        });
}

RunAutoForm::~RunAutoForm()
{
    if (tabChangedConnection) {
        QObject::disconnect(tabChangedConnection);
    }

    delete ui;
    delete windowA;
	delete windowB;
	delete windowC;
}

void RunAutoForm::RunGCode()
{
}

void RunAutoForm::SetMachine(NCMachine* ncMachine)
{
	BaseChildWindow::SetMachine(ncMachine);
	for (int i = 0; i < ui->tabWidget->count(); ++i)
	{
		auto window = qobject_cast<BaseChildWindow*>(ui->tabWidget->widget(i));
		if (window)
		{
			window->SetMachine(ncMachine);
		}
	}
}


