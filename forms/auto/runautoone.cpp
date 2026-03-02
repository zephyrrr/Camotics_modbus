#include "runautoone.h"
#include <QHeaderView>
#include <QComboBox>
#include <QCompleter>
#include "widgets/qlineedit4axisvalue.h"
#include "modbus/NCMachineParametersC.h"
#include "../manual/zhuzhongxingform.h"
#include "widgets/multibuttonswidget.h"
#include "modbus/NCMachine.h"
#include "../../utils/gcodeutils.h"
#include "forms/component/tool4buttons.h"
#include "runautoform.h"
#include "runautoparamform.h"
#include "runautomulti.h"
#include "../../utils/formutils.h"
#include "utils/nfile.h"
#include "widgets/filedialog.h"
#include "BaseMainWindow.h"
#include "forms/edit/nceditform.h"
#include "utils/dataform.h"
#include "runautoform.h"

RunAutoOne::RunAutoOne(QWidget* parent)
	: RunManual(parent, "runautoone")
{
	mbw->removeButton(5);
	mbw->removeButton(4);

	//ui.labelPix->hide();
	//ui.textBrowser->hide();
	table1->hide();

	ui.gridLayout->removeWidget(ui.label_3);
	ui.gridLayout->removeItem(ui.horizontalLayout);
	ui.gridLayout->removeWidget(ui.label_1);
	ui.gridLayout->removeItem(ui.verticalLayout_table1);
	ui.verticalLayout_2->removeItem(ui.verticalSpacer1);
	//ui.verticalLayout_2->removeItem(ui.verticalSpacer2);
	table2->setMinimumHeight(500);

	ui.label_3->hide();
	ui.label_1->hide();
	ui.inAbsolute->hide();
	ui.inAbsolute2->hide();
}

RunAutoOne::~RunAutoOne()
{
}

void RunAutoOne::ExportData()
{
	if (this->table2->getDataCount() == 0) {
		return;
	}
	RunAutoForm* w = this->GetParentOfThis<RunAutoForm>(this);
	w->windowB->SaveData();

	{
		QString parentName = "runautoone";
		QString filePath1 = SystemSettings::GetDataFilePath(parentName, GetProjectDir());
		filePath1 = SystemSettings::AppendDataFilePath(filePath1, QString("%1_table1").arg(parentName));
		QString filePath2 = SystemSettings::GetDataFilePath(parentName, GetProjectDir());
		filePath2 = SystemSettings::AppendDataFilePath(filePath2, QString("%1_table2").arg(parentName));

		QString parentNameDest = "sdjg";
		QString filePath2Dest = SystemSettings::GetDataFilePath(parentNameDest, GetProjectDir());
		filePath2Dest = SystemSettings::AppendDataFilePath(filePath2Dest, QString("%1_table2").arg(parentNameDest));
		FormUtils::CopyFileWithOverwrite(filePath2, filePath2Dest);

		QString filePath1Dest = SystemSettings::GetDataFilePath(parentNameDest, GetProjectDir());
		filePath1Dest = SystemSettings::AppendDataFilePath(filePath1Dest, QString("%1_table1").arg(parentNameDest));
		FormUtils::CopyFileWithOverwrite(filePath1, filePath1Dest);

		QString filePath4Dest = SystemSettings::GetDataFilePath(parentNameDest, SystemSettings::instance().GetProjectDir());
		DataForm* dataFormDest = DataForms::getInstance()->getDataForm(parentNameDest, SystemSettings::instance().GetProjectDir());
		if (w->windowA->GetData("inJgz") == "XYZ") {
			dataFormDest->setValue("inAbsolute", "false");
			dataFormDest->setValue("inAbsolute2", "true");
		}
		else {
			dataFormDest->setValue("inAbsolute", "true");
			dataFormDest->setValue("inAbsolute2", "false");
		}
		dataFormDest->serialize(filePath4Dest);
	}

	{
		QString parentName = "runautomulti";
		QString filePath3 = SystemSettings::GetDataFilePath(parentName, GetProjectDir());
		filePath3 = SystemSettings::AppendDataFilePath(filePath3, QString("%1_table3").arg(parentName));

		QString parentNameDest = "ddjg";
		QString filePath3Dest = SystemSettings::GetDataFilePath(parentNameDest, GetProjectDir());
		filePath3Dest = SystemSettings::AppendDataFilePath(filePath3Dest, QString("%1_table3").arg(parentNameDest));
		FormUtils::CopyFileWithOverwrite(filePath3, filePath3Dest);

		QString filePath4Dest = SystemSettings::GetDataFilePath(parentNameDest, SystemSettings::instance().GetProjectDir());
		DataForm* dataForm = DataForms::getInstance()->getDataForm(parentName, SystemSettings::instance().GetProjectDir());
		DataForm* dataFormDest = DataForms::getInstance()->getDataForm(parentNameDest, SystemSettings::instance().GetProjectDir());
		dataFormDest->setValue("lineEditZAnQuan", dataForm->getValue("lineEditZAnQuan"));
		dataFormDest->setValue("lineEditZKaisi", dataForm->getValue("lineEditZKaisi"));
		dataFormDest->serialize(filePath4Dest);
	}
}

void RunAutoOne::showEvent(QShowEvent* event)
{
	BaseChildWindow::showEvent(event);
	
	if (m_tool4ZeroAndHalfAxis == NULL) {
		m_tool4ZeroAndHalfAxis = new Tool4All(m_ncMachine, this, 0);
		//QPushButton* btn1 = new QPushButton(tr("SDDMSC"), this);
		//m_tool4ZeroAndHalfAxis->AddItem(btn1);
		//QPushButton* btn2 = new QPushButton(tr("NWJSC"), this);
		Tool4Buttons* tools1 = new Tool4Buttons(m_tool4ZeroAndHalfAxis, QStringList() << tr("SDDMSC"));
		m_tool4ZeroAndHalfAxis->AddItem(tools1);
		Tool4Buttons* tools2 = new Tool4Buttons(m_tool4ZeroAndHalfAxis, QStringList() << tr("NWJSC"));
		m_tool4ZeroAndHalfAxis->AddItem(tools2);

		QPushButton* btn = tools1->getButton(0);
		connect(btn, &QPushButton::clicked, [this]() {
			
			this->SaveData();
			this->ExportData();

			BaseMainWindow* mainWindow = BaseChildWindow::GetMainWindow();
			mainWindow->showChildWindow(tr("SDDG"));
			});

		QPushButton* btn2 = tools2->getButton(0);
		connect(btn2, &QPushButton::clicked, [this]() {
			if (this->table2->getDataCount() == 0) {
				return;
			}

			this->SaveData();
			this->ExportData();

			RunAutoForm* w = this->GetParentOfThis<RunAutoForm>(this);
			w->windowB->LoadData();

			DataForm* dataForm = DataForms::getInstance()->getDataForm(this->objectName(), SystemSettings::instance().GetProjectDir());
			dataForm->setValue("GCODE_DBHHW", w->windowA->GetData("inDbhhw").toString());
			dataForm->setValue("GCODE_LN", w->windowA->GetData("inLN").toString());
			dataForm->setValue("GCODE_LP", w->windowA->GetData("inYdxz").toString());
			QString s = w->windowB->GetGCode(false);
			s = GCodeUtils::CleanGCode(s);
			dataForm->deleteItem("GCODE_DBHHW");
			dataForm->deleteItem("GCODE_LN");
			dataForm->deleteItem("GCODE_LP");


			QString m_path = "data/nc";
			if (!s.isEmpty()) {
				QString filePath = NFileDialog::getSaveFileName(this, tr("BCWJ"), m_path, QString("(*.nc)"));
				if (!filePath.isEmpty()) {

					NFile file(filePath);
					if (file.open(QIODevice::WriteOnly)) {
						file.write(s.toUtf8());
						file.close();

						BaseMainWindow* mainWindow = BaseChildWindow::GetMainWindow();
						mainWindow->showChildWindow(QString("NC"));

						NcEditForm* form = dynamic_cast<NcEditForm*>(mainWindow->getChildWindow(QString("NC")));
						form->OpenFile(filePath.toUtf8().data());
					}
				}
			}
			});

		Tool4All::ShowIt(m_tool4ZeroAndHalfAxis);
	}
}
