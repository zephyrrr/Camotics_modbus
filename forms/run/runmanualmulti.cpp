#include "runmanualmulti.h"
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
#include "runmanual.h"

RunManualMulti::RunManualMulti(QWidget *parent, QString objectName)
	: BaseChildWindowWithTools(parent, TOOL_LOAD_SAVE | TOOL_GCODE)
{
	this->setObjectName(objectName);

	table1 = new DataTable();
	table2 = new DataTable();
	
	ui.setupUi(this);

	QString headers[] = { "No.", tr("JG"), "X", "Y", "U", tr("ZBX"), tr("QS"), tr("JS")};
	int columnWidth[] = { 50, 75, 150, 150, 150, 100, 50, 50 };
	QString defaultRowValues[] = { "Y", "0.000", "0.000", "0.000", "054", "", ""};
	
	table3 = new ButtonEditTableWidget(this);
	table3->createForm(1, 7, [this](int col, QWidget* parent) {
		NLineEdit* edit;
		if (col == 1) {
			QLineEditLikeButton* btn = new QLineEditLikeButton(parent);
			connect(btn, &QLineEdit::textChanged, [btn, parent](const QString& s) {
				ButtonEditTableWidget* table = qobject_cast<ButtonEditTableWidget*>(parent);
				int rowHere = table->getRowOfEdit(1, btn);
				bool isOk = btn->IsYes();
				table->setPropertyOnRow(rowHere, "shouldRun", isOk);
				/*table->setReadOnlyOnRow(rowHere, !isOk);
				table->setReadOnlyOnCell(rowHere, 1, false);
				table->setReadOnlyOnCell(rowHere, 0, false);*/
				});
			edit = btn;
		}
		else if (col == 5) {
			edit = new QLineEdit4Coor(parent);
			//QRegExp regExp("^[0-9][5][4-9]$");
			//QRegExpValidator* validator = new QRegExpValidator(regExp, edit);
			//edit->setValidator(validator);
		}
		else if (col == 6 || col == 7) {
			QLineEdit4Keyboard* r = new QLineEdit4Keyboard(parent);
			r->setValidator(new QIntValidator(1, 100000, r));
			r->setAllowNull(true);
			edit = r;
		}
		else {
			edit = new QLineEdit4AxisValue(parent);
		}
		return edit;
		});
	table3->setObjectName(QString("%1_table3").arg(this->objectName()));
	for (int i = 0; i < table3->columnCount(); ++i)
	{
		qobject_cast<QLabel*>(table3->cellWidget(0, i))->setText(headers[i]);
		table3->setColumnWidth(i, columnWidth[i]);
	}

	if (SPK_AXIS_LEN == 3) {
		table3->setColumnHidden(4, true);
	}
	connect(table3, &ButtonEditTableWidget::rowInserted, [this, defaultRowValues](int rowIndex) {
		for (int i = 0; i < table3->getDataYCount(); ++i) {
			table3->setValue(rowIndex, i, defaultRowValues[i]);
		}
		});
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

	mbw = new MultiButtonsWidget(this);
	mbw->addDefaultButtons(table3);
	mbw->addButton(tr("DR"), [this](bool) {
		if (table3->isReadOnly())
			return;
		QString path = SystemSettings::GetPath("WeiZhiJiYiForm", SystemSettings::ProjectFlag);

		QString fileName = NFileDialog::getOpenFileName(this, tr("DKWJ"), path, QString("(*.json.pos);;"));

		if (fileName.isEmpty())
			return;

		table3->deserialize(fileName);
		});
	mbw->addButton(tr("SelectAll"), [this](bool) {
		for (int i = 0; i < table3->getDataCount(); ++i) {
			table3->setValue(i, -1, "True");
		}
		});
	mbw->addButton(tr("Reset"), [this](bool) {
		if (table3->isReadOnly())
			return;
		this->table3->setRowSelection(-1, false);
		for (int i = 0; i < table3->getDataCount(); ++i) {
			table3->setValue(i, 0, "True");
			table3->setValue(i, -1, "False");
		}
		this->setFocus();
		});
	ui.verticalLayout_table1->insertWidget(0, mbw);

	//const auto staticInstances = QPluginLoader::staticInstances();
	//for (QObject* plugin : staticInstances)
	//{
	//	auto iBrush = qobject_cast<IPointGenerator*>(plugin);
	//}

	/*QList<IDoPlugin*> plugins = PluginUtils::loadPlugin<IDoPlugin>(this->objectName());
	for (IDoPlugin* it : plugins) {
		QPushButton* btn = new QPushButton(it->getName());
		ui.horizontalLayoutShortcut->addWidget(btn);
		
		connect(btn, &QPushButton::clicked, [it, this]() {
			QWidget* itWidget = it->getWidget();
			if (itWidget) {
				itWidget->setParent(this);
				itWidget->setWindowFlags(Qt::Dialog);
				itWidget->show();
				itWidget->setStyle(itWidget->style());
			}
			QVector<double> rs = it->doIt().value<QVector<double>>();
			if (rs.count() > 0) {
				for (int i = 0; i < rs.count(); i++)
				{
					if (i / 2 >= table3->getRowCount()) {
						table3->insertNewRow(i / 2);
					}
					table3->setValue(i / 2, i % 2, QString::number(rs[i], 'f', 3));
				}
			}
			});
	}*/

//#ifdef PYTHON_SUPPORT
//	//try {
//	//	PythonQtRuntime::registerCPPClass("RunManualMulti", "QWidget", "", PythonQtCreateObject<RunManualMultiWrapper>);
//	//	PythonQtRuntime::registerCPPClass("ButtonEditTableWidget", "QWidget", "", PythonQtCreateObject<ButtonEditTableWidgetWrapper>);
//	//	QList<QWidget*> scriptWidgets = PluginUtils::CreateScripts(this->objectName(), this);
//	//	for (QWidget* w : scriptWidgets) {
//	//		ui.horizontalLayoutShortcut->addWidget(w);
//	//	}
//	//}
//	//catch (...) {
//	//}
//#endif
}

RunManualMulti::~RunManualMulti()
{
	delete table1;
	delete table2;

	if (mbw) {
		delete mbw;
	}
}

QString RunManualMulti::GetGCode(bool forRun)
{
	SaveData();

	if (table2->getDataCount() <= 0) {
		FormUtils::MessageBoxInfo(tr("JGHSWL"));
		return QString();
	}
	int jghsCount = 0;
	for (int i = 0; i < table2->getDataCount(); ++i) {
		if (!QLineEditLikeButton::IsYes(table2->getValue(i, 0))) {
			continue;
		}
		if (table2->getValue(i, 1).isEmpty()) {
			continue;
		}
		jghsCount++;
		break;
	}
	if (jghsCount == 0) {
		FormUtils::MessageBoxInfo(tr("JGHSWL"));
		return QString();
	}

	int realAxisLen = 0;
	double toAxisLength[] = { INFINITE, INFINITE, INFINITE, INFINITE };
	for (int i = 0; i < table1->getDataCount(); ++i) {
		if (table1->getValue(i, -1) != "True") {
			continue;
		}

		QString s = table1->getValue(i, 0);
		if (!s.isEmpty()) {
			realAxisLen++;
			toAxisLength[i] = s.toDouble();
		}
	}
	if (realAxisLen == 0) {
		FormUtils::MessageBoxInfo(tr("JGZBZWXZ"));
		return QString();
	}

	DataForm* dataForm = DataForms::getInstance()->getDataForm(m_name4RunAutoOne);
	bool isAbsolutePosition = dataForm->getValue("inAbsolute") != "false";

	if (isAbsolutePosition && realAxisLen > 1) {
		FormUtils::MessageBoxInfo(tr("DZJGSYGYXDZB"));
		return QString();
	}


	// 导出G代码，且单轴加工时候
	if (!forRun) {
		QStringList toAxis;
		for (int i = 0; i < 3; ++i) {
			if (table1->getValue(i, -1) != "True") {
				continue;
			}

			QString s = table1->getValue(i, 0);
			if (!s.isEmpty()) {
				toAxis.append(QString("XYZ").at(i));
			}
		}
		bool emptyStartEndRunIndex = true;
		for (int i = 0; i < table3->getDataCount(); ++i) {
			QString startRunManualIndex = table3->getValue(i, 5);
			QString endManualIndex = table3->getValue(i, 6);
			if (startRunManualIndex.isEmpty() && endManualIndex.isEmpty()) {
			}
			else {
				emptyStartEndRunIndex = false;
			}
		}

		if (toAxis.length() == 1 && ui.btnJgff->isChecked() && emptyStartEndRunIndex) {
			return GetGCodeV1();
		}
		//else if (toAxis.length() > 1 && ui.btnJgff->isChecked() && emptyStartEndRunIndex) {
		//	return GetGCodeV1();
		//}
	}

	return GetGCodeV2();
}
QString RunManualMulti::GetGCodeV2()
{
	QString otherAxis[] = { "X", "Y", "U"};

	int axisLen = SPK_AXIS_LEN;
	if (table3->isColumnHidden(4))
	{
		axisLen = 3;
	}

	int realAxisLen = 0;
	double toAxisLength[] = { INFINITE, INFINITE, INFINITE, INFINITE };
	for (int i = 0; i < table1->getDataCount(); ++i) {
		if (table1->getValue(i, -1) != "True") {
			continue;
		}

		QString s = table1->getValue(i, 0);
		if (!s.isEmpty()) {
			realAxisLen++;
			double d = s.toDouble();
			toAxisLength[i] = d;
		}
	}
	if (realAxisLen == 0) {
		FormUtils::MessageBoxInfo(tr("JGZBZWXZ"));
		return QString();
	}
	if (toAxisLength[0] == INFINITE && toAxisLength[1] == INFINITE && toAxisLength[2] == INFINITE) {
		return QString();
	}

	QString gcode = RunManual::GetGCodeOSub803(table2);
	if (gcode.isEmpty()) {
		return QString();
	}
	gcode += "\n\n";
	gcode += "G90 G11 G24 T84\n";


	double axisPositions[] = { 0.0, 0.0, 0.0 };
	for (int i = 0; i < 3; ++i) {
		axisPositions[i] = m_ncMachine->GetController()->getAxisPosition(GCodeTool::axis[i]);
	}
	DataForm* dataForm = DataForms::getInstance()->getDataForm(m_name4RunAutoOne);
	//QString gcode;// = RunManual::GetGCodeStatic(table1, table2, dataForm->getValue("inAbsolute") == "true", axisPositions);
	bool isAbsolutePosition = dataForm->getValue("inAbsolute") != "false";

	if (isAbsolutePosition && realAxisLen > 1){
		FormUtils::MessageBoxInfo(tr("DZJGSYGYXDZB"));
		return QString();
	}

	int maxJgff = 0;
	for (int i = 0; i < table2->getDataCount(); ++i) {
		if (!QLineEditLikeButton::IsYes(table2->getValue(i, 0))) {
			continue;
		}
		if (table2->getValue(i, 1).isEmpty()) {
			continue;
		}
		maxJgff++;
	}

	QList<int> todoJgff;
	if (!ui.btnJgff->isChecked()) {
		for (int i = 0; i < table2->getDataCount(); ++i) {
			if (!QLineEditLikeButton::IsYes(table2->getValue(i, 0))) {
				continue;
			}
			if (table2->getValue(i, 1).isEmpty()) {
				continue;
			}
			todoJgff.append(todoJgff.count() + 1);
		}
	}
	else {
		todoJgff.append(0);
	}

	QList<int> todoRows;
	for(int indexJgff = 0; indexJgff < todoJgff.count(); ++indexJgff) {
		int iJgff = todoJgff[indexJgff];
	//for each(int iJgff in todoJgff) {
		for (int i = 0; i < table3->getDataCount(); ++i) {
			if (!QLineEditLikeButton::IsYes(table3->getValue(i, 0))) {
				continue;
			}
			if (table3->getValue(i, 1).isEmpty() || table3->getValue(i, 2).isEmpty())
				continue;
			gcode += QString("(debug, do_call_ui set_runmanualmulti_table3_current_row_%1)").arg(i + 1) + "\n";

			gcode += QString("G%1 ").arg(table3->getValue(i, 4));
			
			gcode += QString("G00 ");
			for (int j = 0; j < axisLen - 1; ++j)
			{
				gcode += QString("%1%2 ").arg(otherAxis[j]).arg(table3->getValue(i, j + 1));
			}
			gcode += "\n";

			if (!ui.lineEditZKaisi->text().isEmpty())
				gcode += QString("G00 %1%2\n").arg("Z").arg(ui.lineEditZKaisi->text());

			double startAxisPositions[] = { table3->getValue(i, 1).toDouble(), table3->getValue(i, 2).toDouble(), ui.lineEditZKaisi->text().toDouble(), table3->getValue(i, 3).toDouble(), };
			double toAxisPositions[] = { toAxisLength[0], toAxisLength[1], toAxisLength[2], toAxisLength[3] };

			if (!isAbsolutePosition) {
				// G91
				for (int i = 0; i < axisLen; ++i) {
					if (toAxisPositions[i] != INFINITE) {
						toAxisPositions[i] += startAxisPositions[i];
					}
				}
			}
			for (int i = 0; i < axisLen; ++i) {
				double d = startAxisPositions[i];
				gcode += QString("H%1   = %2\n").arg(661 + i).arg(d, 0, 'f', 3);
				gcode += QString("H%1   = %2\n").arg(771 + i).arg(toAxisPositions[i] == INFINITE ? d : toAxisPositions[i], 0, 'f', 3);
				gcode += QString("H%1   = %2\n").arg(881 + i).arg(toAxisLength[i] == INFINITE ? 0 : 1);
			}

			QString startRunManualIndex = table3->getValue(i, 5);
			QString endManualIndex = table3->getValue(i, 6);
			if (startRunManualIndex.isEmpty() && endManualIndex.isEmpty()) {
				gcode += QString("o800 call [%1]\n").arg(iJgff);
			}
			else {
				int startIndex = 0;
				if (!startRunManualIndex.isEmpty())
					startIndex = startRunManualIndex.toInt();
				if (startIndex < 0)
					startIndex = 0;
				if (startIndex > maxJgff)
					startIndex = maxJgff;
				int endIndex = maxJgff;
				if (!endManualIndex.isEmpty())
					endIndex = endManualIndex.toInt();
				if (endIndex < 0)
					endIndex = 0;
				if (endIndex > maxJgff)
					endIndex = maxJgff;

				if (iJgff == 0) {
					for (int iJgff2 = startIndex; iJgff2 <= endIndex; ++iJgff2)
					{
						gcode += QString("o800 call [%1]\n").arg(iJgff2);
					}
				}
				else {
					if (iJgff >= startIndex && iJgff <= endIndex) {
						gcode += QString("o800 call [%1]\n").arg(iJgff);
					}
					else {
						gcode += "\n";
					}
				}
			}
			if (!ui.lineEditZAnQuan->text().isEmpty())
				gcode += QString("G00 %1%2\n").arg("Z").arg(ui.lineEditZAnQuan->text());

			gcode += QString("(debug, do_call_ui unset_runmanualmulti_table3_current_row_%1)").arg(i + 1) + "\n";

			//int nowLines = gcode.split("\n").size();
			//// last line
			//mapLine2Row[nowLines - 3] = i + 1;	// G00 %1%2 %3%4
			todoRows.append(i + 1);

			// 如果是单独加工模式，每个点加工完后，重置table2的行
			if (ui.btnJgff->isChecked()) {
				gcode += "(debug, do_call_ui reset_runmanual_table2_current_row)\n";
			}
		}
		gcode += "(debug, do_call_ui reset_runmanualmulti_table3_current_row)\n";

		// 如果是连续加工模式，重置table3的值
		if (!ui.btnJgff->isChecked() && indexJgff != todoJgff.count() - 1) {
			gcode += "(debug, do_call_ui reset_runmanualmulti_table3_values)\n";
		}
	}

	int beepTime = DataForms::getInstance()->getDataForm("xitongshezhi5")->getValue("FDWCHMJSJ").toInt() * 1000;
	gcode += QString(R"(
T85
do_beep %1
do_light 2
M02
)").arg(beepTime);

	//mapLine2Row.clear();
	//QStringList lines = gcode.split('\n'); // Splits by newline character
	//int lineNumber = 0;
	//int findedRow = 0;
	//QString toSearchLine = QString("G00 %1%2").arg("Z").arg(ui.lineEditZKaisi->text());
	//for (const QString& line : lines) {
	//	lineNumber++;
	//	if (line == toSearchLine) {
	//		this->mapLine2Row[lineNumber] = (todoRows[findedRow] - 1) % table3->getDataCount() + 1;
	//		findedRow++;
	//	}
	//	if (line.startsWith("M02")) {
	//		this->mapLine2Row[lineNumber] = this->table3->getDataCount() + 1;
	//		findedRow++;
	//	}
	//}

	return gcode;
}

QString RunManualMulti::GetGCodeV1()
{
	if (!ui.btnJgff->isChecked()) {
		FormUtils::MessageBoxInfo(tr("BZCLXJGMS"));
		return QString();
	}

	QString otherAxis[] = { "X", "Y", "U" };

	int axisLen = SPK_AXIS_LEN;
	if (table3->isColumnHidden(4))
	{
		axisLen = 3;
	}

	DataForm* dataForm = DataForms::getInstance()->getDataForm(m_name4RunAutoOne);
	QString gcode0 = RunManual::GetGCodeV1(dataForm, table1, table2, NULL);

	QString gcode;

	// 多轴
	if (gcode0.contains("N0001")) {

		double toAxisLength[] = { INFINITE, INFINITE, INFINITE, INFINITE };
		for (int i = 0; i < table1->getDataCount(); ++i) {
			if (table1->getValue(i, -1) != "True") {
				continue;
			}

			QString s = table1->getValue(i, 0);
			if (!s.isEmpty()) {
				toAxisLength[i] = s.toDouble();
			}
		}

		for (int i = 0; i < table3->getDataCount(); ++i) {
			if (!QLineEditLikeButton::IsYes(table3->getValue(i, 0))) {
				continue;
			}
			if (table3->getValue(i, 1).isEmpty() || table3->getValue(i, 2).isEmpty())
				continue;

			// 相对坐标
			for (int j = 0; j < axisLen - 1; ++j) {
				double d = table3->getValue(i, j + 1).toDouble();
				// Z, not U
				if (j == 2) {
					//d = m_ncMachine->GetController()->getAxisPosition(GCodeTool::axis[j]);
					d = ui.lineEditZKaisi->text().toDouble();
				}
				gcode += QString("H%1   = %2\n").arg(661 + j).arg(d, 0, 'f', 3);
				gcode += QString("H%1   = %2\n").arg(771 + j).arg(toAxisLength[j] == INFINITE ? d : d + toAxisLength[j], 0, 'f', 3);
				//gcode += QString("H%1   = %2\n").arg(881 + i).arg(toAxisLength[i] == INFINITE ? 0 : 1);
			}

			gcode += QString("G%1 ").arg(table3->getValue(i, 4));

			gcode += QString("G00 ");
			for (int j = 0; j < axisLen - 1; ++j)
			{
				gcode += QString("%1%2 ").arg(otherAxis[j]).arg(table3->getValue(i, j + 1));
			}

			gcode += "M98P0000\n";
		}

		gcode0 = gcode0.replace("N0000", "N0000\n" + QString("G00 %1 %2\n").arg("Z").arg(ui.lineEditZKaisi->text()));
		gcode0 = gcode0.replace("M99\nN0001", QString("G00 %1%2\n").arg("Z").arg(ui.lineEditZAnQuan->text()) + "\nM99\nN0001");
	}
	else {
		for (int i = 0; i < table3->getDataCount(); ++i) {
			if (!QLineEditLikeButton::IsYes(table3->getValue(i, 0))) {
				continue;
			}
			if (table3->getValue(i, 1).isEmpty() || table3->getValue(i, 2).isEmpty())
				continue;
			gcode += QString("G%1 ").arg(table3->getValue(i, 4));

			gcode += QString("G00 ");
			for (int j = 0; j < axisLen - 1; ++j)
			{
				gcode += QString("%1%2 ").arg(otherAxis[j]).arg(table3->getValue(i, j + 1));
			}

			gcode += "M98P0000\n";
		}
		gcode0 = gcode0.replace("N0000", "N0000\n" + QString("G00 %1 %2\n").arg("Z").arg(ui.lineEditZKaisi->text()));
		gcode0 = gcode0.replace("M99", QString("G00 %1%2\n").arg("Z").arg(ui.lineEditZAnQuan->text()) + "\nM99");
	}

	return gcode0.replace("M98P0000", gcode);
}

void RunManualMulti::RunGCode()
{
	SystemSettings::instance().LastRunNCFileName = tr("SDMG");

	QString gcode = GetGCode();

	this->table3->setRowSelection(-1, false);
	//this->setEnabled(false);
	
	RunManual* runManualForm = qobject_cast<RunManual *>(BaseChildWindow::GetMainWindow()->getChildWindow(tr("SDDG")));
	runManualForm->SetCurrentRunLine(true);

	//bool unsetPreviousLine = !ui.btnJgff->isChecked();
	lastLine4DoneOne = -1;
	lastLine4DoneMulti = -1;

	m_ncMachine->GetRealtimeJsonMachine()->setFunctionDoWhenCallUI([this, runManualForm](std::string s) {
		QMetaObject::invokeMethod(this, [this, s, runManualForm]() {
			QString s2 = QString::fromStdString(s).trimmed();
			if (s2.startsWith("set_runmanual_table2_current_row_")) {
				lastLine4DoneOne = s2.split('_').last().toInt();
				runManualForm->table2->setRowSelection(lastLine4DoneOne, true);
				runManualForm->table2->ScrollToRow(lastLine4DoneOne);
			}
			else if (s2.startsWith("unset_runmanual_table2_current_row_")) {
				int row = s2.split('_').last().toInt();
				runManualForm->table2->setRowSelection(row, false);
				runManualForm->table2->setValue(row - 1, -1, "false");
				//runManualForm->table2->setValue(row - 1, 0, "false");
			}
			else if (s2 == "reset_runmanual_table2_current_row") {
				lastLine4DoneOne = -1;
				runManualForm->table2->setRowSelection(-1, false);
				//for (int i = 0; i < runManualForm->table2->getDataCount(); ++i) {
				//	runManualForm->table2->setValue(i, 0, "True");
				//	runManualForm->table2->setValue(i, -1, "False");
				//}
				runManualForm->table2->ScrollToRow(0);

				//if (lastLine4DoneMulti != -1) {
				//	int row = lastLine4DoneMulti;
				//	this->table3->setValue(row - 1, -1, "false");
				//	this->table3->setValue(row - 1, 0, "false");
				//}
			}
			else if (s2.startsWith("set_runmanualmulti_table3_current_row_")) {
				lastLine4DoneMulti = s2.split('_').last().toInt();
				this->table3->setRowSelection(lastLine4DoneMulti, true);
				this->table3->ScrollToRow(lastLine4DoneMulti);
			}
			else if (s2.startsWith("unset_runmanualmulti_table3_current_row_")) {
				int row = s2.split('_').last().toInt();
				this->table3->setRowSelection(row, false);
				this->table3->setValue(row - 1, -1, "false");
				this->table3->setValue(row - 1, 0, "false");
			}
			else if (s2 == "reset_runmanualmulti_table3_current_row") {
				lastLine4DoneMulti = -1;
				this->table3->setRowSelection(-1, false);
				//for (int i = 0; i < this->table3->getDataCount(); ++i) {
				//	this->table3->setValue(i, 0, "True");
				//	this->table3->setValue(i, -1, "False");
				//}
				this->table3->ScrollToRow(0);
			}
			else if (s2 == "reset_runmanualmulti_table3_values") {
				for (int i = 0; i < this->table3->getDataCount(); ++i) {
					this->table3->setValue(i, 0, "True");
					this->table3->setValue(i, -1, "False");
				}
				this->table3->ScrollToRow(0);
			}
			}, Qt::QueuedConnection);
		});

	//m_ncMachine->GetRealtimeJsonMachine()->setFunctionDoPerLineBefore([this, runManualForm, unsetPreviousLine](cb::JSON::ValuePtr sink, int line) {
	//	QMetaObject::invokeMethod(this, [this, line, sink, runManualForm, unsetPreviousLine]() {
	//		if (mapLine2Row.contains(line)) {
	//			int row = mapLine2Row[line];
	//			// M02
	//			if (row > this->table3->getDataCount()) {
	//				row = this->table3->getDataCount() + 1;
	//				if (unsetPreviousLine) {
	//					table3->setRowSelection(row - 1, false);
	//				}
	//				table3->setValue(row - 2, 0, "N");
	//				return;
	//			}
	//			if (!QLineEditLikeButton::IsYes(table3->getValue(row - 1, 0))) {
	//				// 复位 （加工方式是连续的时候）
	//				if (unsetPreviousLine) {
	//					this->table3->setRowSelection(-1, false);
	//				}
	//				for (int i = 0; i < table3->getDataCount(); ++i) {
	//					if (mapLine2Row.values().contains(i + 1)) {
	//						table3->setValue(i, -1, "False");
	//						table3->setValue(i, 0, "Y");
	//					}
	//				}
	//			}
	//			table3->setRowSelection(row, true);
	//			int scrollRow = row + 0;
	//			if (scrollRow >= table3->rowCount()) {
	//				scrollRow = table3->rowCount() - 1;
	//			}
	//			if (this->isVisible()) {
	//				table3->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
	//				table3->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	//				QTimer::singleShot(0, table3, [=]() {
	//					QModelIndex index = table3->model()->index(scrollRow, 0);
	//					if (index.isValid()) {
	//						table3->scrollTo(index, QAbstractItemView::PositionAtCenter);
	//					}
	//					});
	//			}
	//			// find the previous done row
	//			int lastDoneRow = -1;
	//			for each(int var in mapLine2Row.values())
	//			{
	//				if (var < row && var > lastDoneRow) {
	//					lastDoneRow = var;
	//				}
	//			}
	//			if (lastDoneRow != -1) {
	//				if (unsetPreviousLine) {
	//					table3->setRowSelection(lastDoneRow, false);
	//				}
	//				table3->setValue(lastDoneRow - 1, 0, "N");
	//			}
	//		}
	//		//else if (line == 19) {
	//		//	table3->setValue(table3->getDataRowCount() - 1, 0, "N");
	//		//}
	//		}, Qt::QueuedConnection);
	//	});
	

	//m_ncMachine->GetRealtimeJsonMachine()->setFunctionDoPerLineAfter([this, runManualForm, unsetPreviousLine](cb::JSON::ValuePtr sink, int line) {
	//	if (sink->has("modubs_ret") && sink->getS32("modubs_ret") == 1) {
	//		if (lastLine4DoneOne != -1) {// && sink->getString("type") == "move" && !sink->exists("rapid")) {
	//			int row = lastLine4DoneOne;
	//			// set last row to "否"
	//			if (row - 1 >= 0) {
	//				QMetaObject::invokeMethod(this, [this, unsetPreviousLine, row, runManualForm]() {
	//					//if (unsetPreviousLine) {
	//					//	table2->setRowSelection(row, false);
	//					//}
	//					runManualForm->table2->setValue(row - 1, -1, "false");
	//					runManualForm->table2->setValue(row - 1, 0, "false");
	//					}, Qt::QueuedConnection);
	//			}
	//		}
	//		//if (lastLine4DoneMulti != -1) {// && sink->getString("type") == "move" && !sink->exists("rapid")) {
	//		//	int row = lastLine4DoneMulti;
	//		//	// set last row to "否"
	//		//	if (row - 1 >= 0) {
	//		//		QMetaObject::invokeMethod(this, [this, unsetPreviousLine, row]() {
	//		//			if (unsetPreviousLine) {
	//		//				table3->setRowSelection(row, false);
	//		//			}
	//		//			table3->setValue(row - 1, 0, "N");
	//		//			}, Qt::QueuedConnection);
	//		//	}
	//		//}
	//	}
	//	});

	m_ncMachine->RunGCode(gcode, [this, runManualForm]() {
		SaveData();

		QMetaObject::invokeMethod(this, [this, runManualForm]() {
			runManualForm->table2->setRowSelection(-1, false);
			this->table3->setRowSelection(-1, false);
			}, Qt::QueuedConnection);
	});
}

void RunManualMulti::LoadData(QString filePath)
{
	if (filePath == NULL) {
		filePath = GetDataFilePath(this);
	}
	table3->deserialize(SystemSettings::AppendDataFilePath(filePath, table3->objectName()));

	QString filePath2 = SystemSettings::GetDataFilePath(m_name4RunAutoOne);
	filePath2 = SystemSettings::AppendDataFilePath(filePath2, QString("%1_table2").arg(m_name4RunAutoOne));
	table2->deserialize(filePath2);
	int n = 0;
	for (int i = 0; i < table2->getDataCount(); ++i) {
		if (!QLineEditLikeButton::IsYes(table2->getValue(i, 0))) {
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
}

void RunManualMulti::SaveData(QString filePath)
{
	if (filePath == NULL) {
		filePath = GetDataFilePath(this);
	}
	table3->serialize(SystemSettings::AppendDataFilePath(filePath, table3->objectName()));

	BaseChildWindow::SaveData(filePath);
}

void RunManualMulti::UpdateState()
{
	BaseChildWindowWithTools::UpdateState();

	bool isRunning = m_ncMachine->IsGCodeRunning() || m_ncMachine->GetNowOperating() > 0;
	table3->setReadOnly(isRunning);

	ui.lineEditZKaisi->setReadOnly(isRunning);
	ui.lineEditZAnQuan->setReadOnly(isRunning);
	ui.btnJgff->setEnabled(!isRunning);
	ui.btnJgff2->setEnabled(!isRunning);

	mbw->setEnabled(!isRunning);
}


//void RunManualMulti::on_btnShortcutGrid1_clicked()
//{
//	PointGenratorByGrid dlg(this);
//	QVector<double> rs = dlg.generatePoints();
//	if (rs.count() > 0) {
//		for (int i = 0; i < rs.count(); i ++)
//		{
//			if (i / 2 >= table3->getDataCount()) {
//				table3->insertNewRow(i / 2);
//			}
//			table3->setValue(i / 2, i % 2, QString::number(rs[i], 'f', 3));
//		}
//	}
//}

void RunManualMulti::on_btnJghs_clicked()
{
	BaseMainWindow* mainWindow = GetMainWindow();
	mainWindow->showChildWindow(tr("SDDG"));
}