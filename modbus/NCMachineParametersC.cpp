#include <iostream>
#include <iomanip>
#include <string>
#include "NCMachineParametersC.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <cbang/log/Logger.h>
#include "utils/dataform.h"
#include "NCMachine.h"
#include "utils/nfile.h"
#include "NCMachineProperties.h"

NCMachineParametersC::NCMachineParametersC(Inaccessible)
{
	deserialize();

	for (int i = 0; i < PARAMETERC_COLUMN_COUNT; i++) {
		namesMap2Index.insert(std::make_pair(names[i], i));
	}

	for (int i = 0; i < PARAMETERC_COLUMN_COUNT; i++) {
		std::string pattern;
		//std::string r = namesRegex[i];
		//if (r[0] == '^')
		//	r = r.substr(1, r.length() - 2);
		//pattern = names[i] + "(" + r + ")";
		//re4Search[i] = QRegularExpression(QString::fromStdString(pattern));
		//re4Replace[i] = QString(namesChar[i]) + "\\1";

		pattern = "\\s" + names[i] + "(.[0-9.])";
		re4Search[i] = QRegularExpression(QString::fromStdString(pattern));
		re4Replace[i] = QString(" C%01 #<_%02>=\\1").arg(9001 + i, 4, 10, QChar('0')).arg(QString::fromStdString(names[i]).toUpper());

		std::string pattern2 = names[i] + "(H\\d{1,9})";
		re4Search2[i] = QRegularExpression(QString::fromStdString(pattern2));
	}
}

bool NCMachineParametersC::SetByCIndex(int cIndex)
{
	//cIndex--;
	if (cIndex < 0 || cIndex >= PARAMETERC_ROW_COUNT)
		return false;
	m_currentCIndex = cIndex;
	bool hasValue = false;
	for (int i = 0; i < PARAMETERC_COLUMN_COUNT; ++i) {
		std::string v = m_allValues[cIndex][i];
		if (v.empty())
			continue;
		hasValue = true;
		NCMachineParametersC::SetCurrentByName(names[i], v);
	}
	// 加工条件为C9**时，电阻开路，电感短路，Bit24=0，Bit25=0；
	if (cIndex >= 900) {
		PropertyObjects::getInstance()->propertyObjectFdhl->setdgsnw(0);
		PropertyObjects::getInstance()->propertyObjectFdhl->setdzsnw(0);
	}
	return hasValue;
}
std::map<std::string, std::string> NCMachineParametersC::GetByCIndex(int cIndex)
{
	//cIndex--;

	std::map<std::string, std::string> ret;
	if (cIndex < 0 || cIndex >= PARAMETERC_ROW_COUNT)
		return ret;
	for (int i = 0; i < PARAMETERC_COLUMN_COUNT; ++i) {
		std::string v = m_allValues[cIndex][i];
		ret.insert(std::make_pair(names[i], v));
	}
	return ret;
}
std::string NCMachineParametersC::GetNamesAsString()
{
	std::string namesStr;

	std::ostringstream oss;
	oss << "(      ";
	for (int i = 0; i < PARAMETERC_COLUMN_COUNT; i++) {
		int width = namesLen[i];
		if (width < 0) {
			// 如果宽度为负数，表示不限制宽度
			oss << std::setw(5) << std::setfill(' ') << std::left << names[i] << " ";
		}
		else {
			oss << std::setw(width) << std::setfill(' ') << std::left << names[i] << " ";
		}

		//namesStr += names[i];

		//if (i != PARAMETERC_COLUMN_COUNT - 1) {
		//	namesStr += " ";
		//}
	}
	oss << "      )\n";
	return oss.str();
}

//std::string NCMachineParametersC::GetCurrentAsString()
//{
//	std::ostringstream oss;
//	oss << "C" << std::setw(3) << std::setfill('0') << instance().m_currentCIndex << " = ";
//	std::string namesStr = oss.str();
//
//	for (int i = 0; i < PARAMETERC_COLUMN_COUNT; i++) {
//		namesStr += names[i];
//
//		if (i != PARAMETERC_COLUMN_COUNT - 1) {
//			namesStr += " ";
//		}
//	}
//	return namesStr;
//}

std::string NCMachineParametersC::FormatCCode(std::string cCode)
{
	std::istringstream iss(cCode);
	std::vector<std::string> values;
	std::string value;

	// 将输入字符串按空格分割成单独的值
	while (iss >> value) {
		values.push_back(value);
	}

	// 确保值的数量与 namesLen 的长度匹配
	if (values.size() != PARAMETERC_COLUMN_COUNT + 2) {
		//throw std::runtime_error("Input values count does not match namesLen length");
		return "";
	}

	std::ostringstream oss;
	for (size_t i = 0; i < values.size(); ++i) {
		if (i == 0) {
			oss << "C" << std::setw(3) << std::setfill('0') << std::stoi(values[i].substr(1)) << " = ";
		}
		else if (i == 1) {

		}
		else {
			int width = namesLen[i - 2];
			
			if (width < 0) {
				// 如果宽度为负数，表示不限制宽度
				oss << std::setw(5) << std::setfill(' ') << std::right << values[i] << " ";
			}
			else {
				if (width < (int)names[i - 2].length()) {
					width = names[i - 2].length();
				}

				// 根据宽度填充或截断值
				if (values[i].length() > static_cast<size_t>(width)) {
					oss << values[i].substr(0, width) << " ";
				}
				else {
					if (values[i] == "+" || values[i] == "-") {	// PL
						oss << std::setw(2) << std::setfill(' ') << std::right << values[i] << " ";
					}
					else {
						oss << std::setw(width) << std::setfill('0') << std::right << values[i] << " ";
					}
				}
			}
		}
	}
	oss << "\n";
	return oss.str();
}

void NCMachineParametersC::SetValue(int cIndex, int rowIdx, std::string v)
{
	assert(cIndex >= 0 && cIndex < PARAMETERC_ROW_COUNT);
	assert(rowIdx >= 0 && rowIdx < PARAMETERC_COLUMN_COUNT);
	m_allValues[cIndex][rowIdx] = v;
}

bool NCMachineParametersC::SetCurrentByName(QString name, QString value2)
{
	return SetCurrentByName(EUtils::QString2StdString(name), EUtils::QString2StdString(value2));
}

bool NCMachineParametersC::SetCurrentByName(std::string name, std::string value2)
{
	if (value2.empty())
		return false;
	int currentCIndex = NCMachineParametersC::instance().m_currentCIndex;
	NCMachineParametersC::instance().m_currentValues[name] = value2;

	QString value = QString::fromStdString(value2);

	NCMachineParametersC& pInst = NCMachineParametersC::instance();
	std::map<std::string, int> csmap = pInst.namesMap2Index;
	if (csmap.find(name) != csmap.end()) {
		QRegExp rx(QString::fromStdString(pInst.namesRegex[csmap[name]]));
		QString s = value.rightJustified(NCMachineParametersC::namesLen[csmap[name]], '0');

		if (rx.indexIn(s) == -1) {
			LOG_WARNING("EDM: Invalid C Command: " << name << " try set to " << value2 << ", not ok.");
			return false;
		}
	}

	if (name == "ON") {
		auto on = value.toInt();
		if (on >= 0 && on <= 4000) {
			PropertyObjects::getInstance()->propertyObjectFdjg->setjgmcmk(on);
		}
		else {
			PropertyObjects::getInstance()->propertyObjectFdjg->setjgmcmk(0);
		}
		//PropertyObjects::getInstance()->propertyObjectFdjg->setjgmcmk(abs(on) % 1000);
		//PropertyObjects::getInstance()->propertyObjectFdjg->setjgmcdnlsn(on >= 1000 ? 0 : 1);
	}
	else if (name == "OFF" || name == "MA") {
		std::string soff = GetCurrentByName("OFF");
		std::string sma = GetCurrentByName("MA");
		if (!soff.empty() && !sma.empty()) {
			auto off = std::stoi(soff);
			auto ma = std::stoi(sma);
			if (ma <= 9) {
				PropertyObjects::getInstance()->propertyObjectFdjg->setjgmcmj(off * (ma + 1));
			}
			else {
				PropertyObjects::getInstance()->propertyObjectFdjg->setjgmcmj(off);
			}
		}

		std::string soc = GetCurrentByName("OC");
		if (!soc.empty() && !sma.empty()) {
			auto oc = std::stoi(soc);
			auto ma = std::stoi(sma);
			if (ma < 1000) {
				int oc0 = oc % 100;
				if (oc0 == 0) // OC的后两位 = 0
				{
					PropertyObjects::getInstance()->propertyObjectFdjg->setjcycjcms(0);
				}
				else
				{
					PropertyObjects::getInstance()->propertyObjectFdjg->setjcycjcms(1);
				}
			}
			else if (ma < 2000)
			{
				PropertyObjects::getInstance()->propertyObjectFdjg->setjcycjcms(4);
			}
			else if (ma < 3000)
			{
				PropertyObjects::getInstance()->propertyObjectFdjg->setjcycjcms(5);
			}
			else if (ma < 4000)
			{
				PropertyObjects::getInstance()->propertyObjectFdjg->setjcycjcms(2);
			}
			else
			{
				PropertyObjects::getInstance()->propertyObjectFdjg->setjcycjcms(3);
			}
		}
	}
	else if (name == "IP") {
		auto ip = value.toInt();
		
		DataForm* dataForm4 = DataForms::getInstance()->getDataForm("xitongshezhi4");
		double slv3 = dataForm4->getValue("FJDL3").toDouble() * 10;
		double slv2 = dataForm4->getValue("FJDL2").toDouble() * 10;
		double slv1 = dataForm4->getValue("FJDL1").toDouble() * 10;

		PropertyObjects::getInstance()->propertyObjectFdhl->setSlv3(ip >= slv3);
		if (ip >= slv3) {
			ip = ip - slv3;
		}
		PropertyObjects::getInstance()->propertyObjectFdhl->setSlv2(ip >= slv2);
		if (ip >= slv2) {
			ip = ip - slv2;
		}
		PropertyObjects::getInstance()->propertyObjectFdhl->setSlv1(ip >= slv1);
		if (ip >= slv1) {
			ip = ip - slv1;
		}
		//if (slv3 == 0) {
		//	PropertyObjects::getInstance()->propertyObjectFdhl->setSlv3(0);
		//}
		//if (slv2 == 0) {
		//	PropertyObjects::getInstance()->propertyObjectFdhl->setSlv2(0);
		//}
		//if (slv1 == 0) {
		//	PropertyObjects::getInstance()->propertyObjectFdhl->setSlv1(0);
		//}

		PropertyObjects::getInstance()->propertyObjectFdhl->seta7(ip >= 160);
		if (ip >= 160) {
			ip = ip - 160;
		}
		PropertyObjects::getInstance()->propertyObjectFdhl->seta6(ip >= 80);
		if (ip >= 80) {
			ip = ip - 80;
		}
		PropertyObjects::getInstance()->propertyObjectFdhl->seta5(ip >= 40);
		if (ip >= 40) {
			ip = ip - 40;
		}
		PropertyObjects::getInstance()->propertyObjectFdhl->seta4(ip >= 20);
		if (ip >= 20) {
			ip = ip - 20;
		}
		PropertyObjects::getInstance()->propertyObjectFdhl->seta3(ip >= 10);
		if (ip >= 10) {
			ip = ip - 10;
		}
		PropertyObjects::getInstance()->propertyObjectFdhl->seta2(ip >= 5);
		if (ip >= 5) {
			ip = ip - 5;
		}
		PropertyObjects::getInstance()->propertyObjectFdhl->seta1(ip >= 3);
		if (ip >= 3) {
			ip = ip - 3;
		}
	}
	else if (name == "SV") {
		auto sv = value.toInt();
		if (sv < 0 || sv > 180)
		{
			// invalid. process in outer 
			return false;
		}
		PropertyObjects::getInstance()->propertyObjectFdjg->setfdjgcfdy(sv);
	}
	else if (name == "UP") {
		// 抬刀高度基数
		int x = DataForms::getInstance()->getDataForm("xitongshezhi3")->getValue("TDGDJS").toInt();
		int up = value.toInt();
		PropertyObjects::getInstance()->propertyObjectFdwc->settdgd(up * x);
	}
	else if (name == "DN") {
		// 放电时间基数
		int x = DataForms::getInstance()->getDataForm("xitongshezhi3")->getValue("FDSJJS").toInt();
		int dw = value.toInt();
		PropertyObjects::getInstance()->propertyObjectFdwc->setfdsj(dw * x);
	}
	else if (name == "LN") {
		QString ln = value;
		int len = ln.length();

		// 不要判断if (len > 0)，因为LN的值可能是10而不是010
		PropertyObjects::getInstance()->propertyObjectYd->setydpm(len > 0 ? ln[len - 1].digitValue() : 0);
		PropertyObjects::getInstance()->propertyObjectYd->setydfwcfms(len > 1 ? ln[len - 2].digitValue() : 0);
		PropertyObjects::getInstance()->propertyObjectYd->setydfwdzms(len > 2 ? ln[len - 3].digitValue() : 0);
	}
	else if (name == "LP" || name == "STEP") {
		//NCMachineParametersC::instance().m_currentValues["OLD_LP"] = NCMachineParametersC::instance().m_currentValues["LP"];
		std::string slp = GetCurrentByName("LP");
		std::string sstep = GetCurrentByName("STEP");
		if (!slp.empty() && !sstep.empty()) {
			double step = std::stod(sstep);
			if (step < 0) {
				step = 0;
				NCMachineParametersC::instance().m_currentValues["STEP"] = "0";
			}
			PropertyObjects::getInstance()->propertyObjectYd->setydbj((int)round(step * 1000));

			QString lp = QString::fromStdString(slp);
			if (step == 0) {
				lp = "0000";
			}
			int len = lp.length();

			PropertyObjects::getInstance()->propertyObjectYd->setd1xxydm(len > 0 ? lp[len - 1].digitValue() : 0);
			PropertyObjects::getInstance()->propertyObjectYd->setd2xxydm(len > 1 ? lp[len - 2].digitValue() : 0);
			PropertyObjects::getInstance()->propertyObjectYd->setd3xxydm(len > 2 ? lp[len - 3].digitValue() : 0);
			PropertyObjects::getInstance()->propertyObjectYd->setd4xxydm(len > 3 ? lp[len - 4].digitValue() : 0);
		}
	}
	else if (name == "PL") {
		QString pl = value;
		int len = pl.length();
		PropertyObjects::getInstance()->propertyObjectFdhl->setfjxjgw(len > 0 ? (pl[len - 1] == '+' ? 0 : 1) : 0);
	}
	else if (name == "V") {
		QString v = value;
		int len = v.length();
		{
			int x = len > 0 ? v[len - 1].digitValue() : 0;
			//PropertyObjects::getInstance()->propertyObjectFdhl->setLLV(x == 1);
			//PropertyObjects::getInstance()->propertyObjectFdhl->setHLV(x == 2);
			x = x >= 1 && x <= 4 ? x : 1;
			PropertyObjects::getInstance()->propertyObjectFdhl->setLV(x);
		}
	}
	else if (name == "HP") {

		QString hp = value;
		int len = hp.length();
		PropertyObjects::getInstance()->propertyObjectFdhl->setkgygzs(len > 0 ? hp[len - 1].digitValue() : 0);
		if (currentCIndex >= 900)
		{
			PropertyObjects::getInstance()->propertyObjectFdhl->setdgsnw(0);
			PropertyObjects::getInstance()->propertyObjectFdhl->setdzsnw(0);
		}
		else {
			int x = len > 1 ? hp[len - 2].digitValue() : 0;
			if (x <= 3) {
				PropertyObjects::getInstance()->propertyObjectFdhl->setdgsnw(1);
				PropertyObjects::getInstance()->propertyObjectFdhl->setdzsnw(1);
			}
			else if (x <= 7) {
				PropertyObjects::getInstance()->propertyObjectFdhl->setdgsnw(0);
				PropertyObjects::getInstance()->propertyObjectFdhl->setdzsnw(1);
			}
		}
		//PropertyObjects::getInstance()->propertyObjectFdhl->setfzdyxz(hp.length() > 0 ? hp[0].digitValue() : 0);
	}
	else if (name == "PP") {
		QString pp = value;
		int len = pp.length();
		// PP的个位数，寄存器编号83
		PropertyObjects::getInstance()->propertyObjectReg83->setppgw(len > 0 ? pp[len - 1].digitValue() : 0);
		//PropertyObjects::getInstance()->propertyObjectFdjg->setjgmcfzmcxz(len > 0 ? pp[len - 1].digitValue() : 1);
		// PP的十位对应Bit28~30
		PropertyObjects::getInstance()->propertyObjectFdhl->setfzdyxz(len > 1 ? pp[len - 2].digitValue() : 0);

		/*if (pp.length() > 1) {
			switch (pp[1].digitValue())
			{
			case 0:
				PropertyObjects::getInstance()->propertyObjectFdhl->setdgsnw(1);
				PropertyObjects::getInstance()->propertyObjectFdhl->setdzsnw(0);
				break;
			case 3:
				PropertyObjects::getInstance()->propertyObjectFdhl->setdgsnw(0);
				PropertyObjects::getInstance()->propertyObjectFdhl->setdzsnw(1);
				break;
			case 2:
			default:
				PropertyObjects::getInstance()->propertyObjectFdhl->setdgsnw(1);
				PropertyObjects::getInstance()->propertyObjectFdhl->setdzsnw(1);
				break;
			}
		}*/
	}
	else if (name == "C") {
		QString c = value;
		int len = c.length();
		PropertyObjects::getInstance()->propertyObjectFdhl->setfddrxz(len > 0 ? c[len - 1].digitValue() : 0);
	}
	else if (name == "S") {
		QString s = value;
		int len = s.length();
		PropertyObjects::getInstance()->propertyObjectFdjg->setfdcfsd(len > 0 ? s[len - 1].digitValue() : 0);
		PropertyObjects::getInstance()->propertyObjectFdjg->setfdcftzsd(len > 1 ? s[len - 2].digitValue() : 0);
	}
	else if (name == "LS" || name == "L") {
		// 摇动速度基数
		int x = DataForms::getInstance()->getDataForm("xitongshezhi3")->getValue("YDSDJS").toInt();
		QString ls = value;
		int len = ls.length();

		PropertyObjects::getInstance()->propertyObjectYd->setzdydsd(len > 0 ? ls[len - 1].digitValue() * x : 0);
		PropertyObjects::getInstance()->propertyObjectYd->setydfx(len > 1 ? ls[len - 2].digitValue() : 2);
	}
	else if (name == "AL") {
		auto al = value.toInt();
		//int len = al.length();
		//PropertyObjects::getInstance()->propertyObjectFdjg->setjcycsjyz(len > 0 ? al[len - 1].digitValue() : 0);
		//PropertyObjects::getInstance()->propertyObjectFdjg->setjgmcdyycjcsn(len > 1 ? al[len - 2] == '1' : true);
		PropertyObjects::getInstance()->propertyObjectFdjg->setjgmcdyycjcsn(al);
	}
	else if (name == "JS") {
		QString js = value;
		
		{
			int len = js.length();
			int x = len > 2 ? js[len - 3].digitValue() : 0;
			PropertyObjects::getInstance()->propertyObjectYd->setdmjtdmskgl(x);
			if (x == 0) {
				PropertyObjects::getInstance()->propertyObjectYd->settdqmshtjl(0);
				PropertyObjects::getInstance()->propertyObjectYd->settdqmshtsd(1);
			}
			else if (x == 1) {
				DataForm* dataForm3 = DataForms::getInstance()->getDataForm("xitongshezhi3");

				int v;
				auto s = SystemSettings::instance().GetValue("LEJL");
				if (!s.isEmpty()) {
					v = (int)(s.toDouble() * 1000);
				}
				else {
					v = dataForm3->getValue("MTDGD").toInt();
				}
				PropertyObjects::getInstance()->propertyObjectYd->settdqmshtjl(v);

				s = SystemSettings::instance().GetValue("LEJS");
				if (!s.isEmpty()) {
					v = (int)(s.toDouble() * 1000);
				}
				else {
					v = dataForm3->getValue("MTDSD").toInt();
				}
				PropertyObjects::getInstance()->propertyObjectYd->settdqmshtsd(v);
			}
		}

		// 抬刀速度基数
		QString jm = QString::fromStdString(GetCurrentByName("JM"));
		{
			int len = jm.length();
			int jm2 = len > 1 ? jm[len - 2].digitValue() : 0;
			if (jm2 == 0) {
				QString jsValue = js;
				int jsInt = jsValue.right(2).toInt();
				int tdsdjsValue = DataForms::getInstance()->getDataForm("xitongshezhi3")->getValue("TDSDJS").toInt();
				PropertyObjects::getInstance()->propertyObjectFdwc->settdsd(jsInt * tdsdjsValue);
			}
			else if (jm2 == 1) {
				QString jsValue = js;
				int jsInt = jsValue.right(2).toInt();
				int tdsdjsValue = DataForms::getInstance()->getDataForm("xitongshezhi3")->getValue("TDSDJS").toInt();
				PropertyObjects::getInstance()->propertyObjectFdwc->settdsd(jsInt * tdsdjsValue / 10);
			}
		}
	}
	else if (name == "JM") {
		QString jm = value;
		int len = jm.length();

		//PropertyObjects::getInstance()->propertyObjectFdwc->setectdgdbs(len > 0 ? jm[len - 1].digitValue() : 0);
		//PropertyObjects::getInstance()->propertyObjectFdwc->setectdxydtdcs(len > 1 ? jm[len - 2].digitValue() : 0);
		PropertyObjects::getInstance()->propertyObjectFdwc->settdfx(len > 0 ? jm[len - 1].digitValue() : 9);

		// 抬刀速度基数
		{
			int jm2 = len > 1 ? jm[len - 2].digitValue() : 0;
			if (jm2 == 0) {
				QString jsValue = QString::fromStdString(GetCurrentByName("JS"));
				int jsInt = jsValue.right(2).toInt();
				int tdsdjsValue = DataForms::getInstance()->getDataForm("xitongshezhi3")->getValue("TDSDJS").toInt();
				PropertyObjects::getInstance()->propertyObjectFdwc->settdsd(jsInt * tdsdjsValue);
			}
			else if (jm2 == 1) {
				QString jsValue = QString::fromStdString(GetCurrentByName("JS"));
				int jsInt = jsValue.right(2).toInt();
				int tdsdjsValue = DataForms::getInstance()->getDataForm("xitongshezhi3")->getValue("TDSDJS").toInt();
				PropertyObjects::getInstance()->propertyObjectFdwc->settdsd(jsInt * tdsdjsValue / 10);
			}
		}
	}
	else if (name == "SJ") {
		//// 回中心抬刀高度基数
		//int x = DataForms::getInstance()->getDataForm("xitongshezhi3")->getValue("HZXTDGDJS").toInt();
		//// 回中心抬刀速度基数
		//int x2 = DataForms::getInstance()->getDataForm("xitongshezhi3")->getValue("HZXTDSDJS").toInt();
		//QString sj = value;
		//int len = sj.length();
		//	PropertyObjects::getInstance()->propertyObjectYd->sethzxtdsd(len > 1 ? (sj[len - 2].digitValue() * 10 + sj[len - 1].digitValue()) * x2 : 0);
		//	PropertyObjects::getInstance()->propertyObjectYd->sethzxtdgd(len > 2 ? sj[len - 3].digitValue() * x : 0);
	}
	else if (name == "FM") {
		QString fm = value;
		int len = fm.length();
		PropertyObjects::getInstance()->propertyObjectYd->setsdjcxfs(len > 1 ? (fm[len - 2].digitValue() * 10 + fm[len - 1].digitValue()) : 8);
		PropertyObjects::getInstance()->propertyObjectFdwc->setfdwcms(len > 2 ? fm[len - 3].digitValue() : 0);
	}
	else if (name == "DS") {
		auto ds = value.toInt();
		PropertyObjects::getInstance()->propertyObjectFdwc->setfddssj(ds);
	}
	else if (name == "OC") {
		auto oc = value.toInt();
		PropertyObjects::getInstance()->propertyObjectFdjg->setjgmcdnlsn(oc);

		std::string sma = GetCurrentByName("MA");
		if (!sma.empty()) {
			auto ma = std::stoi(sma);
			if (ma < 1000) {
				int oc0 = oc % 10;
				if (oc0 <= 5)
				{
					PropertyObjects::getInstance()->propertyObjectFdjg->setjcycjcms(0);
				}
				else
				{
					PropertyObjects::getInstance()->propertyObjectFdjg->setjcycjcms(1);
				}
			}
			else if (ma < 2000)
			{
				PropertyObjects::getInstance()->propertyObjectFdjg->setjcycjcms(4);
			}
			else if (ma < 3000)
			{
				PropertyObjects::getInstance()->propertyObjectFdjg->setjcycjcms(5);
			}
			else if (ma < 4000)
			{
				PropertyObjects::getInstance()->propertyObjectFdjg->setjcycjcms(2);
			}
			else
			{
				PropertyObjects::getInstance()->propertyObjectFdjg->setjcycjcms(3);
			}
		}
	}
	else if (name == "LD") {
		auto ld = value.toInt();
		PropertyObjects::getInstance()->propertyObjectReg86->setv(ld);
	}
	else {
		LOG_WARNING("EDM: Invalid C Name " << name);
		return false;
	}

	if (name == "PP" || name == "HP") {
		if (PropertyObjects::getInstance()->propertyObjectFdhl->fzdyxz() == 3) {
			PropertyObjects::getInstance()->propertyObjectFdjg->setjgmcfzmcxz(0);
		}
	}

	return true;
}

std::string NCMachineParametersC::GetCurrentByName(std::string name)
{
	if (NCMachineParametersC::instance().m_currentValues.find(name) != NCMachineParametersC::instance().m_currentValues.end())
	{
		return NCMachineParametersC::instance().m_currentValues[name];
	}
	else 
	{
		return "";
	}
}

void NCMachineParametersC::ExecuteCmds(NCMachine* ncMachine)
{
	PropertyObjects::getInstance()->propertyObjectFdhl->ExecuteCmds(ncMachine);
	PropertyObjects::getInstance()->propertyObjectFdjg->ExecuteCmds(ncMachine);
	PropertyObjects::getInstance()->propertyObjectFdwc->ExecuteCmds(ncMachine);
	PropertyObjects::getInstance()->propertyObjectYd->ExecuteCmds(ncMachine);
	PropertyObjects::getInstance()->propertyObjectReg83->ExecuteCmds(ncMachine);
	PropertyObjects::getInstance()->propertyObjectReg86->ExecuteCmds(ncMachine);
}

bool NCMachineParametersC::deserialize()
{
	try {
		QString filePathName = SystemSettings::GetDataFilePath("fangdiancanshu_global");
		
		if (QFile::exists(filePathName)) {
			QFile file(filePathName);
			file.open(QIODevice::ReadOnly);
			QString json = file.readAll();
			file.close();

			QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
			QJsonArray data = doc.array();

			for (int row = 0; row < data.size(); ++row) {
				if (row >= PARAMETERC_ROW_COUNT)
					break;
				QJsonArray rowData = data[row].toArray();

				for (int col = 0; col < rowData.size(); ++col) {
					if (col >= PARAMETERC_COLUMN_COUNT)
						break;
					m_allValues[row][col] = EUtils::QString2StdString(rowData[col].toString());
				}
			}
		}
	}
	catch (...) {
		return false;
	}
	return true;
}