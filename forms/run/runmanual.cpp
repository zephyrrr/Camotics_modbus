#include "runmanual.h"
#include <QHeaderView>
#include <QComboBox>
#include <QCompleter>
#include <QButtonGroup>
#include "widgets/qlineedit4axisvalue.h"
#include "modbus/NCMachineParametersC.h"
#include "../manual/zhuzhongxingform.h"
#include "widgets/multibuttonswidget.h"
#include "modbus/NCMachine.h"
#include "../../utils/gcodeutils.h"

class QCompleterWithoutInput : public QCompleter
{
public:
	QCompleterWithoutInput(const QStringList& words, QObject* parent = nullptr)
		: QCompleter(words, parent)
	{
	}

	bool eventFilter(QObject* o, QEvent* e) override
	{
		if (e->type() == QEvent::FocusIn) {
			QLineEdit* lineEdit = qobject_cast<QLineEdit*>(this->widget());
			if (lineEdit && !lineEdit->isReadOnly()) {
				this->complete();
			}
		}
		return QCompleter::eventFilter(o, e);
	}
};

RunManual::RunManual(QWidget* parent, QString objectName)
	: BaseChildWindowWithTools(parent, TOOL_LOAD_SAVE | TOOL_GCODE), 
	m_fangDianCanShuForm(NULL)
{
	this->setObjectName(objectName);

	ui.setupUi(this);
	stackedChildWidget = new QStackedWidget(this);
	stackedChildWidget->setWindowFlags((Qt::Tool | Qt::FramelessWindowHint | Qt::WindowTitleHint | Qt::CustomizeWindowHint) & ~Qt::WindowContextHelpButtonHint);
	stackedChildWidget->hide();
	 
	connect(ui.inAbsolute, &QPushButton::toggled, this, &RunManual::HideDetailForm);
	

	QString rowHeaders[] = { "X", "Y", "Z" };
	QString headers[] = { QStringLiteral("坐标轴"), QStringLiteral("深度") };
	table1 = new ButtonEditTableWidget(this);
	table1->createForm(3, 1, [this](int col, QWidget* parent) {
		QLineEdit4AxisValue* p = new QLineEdit4AxisValue(parent);
		p->setAllowNull(true);
		connect(p, &QLineEdit4Keyboard::focusChanged, [this, p](bool b) {
			if (b) {
				SaveFanDianCanShuForm();
			}
			});

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
	QString headers2[] = { QStringLiteral("回数"), QStringLiteral("加工"), "C.No", "LN", "LP", "STEP", QStringLiteral("加工余量"), QStringLiteral("定时"), QStringLiteral("时间"), QStringLiteral("详细") };
	QString defaultRowValues[] = { "", "Y", "000", "000", "0000", "0.000", "0.000", "OFF", "00000", "..." };
	int visibleColumnCount = 9;
	int hiddenColumnCount = 1;

	std::function<ILineEdit* (int, QWidget*)> function4CreateLineEdit = [this](int col, QWidget* parent) {
		ILineEdit* ret = NULL;
		auto namesRegex = NCMachineParametersC::instance().namesRegex;
		QString regStr;

		if (col == 1) {
			QLineEditLikeButton* btn = new QLineEditLikeButton(parent);
			//auto a = btn->styleSheet();
			//btn->setStyleSheet(QApplication::instance()->styleSheet());
			connect(btn, &QLineEdit::textChanged, [btn, parent](const QString& s) {
				ButtonEditTableWidget* table = qobject_cast<ButtonEditTableWidget*>(parent);
				int rowHere = table->getRowOfEdit(1, btn);
				/*if (s == QStringLiteral("否")) {
					btn->setStyleSheet("background-color: lightgray;");
				}
				
				auto p = btn->parentWidget();
				if (p == NULL){
					return;
				}
				auto pos = btn->pos();
				auto pos2 = p->pos();
				if (pos.x() <= 0 || pos.y() <= 0) {
					return;
				}
				auto index = table->indexAt(pos);
				if (!index.isValid())
					return;
				int rowHere = index.row();*/
				
				bool isOk = btn->IsYes();
				table->setPropertyOnRow(rowHere, "shouldRun", isOk);
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

				FangDianCanShuSingleForm form(NULL);
				QString cCode = table2->getValue(row, 9);
				if (!cCode.isEmpty()) {
					form.SetValuesAsCCode(cCode);
				}
				QString cNo = form.GetDynamicForm(0)->getValue("C.No");
				QString cNo2 = btn->text();
				if (cNo != cNo2) {
					form.LoadFromParametersC(cNo2.toInt());
					form.GetDynamicForm(0)->setValue("C.No", table2->getValue(row, 1));
					form.GetDynamicForm(1)->setValue("LN", table2->getValue(row, 2));
					form.GetDynamicForm(1)->setValue("LP", table2->getValue(row, 3));
					form.GetDynamicForm(1)->setValue("STEP", table2->getValue(row, 4));
					QString c = form.GetValuesAsCCode();
					table2->setValue(row, 9, c);

				}
				});

			connect(btn, &QLineEdit4Keyboard::focusChanged, [this, btn](bool b) {
				int row = table2->indexAt(btn->pos()).row() - 1;
				if (b) {
					SaveFanDianCanShuForm();
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
					SaveFanDianCanShuForm();
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
					SaveFanDianCanShuForm();
				}
				});
		}
		else if (col == 5) {
			//QLineEdit4Fdcs* btn = new QLineEdit4Fdcs(-1, parent);
			QLineEdit4AxisValue* btn = new QLineEdit4AxisValue(parent);
			ret = btn;
			regStr = QString::fromStdString(namesRegex[9]);	// "STEP"
			connect(btn, &QLineEdit4Keyboard::focusChanged, [this, btn](bool b) {
				int row = table2->indexAt(btn->pos()).row() - 1;
				if (b) {
					SaveFanDianCanShuForm();
				}
				});
		}
		else if (col == 6) {
			QLineEdit4AxisValue* btn = new QLineEdit4AxisValue(parent);
			ret = btn;
			regStr = "^(0|[1-9]\\d*)?(\\.\\d+)?$";	 // "加工余量"
			connect(btn, &QLineEdit4Keyboard::focusChanged, [this, btn](bool b) {
				int row = table2->indexAt(btn->pos()).row() - 1;
				if (b) {
					SaveFanDianCanShuForm();
				}
				});
		}
		else if (col == 7) {
			//QLineEdit4Keyboard* btn = new QLineEdit4Keyboard(parent);
			QLineEditLikeCombo* btn = new QLineEditLikeCombo({
				{"0", "OFF"}, 
				{"1", "T"},
				{"2", "Z"}}, "OFF", parent);
			ret = btn;

			//QStringList words = { "1", "2", "0" };
			//QCompleter* completer = new QCompleterWithoutInput(words, lineEdit);
			//lineEdit->setCompleter(completer);
			// 

			regStr = "[012]";
			// 
			// 
			connect(btn, &QLineEdit4Keyboard::focusChanged, [this, btn](bool b) {
				int row = table2->indexAt(btn->pos()).row() - 1;
				if (b) {
					SaveFanDianCanShuForm();
				}
				});
		}
		else if (col == 8) {
			QLineEdit4Fdcs* btn = new QLineEdit4Fdcs(5, parent);
			ret = btn;
			regStr = "\\d{1,9}";
			connect(btn, &QLineEdit4Keyboard::focusChanged, [this, btn](bool b) {
				int row = table2->indexAt(btn->pos()).row() - 1;
				if (b) {
					SaveFanDianCanShuForm();
				}
				});
		}
		else if (col == 9) {
			auto btn = new NPushButton4Edit(parent);
			btn->setCheckable(true);
			//buttonGroup4Detail.addButton(btn);
			ret = btn;
			connect(btn, &QPushButton::clicked, [this, btn]() {
				int row = table2->indexAt(btn->pos()).row() - 1;
				if (btn->isChecked()) {
					ShowDetailForm(row);
				}
				else {
					SaveFanDianCanShuForm();
				}
				});
		}
		else {
			ret = NULL;
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
		table2->setColumnWidth(i, std::max(50, defaultRowValues[i].toUtf8().size() * 20));
	}
	connect(table2, &ButtonEditTableWidget::rowInserted, [this, defaultRowValues](int rowIndex) {
		for(int i=0; i<table2->getDataYCount(); ++i) {
			table2->setValue(rowIndex, i, defaultRowValues[i]);
		}
		});
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

	mbw = new MultiButtonsWidget(this);
	mbw->addDefaultButtons(table2);
	mbw->addButton(QStringLiteral("复位"), [this](bool) {
		if (table2->isReadOnly())
			return;
		this->table2->setRowSelection(-1, false);
		for (int i = 0; i < table2->getDataCount(); ++i) {
			table2->setValue(i, 0, "True");
			table2->setValue(i, -1, "False");
		}
		this->setFocus();
		});

	ui.verticalLayout_table2->insertWidget(0, mbw);
}

RunManual::~RunManual()
{
	if (m_fangDianCanShuForm != NULL) {
		delete m_fangDianCanShuForm;
	}
	if (mbw != NULL) {
		delete mbw;
	}
	if (table1 != NULL) {
		delete table1;
	}
	if (table2 != NULL) {
		delete table2;
	}
}
void RunManual::HideDetailForm()
{
	SaveFanDianCanShuForm();
}

void RunManual::ShowDetailForm(int row)
{
	SaveFanDianCanShuForm(false);

	if (m_fangDianCanShuForm != NULL) {
		QWidget* focusWidget = m_fangDianCanShuForm->focusWidget();
		if (focusWidget) {
			focusWidget->clearFocus();
		}
		m_fangDianCanShuForm->clearFocus();
		m_fangDianCanShuForm->GetDynamicForm(0)->clearIsEditing();
		m_fangDianCanShuForm->GetDynamicForm(1)->clearIsEditing();
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

		QList<QLineEdit*> lineEdits = m_fangDianCanShuForm->findChildren<QLineEdit*>();
		for (QLineEdit* lineEdit : lineEdits) {
			// 不保存
			lineEdit->setObjectName("");
		}
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
	m_fangDianCanShuForm->setFocus();

	//for (size_t i = 0; i < m_fangDianCanShuForm->GetDynamicForm(1)->count(); i++)
	//{
	//	auto x = m_fangDianCanShuForm->GetDynamicForm(1)->itemAt(i)->widget();
	//	x->clearFocus();
	//}
}

QString RunManual::GetGCodeOSub803(IDataTable* table2)
{
	QString cCode;
	cCode += QString::fromStdString(NCMachineParametersC::GetNamesAsString());

	QString gcode;
	gcode += QString(R"(o800 sub
G00 XH661 YH662
G00 ZH663

H121 = H661-H771
H122 = H662-H772
H123 = H663-H773
H111 = SQRT[H121*H121+H122*H122+H123*H123]

;MDIV16 AOD0.0300 LEJL0.150 LEJS2000.0

)");
	QHash<int, int> cNos;
	int cOperationCount = 1;
	for (int i = 0; i < table2->getDataCount(); ++i) {
		if (!QLineEditLikeButton::IsYes(table2->getValue(i, 0))) {
			continue;
		}
		if (table2->getValue(i, 1).isEmpty()) {
			continue;
		}

		
		QString s;
		if (table2->getValue(i, 6) == "T") {
			s += QString("G85T%1\n").arg(table2->getValue(i, 7));
		}
		else if (table2->getValue(i, 6) == "Z") {
			s += QString("G85Z%1\n").arg(table2->getValue(i, 7));
		}
		int row = i;
		int nowcNo = table2->getValue(row, 1).toInt();
		QString c = table2->getValue(i, 9);
		if (c.isEmpty()) {
			//FangDianCanShuSingleForm form(NULL);

			//form.LoadFromParametersC(nowcNo);
			//form.GetDynamicForm(0)->setValue("C.No", table2->getValue(row, 1));
			//form.GetDynamicForm(1)->setValue("LN", table2->getValue(row, 2));
			//form.GetDynamicForm(1)->setValue("LP", table2->getValue(row, 3));
			//form.GetDynamicForm(1)->setValue("STEP", table2->getValue(row, 4));
			//c = form.GetValuesAsCCode();

			//table2->setValue(i, 9, c);
			return "";
		}
		if (cNos.contains(nowcNo)) {
			//int nextcNo = cNos[nowcNo] + 10;
			//while (true) {
			//	if (!cNos.contains(nextcNo)) {
			//		cNos[nextcNo] = 1;
			//		c = c.replace("C" + QString::number(nowcNo).rightJustified(3, '0') + " = ", "C" + QString::number(nextcNo).rightJustified(3, '0') + " = ");
			//		nowcNo = nextcNo;
			//		break;
			//	}
			//	else {
			//		nextcNo += 10;
			//	}
			//}
		}
		else {
			cNos[nowcNo] = 1;

			c = QString::fromStdString(NCMachineParametersC::FormatCCode(EUtils::QString2StdString(c)));
			cCode += c;
		}

		s += QString("C%1 H000=%4 H222=%5 H001=%2 H002=%3\n")
			.arg(QString::number(nowcNo).rightJustified(3, '0'))
			.arg(table2->getValue(i, 2))
			.arg(table2->getValue(i, 3))
			.arg(table2->getValue(i, 4))
			.arg(table2->getValue(i, 5));
		s += QString("(debug, do_call_ui set_runmanual_table2_current_row_%1)").arg(i + 1) + "\n";
		s += "o801 call \n";

		int osubIndex = 803 + cOperationCount;
		s = QString("o%1 if[0 EQ #1 or %2 EQ #1]\n").arg(osubIndex).arg(cOperationCount) + s + QString("o%1 endif\n").arg(osubIndex);
		cOperationCount++;

		s += QString("(debug, do_call_ui unset_runmanual_table2_current_row_%1)").arg(i + 1) + "\n";

		gcode += s;

		//int nowLines = gcode.split("\n").size();
		//// last line and remove endif
		//if (mapLine2Row != NULL) {
		//	mapLine2Row->insert(nowLines - 2, i + 1);
		//}
	}

	//gcode += QString("G00 Z+0.000;\n");
	gcode += QString("o800 endsub\n");

	QString doM04 = "";
	DataForm* dataForm = DataForms::getInstance()->getDataForm("xitongshezhi3");
	if (dataForm->getValue("SDM04SX") == "1") {
		doM04 = "M04";
	}

	gcode += QString(R"(
o801 sub
H666 = H771 + [H661 - H771] * H222 / H111
H777 = H772 + [H662 - H772] * H222 / H111
H888 = H773 + [H663 - H773] * H222 / H111
STEPH000 LNH001 LPH002 G01 XH666 YH777 ZH888 %01
o801 endsub
)").arg(doM04);

	if (!cCode.isEmpty()) {
		gcode = cCode + "\n" + gcode;
	}

	return gcode;
}

void RunManual::UpdateState()
{
	BaseChildWindowWithTools::UpdateState();

	bool isRunning = m_ncMachine->IsGCodeRunning() || m_ncMachine->GetNowOperating() > 0;
	table1->setReadOnly(isRunning);
	table2->setReadOnly(isRunning);
	ui.inAbsolute->setEnabled(!isRunning);
	ui.inAbsolute2->setEnabled(!isRunning);
	mbw->setEnabled(!isRunning);

	if (m_fangDianCanShuForm != NULL) {
		m_fangDianCanShuForm->GetDynamicForm(0)->setReadOnly(isRunning);
		m_fangDianCanShuForm->GetDynamicForm(1)->setReadOnly(isRunning);
	}
}

int RunManual::SetCurrentRunLine(bool initialize)
{
	if (initialize) {
		this->table2->setRowSelection(-1, false);
		return -1;
	}
	throw std::invalid_argument("Not implemented");

	int selectedLine = -1;
	for (int i = 0; i < table2->getDataCount(); ++i) {
		if (!QLineEditLikeButton::IsYes(table2->getValue(i, 0))) {
			continue;
		}
		if (table2->getValue(i, 1).isEmpty()) {
			continue;
		}
		
		int cNo = table2->getValue(i, 1).toInt();
		auto ln = EUtils::QString2StdString(table2->getValue(i, 2));
		auto lp = EUtils::QString2StdString(table2->getValue(i, 3));

		int row = i + 1;
		if (m_ncMachine->GetG01Data().isRunning && m_ncMachine->GetG01Data().cNo == cNo && NCMachineParametersC::GetCurrentByName("LN") == ln && NCMachineParametersC::GetCurrentByName("LP") == lp) {
			table2->setRowSelection(row, true);
			selectedLine = row;
			int scrollRow = row + 0;
			if (scrollRow >= table2->rowCount()) {
				scrollRow = table2->rowCount() - 1;
			}
			if (this->isVisible()) {
				table2->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
				table2->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
				QTimer::singleShot(0, table2, [=]() {
					QModelIndex index = table2->model()->index(scrollRow, 0);
					if (index.isValid()) {
						table2->scrollTo(index, QAbstractItemView::PositionAtCenter);
					}
					});
			}
			//break;
		}
		else {
			table2->setRowSelection(row, false);
		}
	}
	return selectedLine;
}

//QString RunManual::GetGCodeOne(bool isAbsolutePosition, double toAxisLength[], double startAxisPositions[])
//{
//	//QHash<int, int> mapLine2Row;
//
//	QString gcode;
//
//	int axisLen = 3;
//	//char toAxis[] = { 'X', 'Y', 'Z' };
//
//	if (!isAbsolutePosition) {
//		// G91
//		for (int i = 0; i < axisLen; ++i) {
//			if (toAxisLength[i] != INFINITE) {
//				toAxisLength[i] += startAxisPositions[i];
//			}
//		}
//	}
//
//	for (int i = 0; i < axisLen; ++i) {
//		double d = startAxisPositions[i];
//		gcode += QString("H%1   = %2\n").arg(661 + i).arg(d, 0, 'f', 3);
//		gcode += QString("H%1   = %2\n").arg(771 + i).arg(toAxisLength[i] == INFINITE ? d : toAxisLength[i], 0, 'f', 3);
//	}
//	gcode += QString("o800 call [0];");
//
//	// 1
//
//	//if (mapLine2Row != NULL) {
//	//	mapLine2Row->clear();
//	//}
//	
//	// 2
//
//	//if (!cCode.isEmpty()) {
//	//	gcode = cCode + "\n" + gcode;
//	//	
//	//	int cCodeLines = cCode.split("\n").size();
//	//	if (mapLine2Row != NULL) {
//	//		QHash<int, int> mapLine2Row2;
//	//		for (int key : mapLine2Row->keys()) {
//	//			mapLine2Row2[key + cCodeLines] = mapLine2Row->value(key);
//	//		}
//	//		mapLine2Row->clear();
//	//		*mapLine2Row = mapLine2Row2;
//	//	}
//	//}
//
//	return gcode;
//}

QString RunManual::GetGCodeV2()
{
	int axisLen = 3;
	double toAxisLength[] = { INFINITE, INFINITE, INFINITE };
	for (int i = 0; i < axisLen; ++i) {
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

	double axisPositions[] = { 0.0, 0.0, 0.0 };
	for (int i = 0; i < axisLen; ++i) {
		axisPositions[i] = m_ncMachine->GetController()->getAxisPosition(GCodeTool::axis[i]);
	}
	//QString gcode = GetGCodeStatic(table1, table2, ui.inAbsolute->isChecked(), axisPositions, &this->mapLine2Row);
	QString gcode = GetGCodeOSub803(table2);
	if (gcode.isEmpty()) {
		return QString();
	}
	gcode += "\n\n";
	gcode += "G90 G11 G24 T84\n";

	//gcode += GetGCodeOne(ui.inAbsolute->isChecked(), toAxisLength, axisPositions);
	if (!ui.inAbsolute->isChecked()) {
		// G91
		for (int i = 0; i < axisLen; ++i) {
			if (toAxisLength[i] != INFINITE) {
				toAxisLength[i] += axisPositions[i];
			}
		}
	}
	for (int i = 0; i < axisLen; ++i) {
		double d = axisPositions[i];
		gcode += QString("H%1   = %2\n").arg(661 + i).arg(d, 0, 'f', 3);
		gcode += QString("H%1   = %2\n").arg(771 + i).arg(toAxisLength[i] == INFINITE ? d : toAxisLength[i], 0, 'f', 3);
	}
	gcode += QString("o800 call [0]");


	int beepTime = DataForms::getInstance()->getDataForm("xitongshezhi5")->getValue("FDWCHMJSJ").toInt() * 1000;
	gcode += QString(R"(
T85
do_beep %1
do_light 2
(debug, do_call_ui reset_runmanual_table2_current_row)
M02
)").arg(beepTime);

	QList<int> todoRows;
	for (int i = 0; i < table2->getDataCount(); ++i) {
		if (!QLineEditLikeButton::IsYes(table2->getValue(i, 0))) {
			continue;
		}
		if (table2->getValue(i, 1).isEmpty()) {
			continue;
		}
		todoRows.append(i + 1);
	}
	//QStringList lines = gcode.split('\n'); // Splits by newline character
	//int lineNumber = 0;
	//int findedRow = 0;
	//for (const QString& line : lines) {
	//	lineNumber++;
	//	if (line.contains("o801 call")) {
	//		this->mapLine2Row[lineNumber] = (todoRows[findedRow] - 1) % table2->getDataCount() + 1;
	//		findedRow++;
	//	}
	//}

	return gcode;
}

QString RunManual::GetGCodeV1()
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
do_beep 100
M02;
;
N0000;
)");
	
	gcode += "T84;\n";

	mapLine2Row.clear();
	QString cCode;
	cCode += QString::fromStdString(NCMachineParametersC::GetNamesAsString());

	QHash<int, int> cNos;
	for (int i = 0; i < table2->getDataCount(); ++i) {
		if (!QLineEditLikeButton::IsYes(table2->getValue(i, 0))) {
			continue;
		}
		if (table2->getValue(i, 1).isEmpty()) {
			continue;
		}

		if (table2->getValue(i, 6) == "T") {
			gcode += QString("G85T%1;\n").arg(table2->getValue(i, 7));
		}
		else if (table2->getValue(i, 6) == "Z") {
			gcode += QString("G85Z%1;\n").arg(table2->getValue(i, 7));
		}
		QString s;
		if (table2->getDataYCount() >= 10) {
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
		//gcode += QString(QString("do_message_yn _%1\n").arg(i));

		int nowLines = gcode.split("\n").size();
		// last line
		mapLine2Row[nowLines - 1] = i + 1;
	}
	//gcode += QString("G00 %1+0.000;\n").arg(toAxis);
	gcode += QString("M99; \n");

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

void RunManual::RunGCode()
{
	SystemSettings::instance().LastRunNCFileName = QStringLiteral("手动单个");

	QString gcode = GetGCode();

	this->table2->setRowSelection(-1, false);
	//this->setEnabled(false);
	lastLine4DoneOne = -1;
	//bool unsetPreviousLine = false;

	m_ncMachine->GetRealtimeJsonMachine()->setFunctionDoWhenCallUI([this](std::string s) {
		QMetaObject::invokeMethod(this, [this, s]() {
			QString s2 = QString::fromStdString(s).trimmed();
			if (s2.startsWith("set_runmanual_table2_current_row_")) {
				lastLine4DoneOne = s2.split('_').last().toInt();
				this->table2->setRowSelection(lastLine4DoneOne, true);
				this->table2->ScrollToRow(lastLine4DoneOne);
			}
			else if (s2.startsWith("unset_runmanual_table2_current_row_")) {
				int row = s2.split('_').last().toInt();
				this->table2->setRowSelection(row, false);
				this->table2->setValue(row - 1, -1, "false");
				this->table2->setValue(row - 1, 0, "false");
			}
			else if (s2 == "reset_runmanual_table2_current_row") {
				lastLine4DoneOne = -1;
				this->table2->setRowSelection(-1, false);
				this->table2->ScrollToRow(0);
			}
			}, Qt::QueuedConnection);
		});

	//m_ncMachine->GetRealtimeJsonMachine()->setFunctionDoPerLineBefore([this](cb::JSON::ValuePtr sink, int line) {
	//	if (sink->getString("type") == "move" && !sink->exists("rapid")) {
	//		QMetaObject::invokeMethod(this, [this]() {
	//			int a = SetCurrentRunLine(false);
	//			if (a != -1) {
	//				lastLine4MapLine2Row = a;
	//			}
	//			}, Qt::QueuedConnection);
	//	}
	//	});

	//m_ncMachine->GetRealtimeJsonMachine()->setFunctionDoPerLineAfter([this, unsetPreviousLine](cb::JSON::ValuePtr sink, int line) {
	//	if (sink->has("modubs_ret") && sink->getS32("modubs_ret") == 1) {
	//		if (lastLine4DoneOne != -1) {// && sink->getString("type") == "move" && !sink->exists("rapid")) {
	//			int row = lastLine4DoneOne;
	//			// set last row to "否"
	//			if (row - 1 >= 0) {
	//				QMetaObject::invokeMethod(this, [this, unsetPreviousLine, row]() {
	//					if (unsetPreviousLine) {
	//						table2->setRowSelection(row, false);
	//					}
	//					table2->setValue(row - 1, -1, "false");
	//					table2->setValue(row - 1, 0, "false");
	//					}, Qt::QueuedConnection);
	//			}
	//		}
	//	}
	//	});

	m_ncMachine->RunGCode(gcode, [this]() {
		SaveData();

		//QMetaObject::invokeMethod(this, [this]() {
		//	//for (int i = 0; i < table2->getDataCount(); ++i) {
		//	//	table2->setRowSelection(i + 1, false);
		//	//}
		//	SetCurrentRunLine(true);
		//}, Qt::QueuedConnection);
	});
}

QString RunManual::GetGCode()
{
	SaveFanDianCanShuForm();
	SaveData();

	return GetGCodeV2();
}

void RunManual::LoadData(QString filePath)
{
	BaseChildWindow::LoadData(filePath);

	if (filePath == NULL) {
		filePath = GetDataFilePath(this, GetProjectDir());
	}
	table1->deserialize(SystemSettings::AppendDataFilePath(filePath, table1->objectName()));
	table2->deserialize(SystemSettings::AppendDataFilePath(filePath, table2->objectName()));
}

void RunManual::SaveData(QString filePath)
{
	if (filePath == NULL) {
		filePath = GetDataFilePath(this, GetProjectDir());
	}
	table1->serialize(SystemSettings::AppendDataFilePath(filePath, table1->objectName()));
	table2->serialize(SystemSettings::AppendDataFilePath(filePath, table2->objectName()));

	BaseChildWindow::SaveData(filePath);
}

void RunManual::SaveFanDianCanShuForm(bool invisible)
{
	if (m_fangDianCanShuFormRow != -1 && m_fangDianCanShuForm != NULL) {
		m_fangDianCanShuForm->clearFocus();

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
		QWidget* cellWidget = table2->cellWidget(row + 1, 9);
		qobject_cast<QPushButton*>(cellWidget)->setChecked(false);
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

void RunManual::showEvent(QShowEvent* event)
{
	BaseChildWindowWithTools::showEvent(event);

	if (!m_bToolsInitialized) {
		m_bToolsInitialized = true;

		QList<QPushButton*> btns = m_tool4ZeroAndHalfAxis->findChildren<QPushButton*>();
		for (QPushButton* btn : btns)
		{
			connect(btn, &QPushButton::pressed, this, &RunManual::HideDetailForm);
		}
	}
}

void RunManual::hideEvent(QHideEvent* event)
{
	SaveFanDianCanShuForm();

	BaseChildWindowWithTools::hideEvent(event);
}

void RunManual::mousePressEvent(QMouseEvent* event)
{
	BaseChildWindowWithTools::mousePressEvent(event);

	SaveFanDianCanShuForm();
}

//void RunManual::LoadDataFromCCode(QString cCode)
//{
//	QStringList ss = cCode.split('\n');
//	for (int row = 0; row < ss.size(); ++row) {
//		if (row >= this->table2->getDataCount()) {
//			this->table2->insertNewRow();
//			table2->setValue(row, 9, ss[row]);
//			ShowDetail(row);
//		}
//	}
//
//	int n = this->table2->rowCount();
//	for (int row = n - 1; row >= ss.size(); --row) {
//		table2->deleteRow(row);
//	}
//}
