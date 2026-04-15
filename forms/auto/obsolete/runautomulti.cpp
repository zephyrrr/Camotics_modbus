#include "runautomulti.h"
#include <QHeaderView>
#include <QComboBox>
#include <QCompleter>
#include <QToolButton>
#include <QDir>
#include <QHash>
#include <QPluginLoader>
#include "widgets/qlineedit4axisvalue.h"
#include "BaseMainWindow.h"
#include "modbus/NCMachineParametersC.h"
//#include "plugins/ipointgenerator.h"
//#include "plugins/pointgenratorbygrid.h"
#include "utils/dataform.h"
#include "modbus/NCMachine.h"
#include "widgets/multibuttonswidget.h"
#include "widgets/filedialog.h"
#include "utils/nfile.h"
#include "utils/formutils.h"

RunAutoMulti::RunAutoMulti(QWidget* parent)
	: BaseChildWindow(parent)
{
	this->setObjectName("runautomulti");

	table1 = new DataTable();
	table2 = new DataTable();

	ui.setupUi(this);

	QString headers[] = { "No.", tr("JG"), "X", "Y", tr("ZBX") };
	int columnWidth[] = { 50, 75, 150, 150, 100 };
	QString defaultRowValues[] = { "", tr("S"), "0.000", "0.000", "054" };

	table3 = new ButtonEditTableWidget(this);

	table3->createForm(1, 4, [this, defaultRowValues](int col, QWidget* parent) {
		NLineEdit* edit;
		if (col == 1) {
			QLineEditLikeButton* btn = new QLineEditLikeButton(parent);
			connect(btn, &QLineEdit::textChanged, [btn, parent](const QString& s) {
				ButtonEditTableWidget* table = qobject_cast<ButtonEditTableWidget*>(parent);
				int rowHere = table->indexAt(btn->pos()).row();
				bool isOk = s == tr("S");
				table->setReadOnlyOnRow(rowHere, !isOk);
				});
			edit = btn;
		}
		else if (col == 4) {
			edit = new QLineEdit4Coor(parent);
			//QRegExp regExp("^[0-9][5][4-9]$");
			//QRegExpValidator* validator = new QRegExpValidator(regExp, edit);
			//edit->setValidator(validator);
		}
		else {
			edit = new QLineEdit4AxisValue(parent);
		}
		edit->setText(defaultRowValues[col]);
		return edit;
		});
	table3->setObjectName(QString("%1_table3").arg(this->objectName()));
	for (int i = 0; i < table3->columnCount(); ++i)
	{
		qobject_cast<QLabel*>(table3->cellWidget(0, i))->setText(headers[i]);
		table3->setColumnWidth(i, columnWidth[i]);
	}
	for (int i = 0; i < 1; ++i)
	{
		emit table3->rowInserted(i);
	}

	//connect(table3, &ButtonEditTableWidget::rowButtonClicked, [this](int row) {
	//	for (int i = 1; i < 3; ++i) {
	//		//table3->setValue(row, i, "00000.000");
	//		//QLineEdit* edit = qobject_cast<QLineEdit*>(this->table3->cellWidget(row + 1, i));
	//		//if (edit->text().isEmpty()) {
	//		//	edit->setText("00000.000"); 
	//		//}
	//	}
	//});

	ui.verticalLayout_table1->addWidget(table3);

	MultiButtonsWidget* mbw = new MultiButtonsWidget(this);
	mbw->addDefaultButtons(table3);
	mbw->removeButton(5);
	mbw->removeButton(4);
	//mbw->addButton(tr("DR"), [this](bool) {
	//	QString path = this->GetProjectDir() + QDir::separator() + QString("WeiZhiJiYiForm");
	//	QString fileName = NFileDialog::getOpenFileName(this, tr("DKWJ"), path, QStringLiteral("(*.json.pos);;"));
	//	if (fileName.isEmpty())
	//		return;
	//	table3->deserialize(fileName);
	//	});
	ui.verticalLayout_table1->insertWidget(0, mbw);

	ui.gridLayout->removeWidget(ui.label_5);
	ui.gridLayout->removeWidget(ui.btnJghs);
	ui.gridLayout->removeWidget(ui.label_1);
	ui.gridLayout->removeItem(ui.horizontalLayout);
	ui.label_1->hide();
	ui.pushButton->hide();
	ui.pushButton_2->hide();
	ui.label_5->hide();
	ui.btnJghs->hide();
}

RunAutoMulti::~RunAutoMulti()
{
	delete table1;
	delete table2;
}

QString RunAutoMulti::GetGCode()
{
	QStringList toAxis, toAxisLength;
	for (int i = 0; i < 3; ++i) {
		if (table1->getValue(i, -1) != "True") {
			continue;
		}

		QString s = table1->getValue(i, 0);
		if (!s.isEmpty()) {
			toAxis.append(QString("XYZ").at(i));
			toAxisLength.append(s);
			break;
		}
	}
	if (toAxis.isEmpty() || toAxis.count() > 3) {
		return QString();
	}


	// 
	QString otherAxis[] = { "X", "Y" };

	if (ui.lineEditZ1->text().isEmpty() || ui.lineEditZ2->text().isEmpty())
		return QString();

	QString gcode;
	for (int i = 0; i < toAxis.size(); ++i) {
		gcode += QString("H%1   = %2\n").arg(100 + i).arg(toAxisLength[i]);
	}

	DataForm* dataForm = DataForms::getInstance()->getDataForm(m_name4RunAutoOne);

	if (dataForm->getValue("inAbsolute") == "true")
		gcode += "G90;\n";
	else
		gcode += "G91;\n";


	mapLine2Row.clear();
	for (int i = 0; i < table3->getDataRowCount(); ++i) {
		if (table3->getValue(i, 0) != tr("S")) {
			continue;
		}
		if (table3->getValue(i, 1).isEmpty() || table3->getValue(i, 2).isEmpty())
			continue;
		gcode += QString("G%1;\n").arg(table3->getValue(i, 3));
		gcode += QString("G00 %1%2;\n").arg("Z").arg(ui.lineEditZ2->text());
		gcode += QString(R"(G00 %1%2 %3%4;
M98P0000;
;
)").arg(otherAxis[0]).arg(table3->getValue(i, 1)).arg(otherAxis[1]).arg(table3->getValue(i, 2));
		int nowLines = gcode.split("\n").size();
		// last line
		mapLine2Row[nowLines - 3] = i + 1;
	}

	gcode += QString(R"(T85;
M02;
;
N0000;
G00 %1%2;
)").arg("Z").arg(ui.lineEditZ1->text());

	gcode += "T84;\n";
	QString cCode;
	cCode += QString::fromStdString(NCMachineParametersC::GetNamesAsString());

	QHash<int, int> cNos;
	for (int i = 0; i < table2->getDataRowCount(); ++i) {
		if (table2->getValue(i, 0) != tr("S")) {
			continue;
		}
		if (table2->getValue(i, 1).isEmpty()) {
			continue;
		}

		if (table2->getValue(i, 6) == "1") {
			gcode += QString("G85T%1;\n").arg(table2->getValue(i, 7));
		}
		else if (table2->getValue(i, 6) == "2") {
			gcode += QString("G85Z%1;\n").arg(table2->getValue(i, 7));
		}
		QString s;
		if (table2->getDataColumnCount() >= 10) {
			int row = i;
			int nowcNo = table2->getValue(row, 1).toInt();
			QString c = table2->getValue(i, 9);
			if (c.isEmpty()) {
			}
			if (!c.endsWith('\n')) {
				c += "\n";
			}
			if (cNos.contains(nowcNo)) {
				int nextcNo = cNos[nowcNo] + 10;
				while (true) {
					if (!cNos.contains(nextcNo)) {
						cNos[nextcNo] = 1;
						c = c.replace("C" + QString::number(nowcNo).rightJustified(3, '0') + " = ", "C" + QString::number(nextcNo).rightJustified(3, '0') + " = ");
						nowcNo = nextcNo;
						break;
					}
					else {
						nextcNo += 10;
					}
				}
			}
			else {
				cNos[nowcNo] = 1;
			}
			cCode += c;

			QString gcode3;
			for (int j = 0; j < toAxis.size(); ++j) {
				gcode3 += QString("%1H%2+%3 ").arg(toAxis[j]).arg(100 + j).arg(table2->getValue(i, 5));
			}

			s = QString("C%1 STEP%4 LN%2 LP%3 G01 %5 M04;\n")
				.arg(QString::number(nowcNo).rightJustified(3, '0'))
				.arg(table2->getValue(i, 2))
				.arg(table2->getValue(i, 3))
				.arg(table2->getValue(i, 4))
				.arg(gcode3);
		}
		else {
			QString gcode3;
			for (int j = 0; j < toAxis.size(); ++j) {
				gcode3 += QString("%1H%2+%3 ").arg(toAxis[j]).arg(100 + j).arg(table2->getValue(i, 5));
			}

			s = QString("C%1 STEP%4 LN%2 LP%3 G01 %5 M04;\n")
				.arg(QString::number(table2->getValue(i, 1).toInt()).rightJustified(3, '0'))
				.arg(table2->getValue(i, 2))
				.arg(table2->getValue(i, 3))
				.arg(table2->getValue(i, 4))
				.arg(gcode3);
		}
		gcode += s;

		// int nowLines = gcode.split("\n").size();
		// not here
		// mapLine2Row[nowLines - 1] = i + 1;
	}

	gcode += QString("M99; ");

	if (!cCode.isEmpty()) {
		gcode = cCode + "\n" + gcode;
		QHash<int, int> mapLine2Row2;
		int cCodeLines = cCode.split("\n").size();
		for (int key : mapLine2Row.keys()) {
			mapLine2Row2[key + cCodeLines] = mapLine2Row[key];
		}
		mapLine2Row = mapLine2Row2;
	}

	return gcode;
}

void RunAutoMulti::RunGCode()
{
	QString gcode = GetGCode();

	this->table3->setRowSelection(-1, false);
	this->setEnabled(false);

	m_ncMachine->GetRealtimeJsonMachine()->setFunctionDoPerLineBefore([this](cb::JSON::ValuePtr sink, int line) {
		QMetaObject::invokeMethod(this, [this, line]() {
			if (mapLine2Row.contains(line)) {
				int row = mapLine2Row[line];
				table3->setRowSelection(row, true);
			}
			}, Qt::QueuedConnection);
		});

	m_ncMachine->RunGCode(gcode, [this]() {
		QMetaObject::invokeMethod(this, [this]() {
			this->setEnabled(true);
			}, Qt::QueuedConnection);
		});
}

void RunAutoMulti::LoadData(QString filePath)
{
	if (filePath == NULL) {
		filePath = GetDataFilePath(this);
	}
	table3->deserialize(SystemSettings::AppendDataFilePath(filePath, table3->objectName()));

	QString filePath2 = SystemSettings::GetDataFilePath(m_name4RunAutoOne);
	filePath2 = SystemSettings::AppendDataFilePath(filePath2, QString("%1_table2").arg(m_name4RunAutoOne));
	table2->deserialize(filePath2);
	int n = 0;
	for (int i = 0; i < table2->getDataRowCount(); ++i) {
		if (table2->getValue(i, 0) != tr("S")) {
			continue;
		}
		if (table2->getValue(i, 1).isEmpty()) {
			continue;
		}
		n++;
	}
	ui.btnJghs->setText(QString::number(n));

	QString filePath1 = SystemSettings::GetDataFilePath(m_name4RunAutoOne);
	filePath1 = SystemSettings::AppendDataFilePath(filePath1, QString("%1_table1").arg(m_name4RunAutoOne));
	table1->deserialize(filePath1);

	BaseChildWindow::LoadData(filePath);

	this->table3->setEnabled(!m_ncMachine->IsGCodeRunning());
}

void RunAutoMulti::SaveData(QString filePath)
{
	if (filePath == NULL) {
		filePath = GetDataFilePath(this);
	}
	table3->serialize(SystemSettings::AppendDataFilePath(filePath, table3->objectName()));

	BaseChildWindow::SaveData(filePath);
}


//void RunAutoMulti::on_btnShortcutGrid1_clicked()
//{
//	PointGenratorByGrid dlg(this);
//	QVector<double> rs = dlg.generatePoints();
//	if (rs.count() > 0) {
//		for (int i = 0; i < rs.count(); i ++)
//		{
//			if (i / 2 >= table3->getDataRowCount()) {
//				table3->insertNewRow(i / 2);
//			}
//			table3->setValue(i / 2, i % 2, QString::number(rs[i], 'f', 3));
//		}
//	}
//}

void RunAutoMulti::on_btnJghs_clicked()
{
	//BaseMainWindow* mainWindow = GetMainWindow();
	//mainWindow->showChildWindow(tr("SDDG"));
}

void RunAutoMulti::showEvent(QShowEvent* event)
{
	BaseChildWindow::showEvent(event);

	//if (m_tool4ZeroAndHalfAxis == NULL) {
	//	m_tool4ZeroAndHalfAxis = new Tool4ZeroAndHalfAxis(ncMachine, this, TOOL_LOAD_SAVE | TOOL_GCODE);
	//	Tool4ZeroAndHalfAxis::ShowIt(m_tool4ZeroAndHalfAxis);
	//}
}