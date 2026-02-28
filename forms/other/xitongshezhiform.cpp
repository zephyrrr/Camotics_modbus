#include "xitongshezhiform.h"
#include <QGroupBox>
#include "boollineedit.h"
#include "../modbus/NCMachine.h"
#include "../modbus/NCMachineProperties.h"
#include "widgets/qlineedit4axisvalue.h"

#define DYNAMIC_FORMS_COUNT 5

XiTongSheZhiForm::XiTongSheZhiForm(QWidget *parent)
	: BaseChildWindow(parent)
{
	ui.setupUi(this);

	// create a vertical layout. in each row, there is a group box
	// the group box contains a form layout
	//QVBoxLayout* verticalLayout = new QVBoxLayout();
	//this->setLayout(verticalLayout);
	QHBoxLayout* verticalLayout = new QHBoxLayout(this);
	this->setLayout(verticalLayout);

	QTabWidget* tabWidget = new QTabWidget(this);
	verticalLayout->setContentsMargins(20, 20, 0, 0);
	verticalLayout->addWidget(tabWidget);

	for (int i = 0; i < DYNAMIC_FORMS_COUNT; i++) {
		QGroupBox* groupBox = new QGroupBox(this);
		//verticalLayout->addWidget(groupBox);
		tabWidget->addTab(groupBox, QString("%1").arg(i + 1));
		
		if (i == 0) {
			groupBox->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
		}
		else {
			groupBox->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
		}
		groupBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

		DynamicForm* dynamicForm = new DynamicForm(groupBox);
		m_dynamicForms.push_back(dynamicForm);
		groupBox->setLayout(dynamicForm);
	}
	
	std::function<ILineEdit* (QString, QWidget*)> funcToCreateEditor = [](QString caption, QWidget* parent) {
		ILineEdit* r;
		if (caption == "FMQKG" || caption == "ZDKQYB2" || caption == "SDM04SX" || caption == "GDMMWZJFH") {
			r = new BoolLineEdit(parent);
		}
		else {
			QLineEdit4Keyboard* r2 = new QLineEdit4Keyboard(parent);
			r2->setAllowNull(false);
			r = r2;
		}
		return r;
		};

	{
		DynamicForm* dynamicForm = m_dynamicForms[0];
		dynamicForm->setObjectName("xitongshezhi");
		dynamicForm->isHorizontalLayout = false;

		dynamicForm->createForm(QStringList()
			<< "HLFX_X" << "HLFX_Y" << "HLFX_Z" 
			<< "JXYD_X+" << "JXYD_X-" << "JXYD_Y+" << "JXYD_Y-" << "JXYD_Z+" << "JXYD_Z-"
			<< "AdGain" << "AdCount0V" << "KLM331" << "K201", funcToCreateEditor);

		dynamicForm->setValue("HLFX_X", "+");
		dynamicForm->setRegexValidation("HLFX_X", "^[\\+|\\-]$");
		dynamicForm->setLabelCaption("HLFX_X", QStringLiteral("X轴回零方向"));
		dynamicForm->setValue("HLFX_Y", "+");
		dynamicForm->setRegexValidation("HLFX_Y", "^[\\+|\\-]$");
		dynamicForm->setLabelCaption("HLFX_Y", QStringLiteral("Y轴回零方向"));
		dynamicForm->setValue("HLFX_Z", "+");
		dynamicForm->setRegexValidation("HLFX_Z", "^[\\+|\\-]$");
		dynamicForm->setLabelCaption("HLFX_Z", QStringLiteral("Z轴回零方向"));

		dynamicForm->setValue("JXYD_X+", "9999999");
		dynamicForm->setRegexValidation("JXYD_X+", "^[+-]?\\d{1,9}$");
		dynamicForm->setLabelCaption("JXYD_X+", QStringLiteral("X+极限位置"));
		dynamicForm->setValue("JXYD_X-", "-9999999");
		dynamicForm->setRegexValidation("JXYD_X-", "^[+-]?\\d{1,9}$");
		dynamicForm->setLabelCaption("JXYD_X-", QStringLiteral("X-极限位置"));
		dynamicForm->setValue("JXYD_Y+", "9999999");
		dynamicForm->setRegexValidation("JXYD_Y+", "^[+-]?\\d{1,9}$");
		dynamicForm->setLabelCaption("JXYD_Y+", QStringLiteral("Y+极限位置"));
		dynamicForm->setValue("JXYD_Y-", "-9999999");
		dynamicForm->setRegexValidation("JXYD_Y-", "^[+-]?\\d{1,9}$");
		dynamicForm->setLabelCaption("JXYD_Y-", QStringLiteral("Y-极限位置"));
		dynamicForm->setValue("JXYD_Z+", "9999999");
		dynamicForm->setRegexValidation("JXYD_Z+", "^[+-]?\\d{1,9}$");
		dynamicForm->setLabelCaption("JXYD_Z+", QStringLiteral("Z+极限位置"));
		dynamicForm->setValue("JXYD_Z-", "-9999999");
		dynamicForm->setRegexValidation("JXYD_Z-", "^[+-]?\\d{1,9}$");
		dynamicForm->setLabelCaption("JXYD_Z-", QStringLiteral("Z-极限位置"));

		dynamicForm->setValue("AdGain", "1.0");
		dynamicForm->setValue("AdCount0V", "2023");
		dynamicForm->setValue("KLM331", "1");
		dynamicForm->setValue("K201", "0.5");

		dynamicForm->setRegexValidation("AdGain", "^\\d{1,9}\\.\\d{1,8}$");
		dynamicForm->setRegexValidation("AdCount0V", "^\\d{1,9}$");
		dynamicForm->setRegexValidation("KLM331", "^\\d{1,9}\\.\\d{1,8}$");
		dynamicForm->setRegexValidation("K201", "^\\d{1,9}\\.\\d{1,8}$");
	}

	{
		DynamicForm* dynamicForm = m_dynamicForms[1];
		dynamicForm->setObjectName("xitongshezhi2");
		dynamicForm->isHorizontalLayout = false;

		
		dynamicForm->createForm(QStringList() 
			<< "SPEED_JOG_V_X" << "SPEED_JOG_T_X"
			<< "SPEED_JOG_V_Y" << "SPEED_JOG_T_Y"
			<< "SPEED_JOG_V_Z" << "SPEED_JOG_T_Z"
			<< "SPEED_JOG_V_U" << "SPEED_JOG_T_U"
			<< "SPEED_LOC_V" << "SPEED_LOC_T"
			<< "SPEED_CMV_V" << "SPEED_CMV_T"
			<< "SPEED_CMV_TOZIG_V" << "SPEED_CMV_TOZIG_T", funcToCreateEditor);

		dynamicForm->setValue("SPEED_JOG_V_X", "2400");
		dynamicForm->setRegexValidation("SPEED_JOG_V_X", "^\\d+$");
		dynamicForm->setLabelCaption("SPEED_JOG_V_X", QStringLiteral("X轴最大JOG速度"));
		dynamicForm->setValue("SPEED_JOG_T_X", "100");
		dynamicForm->setRegexValidation("SPEED_JOG_T_X", "^\\d+$");
		dynamicForm->setLabelCaption("SPEED_JOG_T_X", QStringLiteral("X轴JOG加速时间"));
		dynamicForm->setValue("SPEED_JOG_V_Y", "2400");
		dynamicForm->setRegexValidation("SPEED_JOG_V_Y", "^\\d+$");
		dynamicForm->setLabelCaption("SPEED_JOG_V_Y", QStringLiteral("Y轴最大JOG速度"));
		dynamicForm->setValue("SPEED_JOG_T_Y", "100");
		dynamicForm->setRegexValidation("SPEED_JOG_T_Y", "^\\d+$");
		dynamicForm->setLabelCaption("SPEED_JOG_T_Y", QStringLiteral("Y轴JOG加速时间"));
		dynamicForm->setValue("SPEED_JOG_V_Z", "2400");
		dynamicForm->setRegexValidation("SPEED_JOG_V_Z", "^\\d+$");
		dynamicForm->setLabelCaption("SPEED_JOG_V_Z", QStringLiteral("Z轴最大JOG速度"));
		dynamicForm->setValue("SPEED_JOG_T_Z", "100");
		dynamicForm->setRegexValidation("SPEED_JOG_T_Z", "^\\d+$");
		dynamicForm->setLabelCaption("SPEED_JOG_T_Z", QStringLiteral("Z轴JOG加速时间"));
		dynamicForm->setValue("SPEED_JOG_V_U", "2400");
		dynamicForm->setRegexValidation("SPEED_JOG_V_U", "^\\d+$");
		dynamicForm->setLabelCaption("SPEED_JOG_V_U", QStringLiteral("U轴最大JOG速度"));
		dynamicForm->setValue("SPEED_JOG_T_U", "100");
		dynamicForm->setRegexValidation("SPEED_JOG_T_U", "^\\d+$");
		dynamicForm->setLabelCaption("SPEED_JOG_T_U", QStringLiteral("U轴JOG加速时间"));

		dynamicForm->setValue("SPEED_LOC_V", "1800");
		dynamicForm->setRegexValidation("SPEED_LOC_V", "^\\d+$");
		dynamicForm->setLabelCaption("SPEED_LOC_V", QStringLiteral("LOC速度"));
		dynamicForm->setValue("SPEED_LOC_T", "200");
		dynamicForm->setRegexValidation("SPEED_LOC_T", "^\\d+$");
		dynamicForm->setLabelCaption("SPEED_LOC_T", QStringLiteral("LOC加速时间"));

		dynamicForm->setValue("SPEED_CMV_V", "12");
		dynamicForm->setRegexValidation("SPEED_CMV_V", "^\\d+$");
		dynamicForm->setLabelCaption("SPEED_CMV_V", QStringLiteral("对刀移动速度"));
		dynamicForm->setValue("SPEED_CMV_T", "200");
		dynamicForm->setRegexValidation("SPEED_CMV_T", "^\\d+$");
		dynamicForm->setLabelCaption("SPEED_CMV_T", QStringLiteral("对刀加速时间"));

		dynamicForm->setValue("SPEED_CMV_TOZIG_V", "12");
		dynamicForm->setRegexValidation("SPEED_CMV_TOZIG_V", "^\\d+$");
		dynamicForm->setLabelCaption("SPEED_CMV_TOZIG_V", QStringLiteral("回参考点移动速度"));
		dynamicForm->setValue("SPEED_CMV_TOZIG_T", "200");
		dynamicForm->setRegexValidation("SPEED_CMV_TOZIG_T", "^\\d+$");
		dynamicForm->setLabelCaption("SPEED_CMV_TOZIG_T", QStringLiteral("回参考点加速时间"));
	}

	{
		DynamicForm* dynamicForm = m_dynamicForms[2];
		dynamicForm->setObjectName("xitongshezhi3");
		dynamicForm->isHorizontalLayout = false;

		dynamicForm->createForm(QStringList() 
			<< "DDCS" << "DDGCZDHTJL" << "TDGDJS" << "TDSDJS" << "FDSJJS" << "YDSDJS" 
			<< "HZXTDGD" << "HZXTDSD" << "HZXTDJSSJ"
			<< "JGJSPDFGS" << "MTDGD" << "MTDSD"
			<< "TCQHTJL" << "SDM04SX" << "ZTDJSSJ", funcToCreateEditor);

		dynamicForm->setValue("DDCS", "4");
		dynamicForm->setRegexValidation("DDCS", "^\\d+$");
		dynamicForm->setLabelCaption("DDCS", QStringLiteral("对刀次数"));

		dynamicForm->setValue("DDGCZDHTJL", "300");
		dynamicForm->setRegexValidation("DDGCZDHTJL", "^\\d+$");
		dynamicForm->setLabelCaption("DDGCZDHTJL", QStringLiteral("对刀过程中回退距离"));

		dynamicForm->setValue("TDGDJS", "500");
		dynamicForm->setRegexValidation("TDGDJS", "^\\d+$");
		dynamicForm->setLabelCaption("TDGDJS", QStringLiteral("抬刀高度基数"));

		dynamicForm->setValue("TDSDJS", "40");
		dynamicForm->setRegexValidation("TDSDJS", "^\\d+$");
		dynamicForm->setLabelCaption("TDSDJS", QStringLiteral("抬刀速度基数"));

		dynamicForm->setValue("FDSJJS", "40");
		dynamicForm->setRegexValidation("FDSJJS", "^\\d+$");
		dynamicForm->setLabelCaption("FDSJJS", QStringLiteral("放电时间基数"));

		dynamicForm->setValue("YDSDJS", "5");
		dynamicForm->setRegexValidation("YDSDJS", "^\\d+$");
		dynamicForm->setLabelCaption("YDSDJS", QStringLiteral("摇动速度基数"));

		dynamicForm->setValue("HZXTDGD", "0");
		dynamicForm->setRegexValidation("HZXTDGD", "^\\d+$");
		dynamicForm->setLabelCaption("HZXTDGD", QStringLiteral("回中心抬刀高度"));

		dynamicForm->setValue("HZXTDSD", "60");
		dynamicForm->setRegexValidation("HZXTDSD", "^\\d+$");
		dynamicForm->setLabelCaption("HZXTDSD", QStringLiteral("回中心抬刀速度"));

		dynamicForm->setValue("HZXTDJSSJ", "5");
		dynamicForm->setRegexValidation("HZXTDJSSJ", "^\\d+$");
		dynamicForm->setLabelCaption("HZXTDJSSJ", QStringLiteral("回中心抬刀加速时间"));

		dynamicForm->setValue("JGJSPDFGS", "8");
		dynamicForm->setRegexValidation("JGJSPDFGS", "^\\d+$");
		dynamicForm->setLabelCaption("JGJSPDFGS", QStringLiteral("加工结束判断分割数"));

		dynamicForm->setValue("MTDGD", "50");
		dynamicForm->setRegexValidation("MTDGD", "^\\d+$");
		dynamicForm->setLabelCaption("MTDGD", QStringLiteral("慢抬刀高度(um)"));

		dynamicForm->setValue("MTDSD", "10");
		dynamicForm->setRegexValidation("MTDSD", "^\\d+$");
		dynamicForm->setLabelCaption("MTDSD", QStringLiteral("慢抬刀速度"));

		dynamicForm->setValue("TCQHTJL", "1000");
		dynamicForm->setRegexValidation("TCQHTJL", "^\\d+$");
		dynamicForm->setLabelCaption("TCQHTJL", QStringLiteral("退出前回退距离"));
		
		//dynamicForm->setValue("FDDWHTJL", "0");
		//dynamicForm->setRegexValidation("FDDWHTJL", "^\\d+$");
		//dynamicForm->setLabelCaption("FDDWHTJL", QStringLiteral("放电定位回退距离")); 
		  
		dynamicForm->setValue("SDM04SX", "True");
		dynamicForm->setLabelCaption("SDM04SX", QStringLiteral("手动M04生效"));

		dynamicForm->setValue("ZTDJSSJ", "50");
		dynamicForm->setRegexValidation("ZTDJSSJ", "^\\d+$");
		dynamicForm->setLabelCaption("ZTDJSSJ", QStringLiteral("主抬刀加速时间"));
	}

	{
		DynamicForm* dynamicForm = m_dynamicForms[3];
		dynamicForm->setObjectName("xitongshezhi4");
		dynamicForm->isHorizontalLayout = false;

		dynamicForm->createForm(QStringList()
			<< "FJDL1" << "FJDL2" << "FJDL3" << "FSYCGBSJ" << "ZTHQSDSD", funcToCreateEditor);

		dynamicForm->setValue("FJDL1", "32");
		dynamicForm->setRegexValidation("FJDL1", "^\\d+$");
		dynamicForm->setLabelCaption("FJDL1", QStringLiteral("附加电流1"));

		dynamicForm->setValue("FJDL2", "32");
		dynamicForm->setRegexValidation("FJDL2", "^\\d+$");
		dynamicForm->setLabelCaption("FJDL2", QStringLiteral("附加电流2"));

		dynamicForm->setValue("FJDL3", "64");
		dynamicForm->setRegexValidation("FJDL3", "^\\d+$");
		dynamicForm->setLabelCaption("FJDL3", QStringLiteral("附加电流3"));

		dynamicForm->setValue("FSYCGBSJ", "50");
		dynamicForm->setRegexValidation("FSYCGBSJ", "^\\d+$");
		dynamicForm->setLabelCaption("FSYCGBSJ", QStringLiteral("风扇延迟关闭时间"));

		dynamicForm->setValue("ZTHQSDSD", "500");
		dynamicForm->setRegexValidation("ZTHQSDSD", "^\\d+$");
		dynamicForm->setLabelCaption("ZTHQSDSD", QStringLiteral("暂停回起始点速度"));
	}

	{
		DynamicForm* dynamicForm = m_dynamicForms[4];
		dynamicForm->setObjectName("xitongshezhi5");
		dynamicForm->isHorizontalLayout = false;

		int n = 10;
		int v1[] = { 25, 20, 15, 15, 15, 15, 15, 15, 15, 01 };
		double v2[] = { 1.5, 1.4, 1.3, 1.15, 1.0, 0.83, 0.66, 0.5, 0.4, 0.25 };
		QStringList list;
		for(int i=0; i<n ; i++) {
			QString s = QString("KZSDBS%1").arg(i + 1);
			list << s;
		}
		for (int i = 0; i < n; i++) {
			QString s = QString("SFSDBS%1").arg(i + 1);
			list << s;
		}
		list << "FMQKG" << "GDMMWZJFH" << "ZDKQYB2" << "FDWCHMJSJ" << "SFHFJL" << "AJCFHJL";
		dynamicForm->createForm(list, funcToCreateEditor);

		for (int i = 0; i < n; i++) {
			QString s = QString("KZSDBS%1").arg(i + 1);
			dynamicForm->setValue(s, QString::number(v1[i]));
			dynamicForm->setRegexValidation(s, "^\\d+$");
			dynamicForm->setLabelCaption(s, QStringLiteral("空载速度倍数%1").arg(i + 1));
		}
		for (int i = 0; i < n; i++) {
			QString s = QString("SFSDBS%1").arg(i + 1);
			dynamicForm->setValue(s, QString::number(v2[i]));
			dynamicForm->setRegexValidation(s, "^\\d+(\\.\\d+)$");
			dynamicForm->setLabelCaption(s, QStringLiteral("伺服速度倍数%1").arg(i + 1));
		}

		dynamicForm->setValue("FMQKG", "True");
		dynamicForm->setLabelCaption("FMQKG", QStringLiteral("蜂鸣器开关"));
		dynamicForm->setValue("GDMMWZJFH", "False");
		dynamicForm->setLabelCaption("GDMMWZJFH", QStringLiteral("G代码末尾加分号"));
		
		dynamicForm->setValue("ZDKQYB2", "True");
		dynamicForm->setLabelCaption("ZDKQYB2", QStringLiteral("加工时自动开启油泵2"));

		dynamicForm->setValue("FDWCHMJSJ", "2");
		dynamicForm->setLabelCaption("FDWCHMJSJ", QStringLiteral("放电完成后鸣叫时间"));

		dynamicForm->setValue("SFHFJL", "20");
		dynamicForm->setLabelCaption("SFHFJL", QStringLiteral("伺服恢复距离"));

		dynamicForm->setValue("AJCFHJL", "20");
		dynamicForm->setLabelCaption("AJCFHJL", QStringLiteral("AJC返回距离"));
	}

	LoadData();
}

XiTongSheZhiForm::~XiTongSheZhiForm()
{
	for (int i = 0; i < DYNAMIC_FORMS_COUNT; i++) {
		delete m_dynamicForms[i];
	}
}

void XiTongSheZhiForm::SaveData(QString filePath)
{
	if (filePath != NULL) {
		throw std::invalid_argument("filePath must be NULL");
	}

	for (int i = 0; i < DYNAMIC_FORMS_COUNT; i++) {
		m_dynamicForms[i]->serialize(GetDataFilePath(m_dynamicForms[i]));
	}
}

void XiTongSheZhiForm::LoadData(QString filePath)
{
	if (filePath != NULL) {
		throw std::invalid_argument("filePath must be NULL");
	}

	for (int i = 0; i < DYNAMIC_FORMS_COUNT; i++) {
		m_dynamicForms[i]->deserialize(GetDataFilePath(m_dynamicForms[i]));
	}
}

void XiTongSheZhiForm::RunGCode()
{
	SaveData();

	DataForms::getInstance()->UnloadData();
	DataForms::getInstance()->LoadData();

	PropertyObjects::getInstance()->LoadData();

	DynamicForm* dynamicForm = m_dynamicForms[0];
	auto adGain = dynamicForm->getValue("AdGain").toDouble();
	auto adCount = dynamicForm->getValue("AdCount0V").toInt();
	auto klm331 = dynamicForm->getValue("KLM331").toDouble();
	auto k201 = dynamicForm->getValue("K201").toDouble();

	PropertyObjects::getInstance()->propertyObjectADC->setadczyxs(adGain);
	PropertyObjects::getInstance()->propertyObjectADC->setadcjzsj(adCount);
	PropertyObjects::getInstance()->propertyObjectADC->setsfdysc(klm331);
	PropertyObjects::getInstance()->propertyObjectADC->setgobl(k201);

	m_ncMachine->enterSetPriority(0);
	// to generate multuthread bug in mutex lock
	//EUtils::sleep(5000);
	PropertyObjects::getInstance()->propertyObjectADC->ExecuteCmds(m_ncMachine);
	
	PropertyObjects::getInstance()->propertyObjectFdhl->ExecuteCmds(m_ncMachine);
	PropertyObjects::getInstance()->propertyObjectFdjg->ExecuteCmds(m_ncMachine);
	PropertyObjects::getInstance()->propertyObjectFdwc->ExecuteCmds(m_ncMachine);
	PropertyObjects::getInstance()->propertyObjectYd->ExecuteCmds(m_ncMachine);


	PropertyObjects::getInstance()->propertyObjectReg78->ExecuteCmds(m_ncMachine);
	PropertyObjects::getInstance()->propertyObjectReg81->ExecuteCmds(m_ncMachine);

	PropertyObjects::getInstance()->propertyObjectReg84->ExecuteCmds(m_ncMachine);
	PropertyObjects::getInstance()->propertyObjectReg85->ExecuteCmds(m_ncMachine);

	m_ncMachine->exitSetPriority();
}
