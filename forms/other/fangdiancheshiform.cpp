#include <QLineEdit>
#include <QStringBuilder>
#include "fangdiancheshiform.h"
#include "modbus/NCMachineParametersC.h"
#include "widgets/qlineedit4optionwidget.h"
#include "widgets/qlineedit4axisvalue.h"
#include "forms/edit/cp/widget_cp_ln.h"
#include "modbus/NCMachine.h"

FangDianCanShuSingleForm::FangDianCanShuSingleForm(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	m_dynamicForm0 = new DynamicForm();
	m_dynamicForm0->setObjectName("fangdiancanshu0");

	m_dynamicForm = new DynamicForm();
	m_dynamicForm->setObjectName("fangdiancanshu");

	NCMachineParametersC& cInst = NCMachineParametersC::instance();
	for (const auto& s : cInst.names) {
		m_pNames.push_back(QString::fromStdString(s));
	}
	std::function<ILineEdit* (QString, QWidget*)> funcToCreateEditor = [](QString caption, QWidget* parent){
		NCMachineParametersC& cInst2 = NCMachineParametersC::instance();
		ILineEdit* ret = NULL;
		//if (caption == "LN9999") {
		//	ret = new QLineEdit4OptionWidget(parent, new WidgetCPLN(parent));
		//}
		if (caption == "C.No.2") {
			ret = new QLineEdit4Keyboard(parent);
		}
		else {
			std::string caption2 = EUtils::QString2StdString(caption);
			if (cInst2.namesMap2Index.find(caption2) != cInst2.namesMap2Index.end()) {
				int len = NCMachineParametersC::namesLen[cInst2.namesMap2Index[caption2]];
				ret = new QLineEdit4Fdcs(len, parent);
			}
			else {
				ret = new QLineEdit4Fdcs(9, parent);
			}
		}
		return ret;
	};
	QVBoxLayout* layoutMain = new QVBoxLayout();
	layoutMain->setContentsMargins(0, 0, 0, 0);

	m_layout0 = new QHBoxLayout();
	m_layout0->addLayout(m_dynamicForm0);
	//m_layoutTop->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
	layoutMain->addLayout(m_layout0);

	m_layout1 = new QHBoxLayout();
	m_layout1->addLayout(m_dynamicForm);
	layoutMain->addLayout(m_layout1);

	layoutMain->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
	
	this->setLayout(layoutMain);

	m_dynamicForm0->createForm(QStringList() << "C.No");
	m_dynamicForm0->setRegexValidation("C.No", "\\d{1,3}");
	m_dynamicForm0->setContentsMargins(5, 5, 5, 5);

	m_dynamicForm->createForm(QStringList() << m_pNames, funcToCreateEditor);
	m_dynamicForm->setContentsMargins(5, 5, 5, 5);

	auto namesRegex = cInst.namesRegex;
	for (int i = 0; i < m_pNames.count(); ++i) {
		m_dynamicForm->setRegexValidation(m_pNames[i], QString::fromStdString(namesRegex[i]));
	}
	connect(dynamic_cast<QLineEdit*>(m_dynamicForm->getLineEdit("HP")), &QLineEdit::textChanged, [this](const QString& line) {
		QString v1 = this->m_dynamicForm->getValue("HP");
		if (v1.startsWith('3')) {
			QString v2 = this->m_dynamicForm->getValue("PP");
			if (v2.length() == 2) {
				v2[0] = '0';
			}
			this->m_dynamicForm->setValue("PP", v2);
		}
	});
	connect(dynamic_cast<QLineEdit*>(m_dynamicForm->getLineEdit("PP")), &QLineEdit::textChanged, [this](const QString& line) {
		QString v1 = this->m_dynamicForm->getValue("HP");
		if (v1.startsWith('3')) {
			QString v2 = this->m_dynamicForm->getValue("PP");
			if (v2.length() == 2) {
				v2[0] = '0';
			}
			this->m_dynamicForm->setValue("PP", v2);
		}
		});
	connect(dynamic_cast<QLineEdit*>(m_dynamicForm0->getLineEdit("C.No")), &QLineEdit::textEdited, [this](const QString& line) {
		if (line.length() == 3) {
			LoadFromParametersC(line.toInt());
		}
		//QString v1 = this->m_dynamicForm->getValue("C.No");
		//LoadFromParametersC(v1.toInt());
		//std::map<std::string, std::string> vs = NCMachineParametersC::instance().GetByCIndex(v1.toInt());
		//for (const auto& s : NCMachineParametersC::instance().names) {
		//	if (vs.find(s) != vs.end()) {
		//		QLineEdit* widget = m_dynamicForm->getLineEdit(QString::fromStdString(s));
		//		if (widget->text().isEmpty()) {
		//			widget->setText(QString::fromStdString(vs[s]));
		//		}
		//	}
		//}
	});
	

	//connect(dynamic_cast<QLineEdit*>(m_dynamicForm->getLineEdit("STEP")), &QLineEdit::editingFinished, [this]() {
	//	QString v1 = this->m_dynamicForm->getValue("STEP");
	//	if (v1.toDouble() == 0) {
	//		this->m_dynamicForm->setValue("LP", "0000");
	//	}
	//	});
}

FangDianCanShuSingleForm::~FangDianCanShuSingleForm()
{}

QString FangDianCanShuSingleForm::GetValuesAsCCode()
{
	QString cNo = m_dynamicForm0->getValue("C.No");
	if (cNo.isEmpty())
		return "";

	QString ret = "C" + QString::number(cNo.toInt()).rightJustified(3, '0') + " = ";

	NCMachineParametersC& pInst = NCMachineParametersC::instance();
	for (int i = 0; i < PARAMETERC_COLUMN_COUNT; i++) {
		//if (pInst.namesChar[i] == '=')
		//	continue;
		QString s = QString::fromStdString(pInst.names[i]);
		QString s2 = m_dynamicForm->getValue(s);

		ret.append(s2.isEmpty() ? "0" : s2);
		ret.append(' ');
	}
	ret.append('\n');
	return ret;
}

void FangDianCanShuSingleForm::SetValuesAsCCode(QString cCode)
{
	if (cCode.isEmpty())
		return;

	NCMachineParametersC& pInst = NCMachineParametersC::instance();

	QRegExp regex("C(\\d{3})\\s*=\\s*(.*)");
	int pos = 0;
	if ((pos = regex.indexIn(cCode, pos)) != -1) {

		QString match = regex.cap(1);

		QString ccommand = regex.cap(2).replace(";", "");
		QStringList parts = ccommand.trimmed().split(' ', Qt::SkipEmptyParts);
		if (PARAMETERC_COLUMN_COUNT != parts.count()) {
			LOG_WARNING("EDM: Invalid C Command When SetValuesAsCCode: " << EUtils::QString2StdString(ccommand));
			return;
		}
		for (int i = 0; i < parts.length(); ++i)
		{
			if (parts[i].isEmpty())
				continue;
			QRegExp rx(QString::fromStdString(pInst.namesRegex[i]));
			QString s = parts[i].rightJustified(NCMachineParametersC::namesLen[i], '0');

			if (rx.indexIn(s) == -1) {
				LOG_WARNING("EDM: Invalid C Command When SetValuesAsCCode: " << EUtils::QString2StdString(ccommand));
				continue;
			}
			m_dynamicForm->setValue(QString::fromStdString(pInst.names[i]), s);
		}
		m_dynamicForm0->setValue("C.No", match);
	}
}

void FangDianCanShuSingleForm::LoadFromParametersC(int cIndex)
{
	if (cIndex < -1 || cIndex >= PARAMETERC_ROW_COUNT) {
		return;
	}
	if (cIndex == -1) {
		cIndex = NCMachineParametersC::GetCurrentCIndex();
		if (cIndex != -1) {
			m_dynamicForm0->setValue("C.No", QString::number(cIndex).rightJustified(3, '0'));
		}
		for (const auto& str : NCMachineParametersC::instance().names) {
			m_dynamicForm->setValue(QString::fromStdString(str), QString::fromStdString(NCMachineParametersC::GetCurrentByName(str)));
		}
	}
	else {
		m_dynamicForm0->setValue("C.No", QString::number(cIndex).rightJustified(3, '0'));
		std::map<std::string, std::string> values = NCMachineParametersC::instance().GetByCIndex(cIndex);

		if (values.size() == PARAMETERC_COLUMN_COUNT) {
			for (const auto& str : NCMachineParametersC::instance().names) {
				if (values.find(str) != values.end()) {
					m_dynamicForm->setValue(QString::fromStdString(str), QString::fromStdString(values[str]));
				}
			}
		}
	}
}

void FangDianCanShuSingleForm::RunGCode(NCMachine* ncMachine)
{
	NCMachineParametersC::instance().SetByCIndex(m_dynamicForm0->getValue("C.No").toInt());
	for (QString str : m_pNames) {
		NCMachineParametersC::SetCurrentByName(str, m_dynamicForm->getValue(str));
	}
	if (m_dynamicForm->getValue("STEP").toDouble() == 0) {
		NCMachineParametersC::SetCurrentByName(QString("LP"), "0000");
	}

	ncMachine->enterSetPriority(0);
	NCMachineParametersC::ExecuteCmds(ncMachine);
	ncMachine->exitSetPriority();

	/*auto on = m_dynamicForm->getValue("ON").toDouble();
	auto off = m_dynamicForm->getValue("OFF").toDouble();
	auto ma = m_dynamicForm->getValue("MA").toDouble();
	auto ip = m_dynamicForm->getValue("IP").toDouble();
	auto sv = m_dynamicForm->getValue("SV").toDouble();
	auto up = m_dynamicForm->getValue("UP").toDouble();
	auto dw = m_dynamicForm->getValue("DN").toDouble();
	auto ln = m_dynamicForm->getValue("LN");
	auto lp = m_dynamicForm->getValue("LP");
	auto step = m_dynamicForm->getValue("STEP").toDouble();
	auto pl = m_dynamicForm->getValue("PL");
	auto v = m_dynamicForm->getValue("V");
	auto hp = m_dynamicForm->getValue("HP");
	auto pp = m_dynamicForm->getValue("PP");
	auto c = m_dynamicForm->getValue("C");
	auto s = m_dynamicForm->getValue("S");
	auto ls = m_dynamicForm->getValue("LS");
	auto al = m_dynamicForm->getValue("AL");
	auto js = m_dynamicForm->getValue("JS").toDouble();
	auto jm = m_dynamicForm->getValue("JM");
	auto sj = m_dynamicForm->getValue("SJ");
	auto fm = m_dynamicForm->getValue("FM");
	auto ds = m_dynamicForm->getValue("DS").toDouble();*/
}

FangDianCheShiForm::FangDianCheShiForm(QWidget* parent)
	: BaseChildWindow(parent)
{
	this->setObjectName("fangdiancheshiform");

	m_form = new FangDianCanShuSingleForm(this);
	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->addWidget(m_form);
}

FangDianCheShiForm::~FangDianCheShiForm()
{
}

void FangDianCheShiForm::RunGCode()
{
	SaveData();
	m_form->RunGCode(m_ncMachine);
}

void FangDianCheShiForm::LoadData(QString filePath)
{
	if (LoadFromFile) {
		if (filePath == NULL) {
			filePath = GetDataFilePath(this);
		}
		for (int i = 0; i < 2; ++i) {
			m_form->GetDynamicForm(i)->deserialize(SystemSettings::AppendDataFilePath(filePath, m_form->GetDynamicForm(i)->objectName()));
		}
	}
	else {
		m_form->LoadFromParametersC();
	}
}

void FangDianCheShiForm::SaveData(QString filePath)
{
	if (filePath == NULL) {
		filePath = GetDataFilePath(this);
	}
	for (int i = 0; i < 2; ++i) {
		m_form->GetDynamicForm(i)->serialize(SystemSettings::AppendDataFilePath(filePath, m_form->GetDynamicForm(i)->objectName()));
	}
}
