#include "runautoparamform.h"
#include "ui_runautoparamform.h"
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
#include "modbus/NCMachineParametersC.h"
#include "utils/QTextFileModel.h"

RunAutoParamForm::RunAutoParamForm(QWidget* parent) :
	BaseChildWindow(parent),
	ui(new Ui::RunAutoParamForm)
{
	ui->setupUi(this);

	ui->inClzh->setModel(new QTextFileModel(this, GetDataFilePath(ui->inClzh)));
	ui->inDjxz->setModel(new QTextFileModel(this, GetDataFilePath(ui->inDjxz)));
	ui->inJgz->setModel(new QTextFileModel(this, GetDataFilePath(ui->inJgz)));
	ui->inJgmj->setModel(new QTextFileModel(this, GetDataFilePath(ui->inJgmj)));
	//ui->inJgmj->setModel(new QSqliteModel(this, "./data/cp.db", tr("SJGMJ JGMJ")));
	ui->inCcd->setModel(new QTextFileModel(this, GetDataFilePath(ui->inCcd)));
	ui->inYdxz->setModel(new QTextFileModel(this, GetDataFilePath(ui->inYdxz)));
	ui->inYdms->setModel(new QTextFileModel(this, GetDataFilePath(ui->inYdms)));
	ui->inJgcz->setModel(new QTextFileModel(this, GetDataFilePath(ui->inJgcz)));

	ui->inJgmj->setText("□18○20");

	ui->inYdxzSpecial->setValidator(new QRegExpValidator(QRegExp("^[1-5]{4}$"), ui->inYdxzSpecial));
	ui->inYdxzSpecial->setVisible(false);
	connect(ui->inYdxz, &QComboBox::currentTextChanged, this, [this](const QString& text) {
		if (text == ui->inYdxz->itemText(ui->inYdxz->count() - 1)) {
			ui->inYdxzSpecial->setVisible(true);
		}
		else {
			ui->inYdxzSpecial->setVisible(false);
		}
		});

	QT_TR_NOOP_UTF8("铜-钢");
}

RunAutoParamForm::~RunAutoParamForm()
{
    delete ui;
}


void RunAutoParamForm::showEvent(QShowEvent* event)
{
	BaseChildWindow::showEvent(event);

  //  if (m_tool4ZeroAndHalfAxis == NULL) {
  //      m_tool4ZeroAndHalfAxis = new Tool4ZeroAndHalfAxis(ncMachine, this);
		//Tool4ZeroAndHalfAxis::ShowIt(m_tool4ZeroAndHalfAxis);
  //  }
}

//void RunAutoParamForm::on_btnGenerate_clicked()
//{
//	RunGCode();
//}

QString RunAutoParamForm::GetGCode(){
	//return GenerateManualData_V1("runautoone");
	return GenerateManualData("runautoone");
}
// "runautoone"
QString RunAutoParamForm::GenerateManualData(QString parentName)
{
	QString cCode;
#ifdef PYTHON_SUPPORT
	QMap<QString, QString> pyFiles = PluginUtils::loadPythonScripts(this->objectName());
	for (const auto& pyFileName : pyFiles.keys()) {
		if (pyFileName == "rungcode") {
			this->SetData("result", NULL);

			//this->SetData("inClzh", ui->inClzh->text());
			this->SetData("inClzh", ui->inClzh->currentIndex() + 1);
			this->SetData("inDjxz", ui->inDjxz->currentIndex() + 1);
			this->SetData("inJgz", ui->inJgz->text());
			this->SetData("inJgsd", ui->inJgsd->text());
			this->SetData("inJgmj", ui->inJgmj->text());
			this->SetData("inCcd", ui->inCcd->text());
			QString inYdxz;
			if (ui->inYdxz->currentIndex() == 6) {
				inYdxz = ui->inYdxzSpecial->text();
			}
			else {
				inYdxz = QString::number(ui->inYdxz->currentIndex()).repeated(4);
			}
			this->SetData("inYdxz", inYdxz);
			this->SetData("inYdms", ui->inYdms->currentIndex());
			this->SetData("inJgcz", ui->inJgcz->currentIndex() + 1);
			this->SetData("inDbhhw", ui->inDbhhw->text());
			//this->SetData("inYdxzSpecial", ui->inYdxzSpecial->text());

			QString value = pyFiles.value(pyFileName);
			PluginUtils::RunFile(value, this);

			QVariant result = this->GetData("result");
			if (result != NULL && result.isValid()) {
				cCode = this->GetData("result").value<QString>();
			}
			break;
		}
	}
#endif
	cCode = cCode.trimmed();
	if (!cCode.isEmpty()) {
		QStringList ss = cCode.split('\n', Qt::SkipEmptyParts);

		QList<QString> cCodes;

		QList<QString> gSteps;
		QList<QString> gZs;
		QList<QString> gTzs;
		QString nowTz = "";

		QString ln, step, toz;
		for (int row = 0; row < ss.size(); ++row) {
			if (ss[row].startsWith("C")) {
				if (ss[row].startsWith("C") && ss[row].contains("=")) {
					std::string s = NCMachineParametersC::FormatCCode(EUtils::QString2StdString(ss[row]));

					cCodes.append(QString::fromStdString(s));
				}
				else {
					QStringList ss2 = ss[row].split(' ', Qt::SkipEmptyParts);
					gSteps.append(ss2[1].mid(8));
					QString zs = ss2[4].mid(5);
					if (zs.startsWith("+")) {
						zs = zs.mid(1);
					}
					gZs.append(zs);
					gTzs.append(nowTz);
					nowTz = "";
				}
			}
			else if (ss[row].startsWith("G85")) {
				nowTz = ss[row];
			}
			else if (ss[row].startsWith("H100")) {
				toz = ss[row].mid(7).trimmed();
			}
			else if (ss[row].startsWith("H101")) {
				step = ss[row].mid(7).trimmed();
				if (step.startsWith("+")) {
					step = step.mid(1);
				}
			}
			else if (ss[row].startsWith("H102")) {
				ln = ss[row].mid(7).trimmed();
			}
		}

		DataTable table2(cCodes.length() + 1, 10 + 1);
		for (int row = 0; row < cCodes.length(); ++row) {
			table2.setValue(row, 0, "Y");
			table2.setValue(row, 9, cCodes[row]);

			auto d = NCMachine::ParseCCodeLine(ss[row]);
			table2.setValue(row, 1, d["No"]);
			table2.setValue(row, 2, ln);
			table2.setValue(row, 3, d["LP"]);
			table2.setValue(row, 4, QString::number(step.toDouble() + gSteps[row].toDouble()));	//STEP
			table2.setValue(row, 5, gZs[row]);
			if (gTzs[row].isEmpty()) {
				table2.setValue(row, 6, "OFF");
				table2.setValue(row, 7, "00000");
			}
			else {
				QString tz = gTzs[row].mid(3);
				if (tz[0] == 'Z' || tz[0] == 'T') {
					table2.setValue(row, 6, QString(tz[0]));
					table2.setValue(row, 7, tz.mid(1).replace(";", "").leftJustified(5, '0'));
				}
			}
		}

		QString filePath2 = SystemSettings::GetDataFilePath(parentName, GetProjectDir());
		filePath2 = SystemSettings::AppendDataFilePath(filePath2, QString("%1_table2").arg(parentName));
		table2.serialize(filePath2);

		DataTable table1(3 + 1, 1 + 1);
		table1.setValue(0, -1, "False");
		table1.setValue(1, -1, "False");
		table1.setValue(2, -1, "False");
		if (ui->inJgz->currentText() == "Z") {
			table1.setValue(2, 0, toz);
			table1.setValue(2, -1, "True");
		}
		else if (ui->inJgz->currentText() == "X") {
			table1.setValue(0, 0, toz);
			table1.setValue(0, -1, "True");
		}
		else if (ui->inJgz->currentText() == "Y") {
			table1.setValue(1, 0, toz);
			table1.setValue(1, -1, "True");
		}
		QString filePath1 = SystemSettings::GetDataFilePath(parentName, GetProjectDir());
		filePath1 = SystemSettings::AppendDataFilePath(filePath1, QString("%1_table1").arg(parentName));
		table1.serialize(filePath1);

		//BaseMainWindow* mainWindow = GetMainWindow();
		//mainWindow->showChildWindow(tr("SDDG"));

		return cCode;
	}
	else {
		QString filePath1 = SystemSettings::GetDataFilePath(parentName, GetProjectDir());
		filePath1 = SystemSettings::AppendDataFilePath(filePath1, QString("%1_table1").arg(parentName));
		QFile::remove(filePath1);

		QString filePath2 = SystemSettings::GetDataFilePath(parentName, GetProjectDir());
		filePath2 = SystemSettings::AppendDataFilePath(filePath2, QString("%1_table2").arg(parentName));
		QFile::remove(filePath2);

		return QString();
		FormUtils::MessageBoxInfo(tr("MYXYSJ"));
	}
}

QString RunAutoParamForm::GenerateManualData_V1(QString parentName)
{
	QString cCode;
#ifdef PYTHON_SUPPORT
	QMap<QString, QString> pyFiles = PluginUtils::loadPythonScripts(this->objectName());
	for (const auto& pyFileName : pyFiles.keys()) {
		if (pyFileName == "rungcode") {
			this->SetData("result", NULL);

			this->SetData("inClzh", ui->inClzh->text());
			this->SetData("inDjxz", ui->inDjxz->text());
			this->SetData("inJgz", ui->inJgz->text());
			this->SetData("inJgsd", ui->inJgsd->text());
			this->SetData("inJgmj", ui->inJgmj->text());
			this->SetData("inCcd", ui->inCcd->text());
			this->SetData("inYdxz", ui->inYdxz->text());
			this->SetData("inYdms", ui->inYdms->text());
			this->SetData("inJgcz", ui->inJgcz->text());
			this->SetData("inDbhhw", ui->inDbhhw->text());
			
			QString value = pyFiles.value(pyFileName);
			PluginUtils::RunFile(value, this);

			QVariant result = this->GetData("result");
			if (result != NULL && result.isValid()) {
				cCode = this->GetData("result").value<QString>();
			}
			break;
		}
	}
#endif
	cCode = cCode.trimmed();
	if (!cCode.isEmpty()) {
		QStringList ss = cCode.split('\n', Qt::SkipEmptyParts);

		QList<QString> cCodes;
		QList<QString> gSteps;
		QList<QString> gZs;
		QString ln, step, toz;
		for (int row = 0; row < ss.size(); ++row) {
			if (ss[row].startsWith("C")) {
				if (!ss[row].startsWith("C000")) {
					std::string s = NCMachineParametersC::FormatCCode(EUtils::QString2StdString(ss[row]));

					cCodes.append(QString::fromStdString(s));
				}
				else {
					QStringList ss2 = ss[row].split(' ', Qt::SkipEmptyParts);
					gSteps.append(ss2[1].mid(8));
					QString zs = ss2[4].mid(5);
					if (zs.startsWith("+")) {
						zs = zs.mid(1);
					}
					gZs.append(zs);
				}
			}
			else if (ss[row].startsWith("H100")) {
				toz = ss[row].mid(7).trimmed();
			}
			else if (ss[row].startsWith("H101")) {
				step = ss[row].mid(7).trimmed();
				if (step.startsWith("+")) {
					step = step.mid(1);
				}
			}
			else if (ss[row].startsWith("H102")) {
				ln = ss[row].mid(7).trimmed();
			}
		}

		DataTable table2(cCodes.length() + 1, 10 + 1);
		for (int row = 0; row < cCodes.length(); ++row) {
			table2.setValue(row, 0, "Y");
			table2.setValue(row, 9, cCodes[row]);

			auto d = NCMachine::ParseCCodeLine(ss[row]);
			table2.setValue(row, 1, d["No"]);
			table2.setValue(row, 2, ln);
			table2.setValue(row, 3, d["LP"]);
			table2.setValue(row, 4, QString::number(step.toDouble() + gSteps[row].toDouble()));
			table2.setValue(row, 5, gZs[row]);
			table2.setValue(row, 6, "0");
			table2.setValue(row, 7, "0000");
		}

		QString filePath2 = SystemSettings::GetDataFilePath(parentName, GetProjectDir());
		filePath2 = SystemSettings::AppendDataFilePath(filePath2, QString("%1_table2").arg(parentName));
		table2.serialize(filePath2);

		DataTable table1(3 + 1, 1 + 1);
		table1.setValue(0, -1, "False");
		table1.setValue(1, -1, "False");
		table1.setValue(2, -1, "False");
		if (ui->inJgz->currentText() == "Z") {
			table1.setValue(2, 0, toz);
			table1.setValue(2, -1, "True");
		}
		else if (ui->inJgz->currentText() == "X") {
			table1.setValue(0, 0, toz);
			table1.setValue(0, -1, "True");
		}
		else if (ui->inJgz->currentText() == "Y") {
			table1.setValue(1, 0, toz);
			table1.setValue(1, -1, "True");
		}
		QString filePath1 = SystemSettings::GetDataFilePath(parentName, GetProjectDir());
		filePath1 = SystemSettings::AppendDataFilePath(filePath1, QString("%1_table1").arg(parentName));
		table1.serialize(filePath1);

		//BaseMainWindow* mainWindow = GetMainWindow();
		//mainWindow->showChildWindow(tr("SDDG"));
	}
	else {
		QString filePath1 = SystemSettings::GetDataFilePath(parentName, GetProjectDir());
		filePath1 = SystemSettings::AppendDataFilePath(filePath1, QString("%1_table1").arg(parentName));
		QFile::remove(filePath1);

		QString filePath2 = SystemSettings::GetDataFilePath(parentName, GetProjectDir());
		filePath2 = SystemSettings::AppendDataFilePath(filePath2, QString("%1_table2").arg(parentName));
		QFile::remove(filePath2);

		
	}

	return cCode;
}


