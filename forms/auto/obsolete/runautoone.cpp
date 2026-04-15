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

RunAutoOne::RunAutoOne(QWidget* parent)
	: BaseChildWindow(parent), m_fangDianCanShuForm(NULL)
{
	this->setObjectName("runautoone");

	ui.setupUi(this);
	stackedChildWidget = new QStackedWidget(this);
	stackedChildWidget->setWindowFlags((Qt::Tool | Qt::FramelessWindowHint | Qt::WindowTitleHint | Qt::CustomizeWindowHint) & ~Qt::WindowContextHelpButtonHint);
	stackedChildWidget->hide();

	
	QString rowHeaders[] = { "X", "Y", "Z" };
	QString headers[] = { tr("ZBZ"), tr("SD") };
	table1 = new ButtonEditTableWidget(this);
	table1->createForm(3, 1, [](int col, QWidget* parent) {
		QLineEdit4AxisValue* p = new QLineEdit4AxisValue(parent);
		p->setAllowNull(true);
		return p;
		});
	table1->setObjectName(QString("%1_table1").arg(this->objectName()));
	table1->hideRow(0);
	for (int i = 1; i <= 3; ++i)
	{
		qobject_cast<QPushButton*>(table1->cellWidget(i, 0))->setText(rowHeaders[i - 1]);
	}
	table1->setColumnWidth(1, 250);
	//connect(table1, &ButtonEditTableWidget::rowButtonClicked, [this](int row, bool checked) {
	//	//if (!checked) {
	//	//	table1->setValue(row, 0, "");
	//	//}
	//	//for (int i = 0; i < 3; ++i)
	//	//{
	//	//	if (i == row - 1)
	//	//	{
	//	//		if (table1->getValue(i, 0).isEmpty()) {
	//	//			table1->setValue(i, 0, "00000.000");
	//	//		}
	//	//	}
	//	//	else {
	//	//		table1->setValue(i, 0, "");
	//	//	}
	//	//}
	//	//QLineEdit* edit = qobject_cast<QLineEdit*>(this->table1->cellWidget(row, 1));
	//	//if (edit->text().isEmpty()) {
	//	//	edit->setText("00000.000");
	//	//}
	//});
	QString headers2[] = { tr("HS"), tr("JG"), "C.No", "LN", "LP", "STEP", tr("JGYL"), tr("DS"), tr("SJ"), tr("XX") };
	QString defaultRowValues[] = { "", tr("S"), "001", "000", "0000", "0.000", "0.000", "0", "0000", "..." };
	int visibleColumnCount = 9;
	int hiddenColumnCount = 1;

	std::function<ILineEdit* (int, QWidget*)> function4CreateLineEdit = [this, defaultRowValues](int col, QWidget* parent) {
		ILineEdit* ret = NULL;
		auto namesRegex = NCMachineParametersC::instance().namesRegex;
		QString regStr;

		if (col == 1) {
			QLineEditLikeButton* btn = new QLineEditLikeButton(parent);
			//auto a = btn->styleSheet();
			//btn->setStyleSheet(QApplication::instance()->styleSheet());
			connect(btn, &QLineEdit::textChanged, [btn, parent](const QString& s) {
				ButtonEditTableWidget* table = qobject_cast<ButtonEditTableWidget*>(parent);
				int rowHere = table->indexAt(btn->pos()).row();
				bool isOk = s == tr("S");
				table->setReadOnlyOnRow(rowHere, !isOk);
				});
			ret = btn;
		}
		else if (col == 2) {
			QLineEdit4Fdcs* btn = new QLineEdit4Fdcs(3, parent);
			ret = btn;
			regStr = "\\d{1,3}";

			connect(btn, &QLineEdit::editingFinished, [btn, this]() {
				//const QString& s = "";
				int row = table2->indexAt(btn->pos()).row() - 1;
				QString cNo = table2->getValue(row, 2);
				QString cNo2 = btn->text();
				if (cNo != cNo2) {
					table2->setValue(row, 9, "");
					if (m_fangDianCanShuForm->isVisible()) {
						m_fangDianCanShuForm->LoadFromParametersC(cNo2.toInt());
					}
				}
				});

			connect(btn, &QLineEdit4Keyboard::focusChanged, [this, btn](bool b) {
				int row = table2->indexAt(btn->pos()).row() - 1;
				if (b) {
					ShowDetail(row);
				}
				});
		}
		else if (col == 3) {
			QLineEdit4Fdcs* btn = new QLineEdit4Fdcs(3, parent);
			ret = btn;
			regStr = QString::fromStdString(namesRegex[7]);	// LN
			connect(btn, &QLineEdit4Keyboard::focusChanged, [this, btn](bool b) {
				int row = table2->indexAt(btn->pos()).row() - 1;
				if (b) {
					ShowDetail(row);
				}
				});
		}
		else if (col == 4) {
			QLineEdit4Fdcs* btn = new QLineEdit4Fdcs(4, parent);
			ret = btn;
			regStr = QString::fromStdString(namesRegex[8]);	// "LP"
			connect(btn, &QLineEdit4Keyboard::focusChanged, [this, btn](bool b) {
				int row = table2->indexAt(btn->pos()).row() - 1;
				if (b) {
					ShowDetail(row);
				}
				});
		}
		else if (col == 5) {
			QLineEdit4Fdcs* btn = new QLineEdit4Fdcs(-1, parent);
			ret = btn;
			regStr = QString::fromStdString(namesRegex[9]);	// "STEP"
			connect(btn, &QLineEdit4Keyboard::focusChanged, [this, btn](bool b) {
				int row = table2->indexAt(btn->pos()).row() - 1;
				if (b) {
					ShowDetail(row);
				}
				});
		}
		else if (col == 6) {
			QLineEdit4AxisValue* btn = new QLineEdit4AxisValue(parent);
			ret = btn;
			connect(btn, &QLineEdit4Keyboard::focusChanged, [this, btn](bool b) {
				int row = table2->indexAt(btn->pos()).row() - 1;
				if (b) {
					ShowDetail(row);
				}
				});
		}
		else if (col == 7) {
			QLineEdit4Keyboard* btn = new QLineEdit4Keyboard(parent);
			ret = btn;
			//QStringList words = { "1", "2", "0" };
			//QCompleter* completer = new QCompleterWithoutInput(words, lineEdit);
			//lineEdit->setCompleter(completer);
			regStr = "[012]";
			connect(btn, &QLineEdit4Keyboard::focusChanged, [this, btn](bool b) {
				int row = table2->indexAt(btn->pos()).row() - 1;
				if (b) {
					ShowDetail(row);
				}
				});
		}
		else if (col == 8) {
			QLineEdit4Fdcs* btn = new QLineEdit4Fdcs(4, parent);
			ret = btn;
			regStr = "\\d{1,9}";
			connect(btn, &QLineEdit4Keyboard::focusChanged, [this, btn](bool b) {
				int row = table2->indexAt(btn->pos()).row() - 1;
				if (b) {
					ShowDetail(row);
				}
				});
		}
		else if (col == 9) {
			auto btn = new NPushButton4Edit(parent);
			ret = btn;
			connect(btn, &QPushButton::clicked, [this, btn]() {
				//ShowDetail(row);
				SaveFanDianCanShuForm(true);
				});
		}
		else {
			ret = NULL;
		}

		if (ret != NULL) {
			ret->setText(defaultRowValues[col]);
		}
		if (!regStr.isEmpty()) {
			QLineEdit* edit2 = dynamic_cast<QLineEdit*>(ret);
			if (edit2 != NULL) {
				QRegExp regExp(regStr);
				QRegExpValidator* validator = new QRegExpValidator(regExp, edit2);
				edit2->setValidator(validator);
			}
		}
		return ret;
		};

	table2 = new ButtonEditTableWidget(this);
	table2->createForm(1, visibleColumnCount + hiddenColumnCount, function4CreateLineEdit);
	for (int i = 0; i < hiddenColumnCount; ++i) {
		table2->setColumnHidden(1 + visibleColumnCount + i, true);
	}
	table2->setObjectName(QString("%1_table2").arg(this->objectName()));
	for (int i = 0; i < 1 + visibleColumnCount; ++i)
	{
		qobject_cast<QLabel*>(table2->cellWidget(0, i))->setText(headers2[i]);
		table2->setColumnWidth(i, std::max(50, defaultRowValues[i].toUtf8().size() * 22));
	}

	for (int i = 0; i < 1; ++i)
	{
		emit table2->rowInserted(i);
	}

	table2->setSelectionMode(QAbstractItemView::SingleSelection);
	connect(table2, &ButtonEditTableWidget::currentCellChanged, [this](int currentRow, int currentColumn, int previousRow, int previousColumn) {
		// not button column
		if (currentRow != previousRow && currentColumn != 9) {
			SaveFanDianCanShuForm();
		}
		});

	ui.verticalLayout_table1->addWidget(table1);
	ui.verticalLayout_table2->addWidget(table2);
	//for (int i = 1; i <= 3; ++i)
	//{
	//	ZhuZhongXingForm::setButton4LineEdit(qobject_cast<QPushButton*>(table1->cellWidget(i, 0)));
	//}

	MultiButtonsWidget* mbw = new MultiButtonsWidget(this);
	mbw->addDefaultButtons(table2);
	mbw->removeButton(5);
	mbw->removeButton(4);
	ui.verticalLayout_table2->insertWidget(0, mbw);

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
	ui.pushButton_6->hide();
}

RunAutoOne::~RunAutoOne()
{
	if (m_fangDianCanShuForm != NULL) {
		delete m_fangDianCanShuForm;
	}
}

void RunAutoOne::ShowDetail(int row)
{
	SaveFanDianCanShuForm(false);

	if (m_fangDianCanShuForm != NULL) {
		QWidget* focusWidget = m_fangDianCanShuForm->focusWidget();
		if (focusWidget) {
			focusWidget->clearFocus();
		}
	}

	m_fangDianCanShuFormRow = row;
	if (m_fangDianCanShuForm == NULL) {
		m_fangDianCanShuForm = new FangDianCanShuSingleForm(stackedChildWidget);
		m_fangDianCanShuForm->setObjectName(QString("fangdiancanshu_%1").arg(row));
		//form->setWindowFlags((Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowTitleHint | Qt::CustomizeWindowHint) & ~Qt::WindowContextHelpButtonHint);
		m_fangDianCanShuForm->GetDynamicForm(0)->hideItem("C.No");
		m_fangDianCanShuForm->GetDynamicForm(1)->hideItem("LN");
		m_fangDianCanShuForm->GetDynamicForm(1)->hideItem("LP");
		m_fangDianCanShuForm->GetDynamicForm(1)->hideItem("STEP");

		stackedChildWidget->addWidget(m_fangDianCanShuForm);
		stackedChildWidget->setCurrentWidget(m_fangDianCanShuForm);
	}

	QString cCode = table2->getValue(row, 9);
	if (!cCode.isEmpty()) {
		m_fangDianCanShuForm->SetValuesAsCCode(cCode);
	}
	else {
		QString cIndex = this->table2->getValue(row, 1);
		if (!cIndex.isEmpty()) {
			m_fangDianCanShuForm->LoadFromParametersC(cIndex.toInt());
		}
	}
	m_fangDianCanShuForm->GetDynamicForm(0)->setValue("C.No", table2->getValue(row, 1));
	m_fangDianCanShuForm->GetDynamicForm(1)->setValue("LN", table2->getValue(row, 2));
	m_fangDianCanShuForm->GetDynamicForm(1)->setValue("LP", table2->getValue(row, 3));
	m_fangDianCanShuForm->GetDynamicForm(1)->setValue("STEP", table2->getValue(row, 4));

	if (!stackedChildWidget->isVisible()) {
		stackedChildWidget->setGeometry(0, 0, this->ui.labelPix->width() + 0, this->ui.labelPix->height() + this->ui.textBrowser->height() + 10);
		QPoint newPos = this->mapToGlobal(this->ui.labelPix->pos());
		stackedChildWidget->move(newPos);
		stackedChildWidget->show();
		stackedChildWidget->clearFocus();
	}
	m_fangDianCanShuForm->clearFocus();

	//for (size_t i = 0; i < m_fangDianCanShuForm->GetDynamicForm(1)->count(); i++)
	//{
	//	auto x = m_fangDianCanShuForm->GetDynamicForm(1)->itemAt(i)->widget();
	//	x->clearFocus();
	//}
}

QString RunAutoOne::GetGCodeV2()
{
	SaveFanDianCanShuForm();
	SaveData();

	int axisLen = 3;
	char toAxis[] = { 'X', 'Y', 'Z' };
	double toAxisLength[] = { INFINITE, INFINITE, INFINITE };

	for (int i = 0; i < 3; ++i) {
		if (table1->getValue(i, -1) != "True") {
			continue;
		}

		QString s = table1->getValue(i, 0);
		if (!s.isEmpty()) {
			double d = s.toDouble();
			toAxisLength[i] = d;
		}
	}
	if (toAxisLength[0] == INFINITE && toAxisLength[1] == INFINITE && toAxisLength[2] == INFINITE) {
		return QString();
	}

	QString gcode;
	for (int i = 0; i < axisLen; ++i) {
		double d = m_ncMachine->GetController()->getAxisPosition(toAxis[i]);
		gcode += QString("H%1   = %2\n").arg(661 + i).arg(d, 0, 'f', 3);
		gcode += QString("H%1   = %2\n").arg(771 + i).arg(toAxisLength[i] == INFINITE ? d : toAxisLength[i], 0, 'f', 3);
	}
	gcode += QString(R"(%1 G11 G24 T84;
M98P0000;
;
T85;
M02;
;
)").arg(ui.inAbsolute->isChecked() ? "G90" : "G91");

	gcode += QString(R"(N0000;
G00 XH661 YH662;
G00 ZH663;
;
H121 = H661-H771;
H122 = H662-H772;
H123 = H663-H773;
H111 = SQRT[H121*H121+H122*H122+H123*H123]; 
;
;MDIV16 AOD0.0300 LEJL0.150 LEJS2000.0;
;
)");


	mapLine2Row.clear();
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

		int row = i;
		int nowcNo = table2->getValue(row, 1).toInt();
		QString c = table2->getValue(i, 9);
		if (c.isEmpty()) {
			FangDianCanShuSingleForm form(stackedChildWidget);

			form.LoadFromParametersC(nowcNo);
			form.GetDynamicForm(0)->setValue("C.No", table2->getValue(row, 1));
			form.GetDynamicForm(1)->setValue("LN", table2->getValue(row, 2));
			form.GetDynamicForm(1)->setValue("LP", table2->getValue(row, 3));
			form.GetDynamicForm(1)->setValue("STEP", table2->getValue(row, 4));
			c = form.GetValuesAsCCode();

			table2->setValue(i, 9, c);
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

		s = QString("C%1 H000=%4 H222=%5 LN%2 LP%3 M98P0001;\n")
			.arg(QString::number(nowcNo).rightJustified(3, '0'))
			.arg(table2->getValue(i, 2))
			.arg(table2->getValue(i, 3))
			.arg(table2->getValue(i, 4))
			.arg(table2->getValue(i, 5));

		gcode += s;

		int nowLines = gcode.split("\n").size();
		// last line
		mapLine2Row[nowLines - 1] = i + 1;
	}

	//gcode += QString("G00 Z+0.000;\n");
	gcode += QString("M99;\n");

	gcode += QString(R"(;
N0001;
H666 = H771 + [H661 - H771] * H222 / H111;
H777 = H772 + [H662 - H772] * H222 / H111;
H888 = H773 + [H663 - H773] * H222 / H111;
STEPH000 G01 XH666 YH777 ZH888 M04;
N0004;
M99;
)");

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

QString RunAutoOne::GetGCodeV1()
{
	SaveFanDianCanShuForm();
	SaveData();

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

	QString gcode;
	for (int i = 0; i < toAxis.size(); ++i) {
		gcode += QString("H%1   = %2\n").arg(100 + i).arg(toAxisLength[i]);
	}

	gcode += QString("G11\n");

	if (ui.inAbsolute->isChecked())
		gcode += "G90\n";
	else
		gcode += "G91\n";

	gcode += QString(R"(M98P0000;
;
)");
	gcode += QString(R"(T85;
M02;
;
N0000;
)");

	gcode += "T84;\n";

	mapLine2Row.clear();
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
				FangDianCanShuSingleForm form(stackedChildWidget);

				form.LoadFromParametersC(nowcNo);
				form.GetDynamicForm(0)->setValue("C.No", table2->getValue(row, 1));
				form.GetDynamicForm(1)->setValue("LN", table2->getValue(row, 2));
				form.GetDynamicForm(1)->setValue("LP", table2->getValue(row, 3));
				form.GetDynamicForm(1)->setValue("STEP", table2->getValue(row, 4));
				c = form.GetValuesAsCCode();

				table2->setValue(i, 9, c);
			}
			else
			{
				if (!c.endsWith("\n")) {
					c += "\n";
				}
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
			c = QString::fromStdString(NCMachineParametersC::FormatCCode(EUtils::QString2StdString(c)));
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

		int nowLines = gcode.split("\n").size();
		// last line
		mapLine2Row[nowLines - 1] = i + 1;
	}
	//gcode += QString("G00 %1+0.000;\n").arg(toAxis);
	gcode += QString("M99; ");

	if (!cCode.isEmpty()) {
		gcode = cCode + "\n" + gcode;
		QHash<int, int> mapLine2Row2;
		int cCodeLines = cCode.split("\n").size();
		for (int key : mapLine2Row.keys()) {
			mapLine2Row2[key + cCodeLines] = mapLine2Row[key];
		}

		mapLine2Row = mapLine2Row2;
		// mapLine2Row2 is local variable ???. it's ok to use "="
		//for (auto it = mapLine2Row2.constBegin(); it != mapLine2Row2.constEnd(); ++it) {
		//	mapLine2Row.insert(it.key(), it.value());
		//}
	}

	return gcode;
}

void RunAutoOne::RunGCode()
{
	QString gcode = GetGCode();

	this->table2->setRowSelection(-1, false);
	this->setEnabled(false);

	m_ncMachine->GetRealtimeJsonMachine()->setFunctionDoPerLineBefore([this](cb::JSON::ValuePtr sink, int line) {
		QMetaObject::invokeMethod(this, [this, line]() {
			if (mapLine2Row.contains(line)) {
				int row = mapLine2Row[line];
				table2->setRowSelection(row, true);
			}
			}, Qt::QueuedConnection);
		});

	m_ncMachine->RunGCode(gcode, [this]() {
		QMetaObject::invokeMethod(this, [this]() {
			this->setEnabled(true);
			}, Qt::QueuedConnection);
		});
}

QString RunAutoOne::GetGCode()
{
	return GetGCodeV1();
}

void RunAutoOne::LoadData(QString filePath)
{
	if (filePath == NULL) {
		filePath = GetDataFilePath(this);
	}
	table1->deserialize(SystemSettings::AppendDataFilePath(filePath, table1->objectName()));
	table2->deserialize(SystemSettings::AppendDataFilePath(filePath, table2->objectName()));

	BaseChildWindow::LoadData(filePath);

	this->table1->setEnabled(!m_ncMachine->IsGCodeRunning());
	this->table2->setEnabled(!m_ncMachine->IsGCodeRunning());
}

void RunAutoOne::SaveData(QString filePath)
{
	if (filePath == NULL) {
		filePath = GetDataFilePath(this);
	}
	table1->serialize(SystemSettings::AppendDataFilePath(filePath, table1->objectName()));
	table2->serialize(SystemSettings::AppendDataFilePath(filePath, table2->objectName()));

	BaseChildWindow::SaveData(filePath);
}

void RunAutoOne::SaveFanDianCanShuForm(bool invisible)
{
	if (m_fangDianCanShuFormRow != -1 && m_fangDianCanShuForm != NULL) {
		int row = m_fangDianCanShuFormRow;
		m_fangDianCanShuForm->GetDynamicForm(0)->setValue("C.No", table2->getValue(row, 1));
		m_fangDianCanShuForm->GetDynamicForm(1)->setValue("LN", table2->getValue(row, 2));
		m_fangDianCanShuForm->GetDynamicForm(1)->setValue("LP", table2->getValue(row, 3));
		m_fangDianCanShuForm->GetDynamicForm(1)->setValue("STEP", table2->getValue(row, 4));
		QString s = m_fangDianCanShuForm->GetValuesAsCCode();
		table2->setValue(row, 9, s);
		auto s2 = table2->getValue(row, 9);
		if (invisible) {
			stackedChildWidget->hide();
		}

		m_fangDianCanShuFormRow = -1;
	}

	//for (auto it = m_fangDianCanShus.begin(); it != m_fangDianCanShus.end(); ++it) {
	//	if (it.key() == excludeRow)
	//		continue;
	//	if (it.value()->isVisible()) {
	//		int row = it.key();
	//		it.value()->GetDynamicForm(0)->setValue("C.No", table2->getValue(row, 1));
	//		DynamicForm* form = it.value()->GetDynamicForm(1);
	//		form->setValue("LN", table2->getValue(row, 2));
	//		form->setValue("LP", table2->getValue(row, 3));
	//		form->setValue("STEP", table2->getValue(row, 4));
	//		QString s = it.value()->GetValuesAsCCode();
	//		table2->setValue(row, 9, s);

	//		//it.value()->hide();
	//	}
	//}
	//if (excludeRow == -1) {
	//	stackedChildWidget->setCurrentWidget(NULL);
	//}
	//else {
	//	stackedChildWidget->setCurrentWidget(m_fangDianCanShus[excludeRow]);
	//}
}

void RunAutoOne::showEvent(QShowEvent* event)
{
	BaseChildWindow::showEvent(event);

	if (m_tool4ZeroAndHalfAxis == NULL) {
		m_tool4ZeroAndHalfAxis = new Tool4ZeroAndHalfAxis(m_ncMachine, this, 0);
		//QPushButton* btn1 = new QPushButton(tr("SDDMSC"), this);
		//m_tool4ZeroAndHalfAxis->AddItem(btn1);
		//QPushButton* btn2 = new QPushButton(tr("NWJSC"), this);
		Tool4Buttons* tools1 = new Tool4Buttons(QStringList() << tr("SDDMSC"), m_tool4ZeroAndHalfAxis);
		m_tool4ZeroAndHalfAxis->AddItem(tools1);
		Tool4Buttons* tools2 = new Tool4Buttons(QStringList() << tr("NWJSC"), m_tool4ZeroAndHalfAxis);
		m_tool4ZeroAndHalfAxis->AddItem(tools2);

		QPushButton* btn = tools1->getButton(0);
		connect(btn, &QPushButton::clicked, [this]() {
			if (this->table2->getDataRowCount() == 0) {
				return;
			}
			this->SaveData();

			RunAutoForm* w = this->GetParentOfThis<RunAutoForm>(this);
			w->windowB->SaveData();


			QString parentName = "runautoone";
			QString filePath2 = SystemSettings::GetDataFilePath(parentName);
			filePath2 = SystemSettings::AppendDataFilePath(filePath2, QString("%1_table2").arg(parentName));
			QString parentNameDest = "sdjg";
			QString filePath2Dest = SystemSettings::GetDataFilePath(parentNameDest);
			filePath2Dest = SystemSettings::AppendDataFilePath(filePath2Dest, QString("%1_table2").arg(parentNameDest));
			FormUtils::CopyFileWithOverwrite(filePath2, filePath2Dest);

			QString filePath1 = SystemSettings::GetDataFilePath(parentName);
			filePath1 = SystemSettings::AppendDataFilePath(filePath1, QString("%1_table1").arg(parentName));
			QString filePath1Dest = SystemSettings::GetDataFilePath(parentNameDest);
			filePath1Dest = SystemSettings::AppendDataFilePath(filePath1Dest, QString("%1_table1").arg(parentNameDest));
			FormUtils::CopyFileWithOverwrite(filePath1, filePath1Dest);

			parentName = "runautomulti";
			parentNameDest = "ddjg";
			QString filePath3 = SystemSettings::GetDataFilePath(parentName);
			filePath3 = SystemSettings::AppendDataFilePath(filePath3, QString("%1_table3").arg(parentName));
			QString filePath3Dest = SystemSettings::GetDataFilePath(parentNameDest);
			filePath3Dest = SystemSettings::AppendDataFilePath(filePath3Dest, QString("%1_table3").arg(parentNameDest));
			FormUtils::CopyFileWithOverwrite(filePath3, filePath3Dest);

			QString filePath4Dest = SystemSettings::GetDataFilePath(parentNameDest);
			DataForm* dataForm = DataForms::getInstance()->getDataForm(parentName);
			DataForm* dataFormDest = DataForms::getInstance()->getDataForm(parentNameDest);
			dataFormDest->setValue("lineEditZ1", dataForm->getValue("lineEditZ1"));
			dataFormDest->setValue("lineEditZ2", dataForm->getValue("lineEditZ2"));
			dataFormDest->serialize(filePath4Dest);

			BaseMainWindow* mainWindow = BaseChildWindow::GetMainWindow();
			mainWindow->showChildWindow(tr("SDDG"));
			});

		QPushButton* btn2 = tools2->getButton(0);
		connect(btn2, &QPushButton::clicked, [this]() {
			if (this->table2->getDataRowCount() == 0) {
				return;
			}

			RunAutoForm* w = this->GetParentOfThis<RunAutoForm>(this);
			w->windowB->LoadData();
			QString s = w->windowB->GetGCode();
			QString m_path = "data/nc";
			if (!s.isEmpty()) {
				QString filePath = NFileDialog::getSaveFileName(this, tr("BCWJ"), m_path, QStringLiteral("(*.nc)"));
				if (!filePath.isEmpty()) {

					NFile file(filePath);
					if (file.open(QIODevice::WriteOnly)) {
						file.write(s.toUtf8());
						file.close();

						BaseMainWindow* mainWindow = BaseChildWindow::GetMainWindow();
						mainWindow->showChildWindow(QStringLiteral("NC"));

						NcEditForm* form = dynamic_cast<NcEditForm*>(mainWindow->getChildWindow(QStringLiteral("NC")));
						form->OpenFile(filePath.toUtf8().data());
					}
				}
			}
			});

		Tool4ZeroAndHalfAxis::ShowIt(m_tool4ZeroAndHalfAxis);
	}
}

void RunAutoOne::hideEvent(QHideEvent* event)
{
	SaveFanDianCanShuForm();

	BaseChildWindow::hideEvent(event);
}