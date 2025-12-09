#include <cmath>
#include <QDir>
#include <QSettings>
#include "NCMachineProperties.h"
#include "NCMachineDefines.h"
#include "NCMachine.h"
#include "utils/dataform.h"
#include "NCCommand.h"
#include "NCMachineParametersC.h"

void SystemSettings::SetProjectDir(const QString& projectDir)
{
	if (projectDir.isEmpty())
		return;
	if (m_projectDir == projectDir)
		return;

	QDir dir(projectDir);
	bool dirExist = false;
	if (!dir.exists())
	{
		dirExist = dir.mkdir(projectDir);
	}
	else
	{
		dirExist = true;
	}
	if (dirExist)
	{
		m_projectDir = projectDir;
	}
}

QString SystemSettings::GetDataFilePath(const QString& objectName, QString projectDir)
{
	if (projectDir == NULL)
		projectDir = "data";

	return projectDir + QDir::separator() + QString("%1_data.json").arg(objectName);
}

QString SystemSettings::AppendDataFilePath(const QString& filePath, const QString& append)
{
	int dotIndex = filePath.lastIndexOf('.');
	if (dotIndex >= 5 && filePath.mid(dotIndex - 5, 5) == "_data")
		dotIndex -= 5;
	QString newFilePath = filePath;
	newFilePath.insert(dotIndex, "_s_" + append);
	return newFilePath;
}

void SystemSettings::LoadFromFile(const QString& filePath)
{
	if (filePath.endsWith(".ini")) {
		QSettings settings(filePath, QSettings::IniFormat);
		//if (!session.value("ModbusWriteAsError").isNull()) {
		//	SetValue("ModbusWriteAsError", session.value("ModbusWriteAsError").toString());
		//}
		QStringList allKeys = settings.allKeys();
		for (const QString& key : allKeys) {
			QVariant value = settings.value(key);
			SetValue(key, value.toString());
		}
	}
}

BasePropertyObject::BasePropertyObject(QObject* parent, const QString& name) :
	QObject(parent)
{
	setObjectName(name);
}

QList<ModbusTask*> BasePropertyObject::ExecuteCmds(NCMachine* ncMachine, int idx) const
{
	QList<ModbusTask*> tasks;
	if (idx != -1) {
		assert(idx >= 0 && idx < TASKQUEUE_MAX_COUNT);
		ncMachine->enterSetPriority(idx);
	}
	int n = GetCmdAddress();
	if (n >= TMBS_MAP0_ID_MAX) {
		tasks = ncMachine->executeCmdsByTncOrder(n - TMBS_MAP0_ID_MAX, GetValues());
	}
	else {
		tasks = ncMachine->executeCmdsByAddress(GetCmdAddress(), GetValues());
	}
	if (idx != -1) {
		ncMachine->exitSetPriority();
		// 如果已经exitSetPriority，说明任务已经加入到队列并可能执行并delete了，不能再操作tasks
		return QList<ModbusTask*> { NULL };
	}
	return tasks;
}

JogPropertyObject::JogPropertyObject(QObject* parent, const QString& name) :
	BasePropertyObject(parent, name), m_JOGENABLE(true), m_tddzsnw(false), m_fdzlw(false), m_isgz(true), m_ms(MsEnum::SDMS)
{
	//// Child object.
	//if (hasChild)
	//    new JogPropertyObject("child", this, false);

	headers = {
			{ "objectName", QStringLiteral("名称")},
			{ "JOGENABLE", QStringLiteral("JOG使能") },
			{ "tddzsnw", QStringLiteral("抬刀动作失能位") },
			{ "fdzlw", QStringLiteral("放电暂停位") },
			{ "ms", QStringLiteral("模式") },
			{ "jcdl", QStringLiteral("监测短路") },
			{ "isgz", QStringLiteral("公制") },
			{ "xV", QStringLiteral("X轴速度mm/min") },
			{ "xaccl", QStringLiteral("加速时间ms") },
			{ "yV", QStringLiteral("Y轴速度mm/min") },
			{ "yaccl", QStringLiteral("Y轴加速时间ms") },
			{ "zV", QStringLiteral("Z轴速度mm/min") },
			{ "zaccl", QStringLiteral("Z轴加速时间ms") },
			{ "uV", QStringLiteral("U轴速度mm/min") },
			{ "uaccl", QStringLiteral("U轴加速时间ms") },
	};

	enumHeaders = {
		{ "SDMS", QStringLiteral("手动模式")},
		{ "DDMS", QStringLiteral("点动模式")},
		{ "SLMS", QStringLiteral("手轮模式")},
	};
}

std::vector<uint16_t> JogPropertyObject::GetValues() const
{
	std::vector<uint16_t> ret;

	uint16_t uint1 = 0;
	uint1 |= m_JOGENABLE ? 0x1 : 0x0;
	//uint1 |= m_tddzsnw ? 0x2 : 0x0;
	uint1 |= m_fdzlw ? 0x2 : 0x0;
	ret.push_back(uint1);

	uint1 = 0;
	switch (m_ms)
	{
	case JogPropertyObject::SDMS:
		uint1 |= 0x0;
		break;
	case JogPropertyObject::DDMS:
		uint1 |= 0x1;
		break;
	case JogPropertyObject::SLMS:
		uint1 |= 0x2;
		break;
	default:
		break;
	}

	//uint1 |= m_jcdl ? 0x8 : 0x0;
	uint1 |= m_isgz ? 0x0 : 0x10;
	ret.push_back(uint1);

	ret.push_back(m_xV);
	ret.push_back(m_xaccl);
	ret.push_back(m_yV);
	ret.push_back(m_yaccl);
	ret.push_back(m_zV);
	ret.push_back(m_zaccl);
	ret.push_back(m_uV);
	ret.push_back(m_uaccl);

	if (m_run)
		ret.push_back(TNC_ORDER0_JOG);
	else
		ret.push_back(-1);
	assert(ret.size() == 11);

	return ret;
}

int JogPropertyObject::GetCmdAddress() const
{
	return TMBS_MAP0_ID_MAX + TNC_ORDER0_JOG;
}

void JogPropertyObject::ExecuteCmdsFdzlw(NCMachine* ncMachine, bool fdzlw, int idx)
{
	if (idx != -1) {
		assert(idx >= 0 && idx < TASKQUEUE_MAX_COUNT);
		ncMachine->enterSetPriority(idx);
	}
	// pause
	if (fdzlw) {
		ncMachine->GoApi();
		// GoJog在外面设置（等设置了lastPos后再调用）
		//ncMachine->GoJog();
	}
	// resume
	else {
		ncMachine->GoApi();

		SPKPropertyObject* spkP = PropertyObjects::getInstance()->propertyObjectSPK;
		spkP->setzgj(3);
		auto v = spkP->GetValues();
		ncMachine->executeCmdsByTncOrder(TNC_ORDER0_SPK, v);
	}
	if (idx != -1) {
		ncMachine->exitSetPriority();
	}

	// old 
	/*m_fdzlw = fdzlw;

	std::vector<uint16_t> vs;

	uint16_t uint1 = 0;
	uint1 |= m_JOGENABLE ? 0x1 : 0x0;
	uint1 |= m_fdzlw ? 0x2 : 0x0;
	vs.push_back(uint1);

	if (idx != -1) {
		assert(idx >= 0 && idx < TASKQUEUE_MAX_COUNT);
		ncMachine->enterSetPriority(idx);
	}

	ncMachine->executeCmdsByAddress(TMBS_MAP0_ID_NCTCTR, vs);

	if (idx != -1) {
		ncMachine->exitSetPriority();
	}*/
}

void JogPropertyObject::ExecuteCmdSetMs(NCMachine* ncMachine, int idx)
{
	if (idx != -1) {
		assert(idx >= 0 && idx < TASKQUEUE_MAX_COUNT);
		ncMachine->enterSetPriority(idx);
	}
	std::vector<uint16_t> ret;

	uint16_t uint1 = 0;
	switch (m_ms)
	{
	case JogPropertyObject::SDMS:
		uint1 |= 0x0;
		break;
	case JogPropertyObject::DDMS:
		uint1 |= 0x1;
		break;
	case JogPropertyObject::SLMS:
		uint1 |= 0x2;
		break;
	default:
		break;
	}

	//uint1 |= m_jcdl ? 0x8 : 0x0;
	uint1 |= m_isgz ? 0x0 : 0x10;
	ret.push_back(uint1);

	ncMachine->executeCmdsByAddress(TMBS_MAP0_ID_JOGCFG, ret);
	if (idx != -1) {
		ncMachine->exitSetPriority();
	}
}

LocPropertyObject::LocPropertyObject(QObject* parent, const QString& name) :
	BasePropertyObject(parent, name), m_x(0), m_y(0), m_z(0), m_u(0)
{
	headers = {
			{ "objectName", QStringLiteral("名称")},
			{ "x", QStringLiteral("X轴目标坐标") },
			{ "y", QStringLiteral("Y轴目标坐标") },
			{ "z", QStringLiteral("Z轴目标坐标") },
			{ "u", QStringLiteral("U轴目标坐标") },
			{ "jcdl", QStringLiteral("监测短路") },
			{ "V", QStringLiteral("轴速度mm/min") },
			{ "accl", QStringLiteral("加速时间ms") },
	};
}

std::vector<uint16_t> LocPropertyObject::GetValues() const
{
	std::vector<uint16_t> ret;

	ret.push_back(TNC_ORDER0_LOC);
	ret.push_back(0);

	ret.push_back(m_x & 0xFFFF);
	ret.push_back(m_x >> 16);
	ret.push_back(m_y & 0xFFFF);
	ret.push_back(m_y >> 16);
	ret.push_back(m_z & 0xFFFF);
	ret.push_back(m_z >> 16);
	if (LOC_AXIS_LEN >= 4) {
		ret.push_back(m_u & 0xFFFF);
		ret.push_back(m_u >> 16);
	}

	ret.push_back(m_V);
	ret.push_back(m_accl);

	ret.push_back(static_cast<uint16_t>(m_jcdl));
	ret.push_back(0);

	assert(ret.size() == 6 + 2 * LOC_AXIS_LEN);

	return ret;
}

int LocPropertyObject::GetCmdAddress() const
{
	return TMBS_MAP0_ID_MAX + TNC_ORDER0_LOC;
}

CmvTouchPropertyObject::CmvTouchPropertyObject(QObject* parent, const QString& name) :
	BasePropertyObject(parent, name), m_ddfx(DdfxEnum::X_PLUS)
{
	headers = {
			{ "objectName", QStringLiteral("名称")},
			{ "xV", QStringLiteral("X轴速度mm/min") },
			{ "xaccl", QStringLiteral("X轴加速时间ms") },
			{ "yV", QStringLiteral("Y轴速度mm/min") },
			{ "yaccl", QStringLiteral("Y轴加速时间ms") },
			{ "ddfx", QStringLiteral("对刀方向") },
	};
}

std::vector<uint16_t> CmvTouchPropertyObject::GetValues() const
{
	std::vector<uint16_t> ret;

	ret.push_back(m_xV);
	ret.push_back(m_xaccl);
	ret.push_back(m_yV);
	ret.push_back(m_yaccl);

	ret.push_back(TNC_ORDER0_TOUCH);
	ret.push_back(0);
	ret.push_back(static_cast<uint16_t>(m_ddfx));
	ret.push_back(0);

	assert(ret.size() == 8);

	return ret;
}

int CmvTouchPropertyObject::GetCmdAddress() const
{
	return TMBS_MAP0_ID_MAX + TNC_ORDER0_TOUCH;
}

CmvTolmtPropertyObject::CmvTolmtPropertyObject(QObject* parent, const QString& name) :
	BasePropertyObject(parent, name), m_ddfx(DdfxEnum::X_PLUS)
{
	headers = {
			{ "objectName", QStringLiteral("名称")},
			{ "xV", QStringLiteral("轴速度mm/min") },
			{ "xaccl", QStringLiteral("加速时间ms") },
			{ "ddfx", QStringLiteral("对刀方向") },
			{ "jcdl", QStringLiteral("监测短路") },
	};
}

std::vector<uint16_t> CmvTolmtPropertyObject::GetValues() const
{
	std::vector<uint16_t> ret;

	ret.push_back(0);
	ret.push_back(0);
	ret.push_back(m_xV);
	ret.push_back(m_xaccl);

	ret.push_back(TNC_ORDER0_TOLMT);
	ret.push_back(0);
	ret.push_back(static_cast<uint16_t>(m_ddfx));
	ret.push_back(m_jcdl);

	assert(ret.size() == 8);

	return ret;
}
int CmvTolmtPropertyObject::GetCmdAddress() const
{
	return TMBS_MAP0_ID_MAX + TNC_ORDER0_TOLMT;
}

CmvFrmlmtPropertyObject::CmvFrmlmtPropertyObject(QObject* parent, const QString& name) :
	BasePropertyObject(parent, name), m_ddfx(DdfxEnum::X_PLUS)
{
	headers = {
			{ "objectName", QStringLiteral("名称")},
			{ "xV", QStringLiteral("轴速度mm/min") },
			{ "xaccl", QStringLiteral("加速时间ms") },
			{ "ddfx", QStringLiteral("对刀方向") },
			{ "jcdl", QStringLiteral("监测短路") },
	};
}

std::vector<uint16_t> CmvFrmlmtPropertyObject::GetValues() const
{
	std::vector<uint16_t> ret;

	ret.push_back(0);
	ret.push_back(0);
	ret.push_back(m_xV);
	ret.push_back(m_xaccl);

	ret.push_back(TNC_ORDER0_FRMLMT);
	ret.push_back(0);
	ret.push_back(static_cast<uint16_t>(m_ddfx));
	ret.push_back(m_jcdl);

	assert(ret.size() == 8);

	return ret;
}
int CmvFrmlmtPropertyObject::GetCmdAddress() const
{
	return TMBS_MAP0_ID_MAX + TNC_ORDER0_FRMLMT;
}

CmvTozsigPropertyObject::CmvTozsigPropertyObject(QObject* parent, const QString& name) :
	BasePropertyObject(parent, name), m_ddfx(DdfxEnum::X_PLUS)
{
	headers = {
			{ "objectName", QStringLiteral("名称")},
			{ "xV", QStringLiteral("轴速度mm/min") },
			{ "xaccl", QStringLiteral("加速时间ms") },
			{ "ddfx", QStringLiteral("对刀方向") },
			{ "jcdl", QStringLiteral("监测短路") },
	};
}

std::vector<uint16_t> CmvTozsigPropertyObject::GetValues() const
{
	std::vector<uint16_t> ret;

	ret.push_back(0);
	ret.push_back(0);
	ret.push_back(m_xV);
	ret.push_back(m_xaccl);

	ret.push_back(TNC_ORDER0_TOZSIG);
	ret.push_back(0);
	ret.push_back(static_cast<uint16_t>(m_ddfx));
	ret.push_back(m_jcdl);

	assert(ret.size() == 8);

	return ret;
}
int CmvTozsigPropertyObject::GetCmdAddress() const
{
	return TMBS_MAP0_ID_MAX + TNC_ORDER0_TOZSIG;
}

SPKPropertyObject::SPKPropertyObject(QObject* parent, const QString& name) :
	BasePropertyObject(parent, name)
{
	headers = {
			{"x", QStringLiteral("X") },
			{"y", QStringLiteral("Y") },
			{"z", QStringLiteral("Z") },
			{"zgj", QStringLiteral("子轨迹") },
	};
}
std::vector<uint16_t> SPKPropertyObject::GetValues() const
{
	std::vector<uint16_t> ret;

	ret.push_back(TNC_ORDER0_SPK);
	ret.push_back(0);

	ret.push_back(m_x & 0xFFFF);
	ret.push_back(m_x >> 16);
	ret.push_back(m_y & 0xFFFF);
	ret.push_back(m_y >> 16);
	ret.push_back(m_z & 0xFFFF);
	ret.push_back(m_z >> 16);
	if (SPK_AXIS_LEN >= 4) {
		ret.push_back(m_u & 0xFFFF);
		ret.push_back(m_u >> 16);
	}

	ret.push_back(m_zgj);

	ret.push_back(0);

	assert(ret.size() == 4 + 2 * SPK_AXIS_LEN);

	return ret;
}

int SPKPropertyObject::GetCmdAddress() const
{
	return TMBS_MAP0_ID_MAX + TNC_ORDER0_SPK;
}

FDQDPropertyObject::FDQDPropertyObject(QObject* parent, const QString& name) :
	BasePropertyObject(parent, name), m_kgygzs(Enum1::Enum1NONE), m_fddrxz(Enum2::Enum2NONE), m_dgsnw(Enum3::DLDG), m_dzsnw(Enum4::DLDZ), m_fjxjgw(Enum5::DJWZJ), m_fzdyxz(Enum6::BWQGB)
{
	headers = {
			{ "objectName", QStringLiteral("名称")},
			{ "a1", QStringLiteral("开电流数（0.3A）") },
			{ "a2", QStringLiteral("开电流数（0.5A）") },
			{ "a3", QStringLiteral("开电流数（1A）") },
			{ "a4", QStringLiteral("开电流数（2A）") },
			{ "a5", QStringLiteral("开电流数（4A）") },
			{ "a6", QStringLiteral("开电流数（8A）") },
			{ "a7", QStringLiteral("开电流数（16A）") },
			{ "Slv1", QStringLiteral("辅助电流1（32A）") },
			{ "Slv2", QStringLiteral("辅助电流2（32A）") },
			{ "LLV", QStringLiteral("低压电源1档（LLV）") },
			{ "HLV", QStringLiteral("低压电源2档（HLV）") },
			{ "Slv3", QStringLiteral("辅助电流3（32A）") },
			{ "kgygzs", QStringLiteral("开高压管子数") },
			{ "fddrxz", QStringLiteral("放电电容选择") },
			{ "dgsnw", QStringLiteral("电感失能位") },
			{ "dzsnw", QStringLiteral("电阻失能位") },
			{ "fjxjgw", QStringLiteral("反极性加工位") },
			{ "fzdyxz", QStringLiteral("辅助电压选择") },
	};

	enumHeaders = {
		{ "Enum1NONE", QStringLiteral("不开高压")},
		{ "Enum2NONE", QStringLiteral("无电容")},
		{ "DGJRHL", QStringLiteral("电感接入回路")},
		{ "DLDG", QStringLiteral("短路电感")},
		{ "DZJRHL", QStringLiteral("电阻接入回路")},
		{ "DLDZ", QStringLiteral("短路电阻")},
		{ "DJWZJ", QStringLiteral("电极为正极")},
		{ "DJWFJ", QStringLiteral("电极为负极")},
		{ "BWQGB", QStringLiteral("不完全关闭")},
		{ "ZYFZ", QStringLiteral("中压辅助")},
		{ "GZFZ", QStringLiteral("高压辅助")},
		{ "WQGB", QStringLiteral("完全关闭")},
	};
}

std::vector<uint16_t> FDQDPropertyObject::GetValues() const
{
	std::vector<uint16_t> ret;

	// 先低位再高位，高位在后
	uint16_t uint1 = 0;
	uint1 |= m_a1 ? 0x1 : 0x0;
	uint1 |= m_a2 ? 0x2 : 0x0;
	uint1 |= m_a3 ? 0x4 : 0x0;
	uint1 |= m_a4 ? 0x8 : 0x0;
	uint1 |= m_a5 ? 0x10 : 0x0;
	uint1 |= m_a6 ? 0x20 : 0x0;
	uint1 |= m_a7 ? 0x40 : 0x0;
	uint1 |= m_Slv1 ? 0x80 : 0x0;
	uint1 |= m_Slv2 ? 0x100 : 0x0;
	uint1 |= m_LLV ? 0x200 : 0x0;
	uint1 |= m_HLV ? 0x400 : 0x0;
	uint1 |= false ? 0x800 : 0x0;
	uint1 |= false ? 0x1000 : 0x0;
	uint1 |= m_Slv3 ? 0x2000 : 0x0;
	uint1 |= false ? 0x4000 : 0x0;
	uint1 |= false ? 0x8000 : 0x0;
	ret.push_back(uint1);

	uint16_t uint2 = 0;
	uint2 |= (uint16_t)m_kgygzs;
	uint2 |= (uint16_t)m_fddrxz << 4;
	uint2 |= m_dgsnw == Enum3::DLDG ? 0x0100 : 0x0;
	uint2 |= m_dzsnw == Enum4::DLDZ ? 0x0200 : 0x0;
	uint2 |= m_fjxjgw == Enum5::DJWFJ ? 0x0400 : 0x0;
	uint2 |= false ? 0x0800 : 0x0;
	uint2 |= (uint16_t)m_fzdyxz << 12;
	ret.push_back(uint2);

	assert(ret.size() == 2);

	return ret;
}
int FDQDPropertyObject::GetCmdAddress() const
{
	return TMBS_MAP0_ID_MCUP;
}

FZQDPropertyObject::FZQDPropertyObject(QObject* parent, const QString& name) :
	BasePropertyObject(parent, name)
{
	headers = {
			{ "objectName", QStringLiteral("名称")},
			{ "yb1", QStringLiteral("油泵1") },
			{ "yb2", QStringLiteral("油泵2") },
			{ "out1", QStringLiteral("红色报警灯") },
			{ "out2", QStringLiteral("绿色报警灯") },
			{ "out3", QStringLiteral("OUT3") },
			{ "out4", QStringLiteral("OUT4") },
			//{ "bjd", QStringLiteral("报警灯") }
	};
}

std::vector<uint16_t> FZQDPropertyObject::GetValues() const
{
	std::vector<uint16_t> ret;

	uint16_t uint1 = 0;
	uint1 |= m_yb1 ? 0x100 : 0x0;
	uint1 |= m_yb2 ? 0x200 : 0x0;
	uint1 |= m_out1 ? 0x400 : 0x0;
	uint1 |= m_out2 ? 0x800 : 0x0;
	uint1 |= m_out3 ? 0x1000 : 0x0;
	uint1 |= m_out4 ? 0x2000 : 0x0;
	ret.push_back(uint1);

	assert(ret.size() == 1);

	return ret;
}
int FZQDPropertyObject::GetCmdAddress() const
{
	return TMBS_MAP0_ID_MCUSW;
}

SKHKZPropertyObject::SKHKZPropertyObject(QObject* parent, const QString& name) :
	BasePropertyObject(parent, name)
{
	headers = {
			{ "objectName", QStringLiteral("名称")},
			{ "ztkz", QStringLiteral("状态控制") },
			{ "xs", QStringLiteral("显示使能") },
			{ "jpsd", QStringLiteral("键盘锁定") },
			{ "sczsn", QStringLiteral("轴操作使能") },
			{ "sc32wjm", QStringLiteral("上传32位键码") },
			{ "isgz", QStringLiteral("公制（mm）") },
			{ "zgs", QStringLiteral("显示/操作轴个数") },
			{ "sdxskz", QStringLiteral("速度显示控制") },
			{ "ddsdkz", QStringLiteral("点动/手动显示控制") }
	};
	enumHeaders = {
		{ "Enum1NONE", QStringLiteral("无状态")},
		{ "ZTJGZT", QStringLiteral("暂停加工状态")},
		{ "JGZT", QStringLiteral("加工状态")},
		{ "SD", QStringLiteral("手动（连续移动）")},
		{ "DD", QStringLiteral("点动移动")},
	};
}

std::vector<uint16_t> SKHKZPropertyObject::GetValues() const
{
	std::vector<uint16_t> ret;

	uint16_t uint1 = 0;
	uint1 |= (uint16_t)m_ztkz;
	uint1 |= m_xs ? 0x8 : 0x0;
	uint1 |= m_jpsd ? 0x10 : 0x0;
	uint1 |= m_sczsn ? 0x20 : 0x0;
	uint1 |= m_sc32wjm ? 0x40 : 0x0;
	uint1 |= m_isgz ? 0x0 : 0x80;
	uint1 |= m_zgs << 8;
	ret.push_back(uint1);

	uint16_t uint2 = (uint16_t)m_sdxskz;
	uint2 |= m_ddsdkz == Enum3::DD ? 0x8000 : 0x0;
	ret.push_back(uint2);

	assert(ret.size() == 2);

	return ret;
}
int SKHKZPropertyObject::GetCmdAddress() const
{
	return TMBS_MAP0_ID_HCTRL;
}

ZLJBCPropertyObject::ZLJBCPropertyObject(QObject* parent, const QString& name) :
	BasePropertyObject(parent, name)
{
	headers = {
			{ "objectName", QStringLiteral("名称")},
			{ "bcjg", QStringLiteral("补偿间隔") },
			{ "bcckd", QStringLiteral("补偿参考点") },
			{ "zfxbczdjl", QStringLiteral("正方向补偿使用的最大记录") },
			{ "ffxbczdjl", QStringLiteral("负方向补偿使用的最小记录") },
			{ "fxjxbc", QStringLiteral("反向间隙补偿数据") },
			{ "dwsjzdbc", QStringLiteral("单位时间最大补偿数据") },
			{ "bcsx", QStringLiteral("补偿生效") }
	};
}

std::vector<uint16_t> ZLJBCPropertyObject::GetValues() const
{
	std::vector<uint16_t> ret;

	ret.push_back(m_bcjg & 0xFFFF);
	ret.push_back(m_bcjg >> 16);
	ret.push_back(m_bcckd);
	ret.push_back(m_zfxbczdjl);
	ret.push_back(m_ffxbczdjl);
	ret.push_back(0);

	ret.push_back(m_fxjxbc);
	ret.push_back(m_dwsjzdbc);
	ret.push_back(m_bcsx);

	assert(ret.size() == 9);

	return ret;
}
int ZLJBCPropertyObject::GetCmdAddress() const
{
	assert(false);
	return -1;
}

void ZLJBCPropertyObject::ExecuteCmds1(NCMachine* ncMachine, ZLJBCPropertyObject* propertyObjects[], int idx)
{
	if (idx != -1) {
		assert(idx >= 0 && idx < TASKQUEUE_MAX_COUNT);
		ncMachine->enterSetPriority(idx);
	}

	std::vector<uint16_t> v;
	std::vector<uint16_t> vs[4];

	for (int i = 0; i < LJBC_AXIS_LEN; ++i) {
		vs[i] = propertyObjects[i]->GetValues();
	}

	for (int i = 0; i < LJBC_AXIS_LEN; ++i)
	{
		for (int j = 0; j < 6; ++j)
		{
			v.push_back(vs[i][j]);
		}
	}
	ncMachine->executeCmdsByAddress(TMBS_MAP0_ID_XCPSPOPT, v);

	v.clear();
	for (int i = 0; i < LJBC_AXIS_LEN; ++i)
		v.push_back(vs[i][6]);
	ncMachine->executeCmdsByAddress(TMBS_MAP0_ID_XGAP, v);


	v.clear();
	for (int i = 0; i < LJBC_AXIS_LEN; ++i)
		v.push_back(vs[i][7]);
	ncMachine->executeCmdsByAddress(TMBS_MAP0_ID_XGAPMAXPT, v);

	// TMBS_MAP0_ID_CPSPOPTCFG 这个另外发(ExecuteCmds2)，一般开机清零的时候

	if (idx != -1) {
		ncMachine->exitSetPriority();
	}
}

void ZLJBCPropertyObject::ExecuteCmds2(NCMachine* ncMachine, ZLJBCPropertyObject* propertyObjects[], int idx)
{
	if (idx != -1) {
		assert(idx >= 0 && idx < TASKQUEUE_MAX_COUNT);
		ncMachine->enterSetPriority(idx);
	}

	std::vector<uint16_t> v;
	std::vector<uint16_t> vs[4];

	for (int i = 0; i < LJBC_AXIS_LEN; ++i) {
		vs[i] = propertyObjects[i]->GetValues();
	}

	v.clear();
	uint16_t uint1 = 0;
	uint1 |= vs[0][8] ? 0x1 : 0x0;
	uint1 |= vs[1][8] ? 0x2 : 0x0;
	uint1 |= vs[2][8] ? 0x4 : 0x0;
	v.push_back(uint1);

	if (uint1 > 0) {
		ncMachine->executeCmdsByAddress(TMBS_MAP0_ID_CPSPOPTCFG, v);
	}

	v.clear();
	uint1 = 0;
	uint1 |= !vs[0][8] ? 0x1 : 0x0;
	uint1 |= !vs[1][8] ? 0x2 : 0x0;
	uint1 |= !vs[2][8] ? 0x4 : 0x0;
	v.push_back(uint1);

	if (uint1 > 0) {
		ncMachine->executeCmdsByAddress(TMBS_MAP0_ID_CPSPOPTCFG_DISABLE, v);
	}

	if (idx != -1) {
		ncMachine->exitSetPriority();
	}
}

DDYDPropertyObject::DDYDPropertyObject(QObject* parent, const QString& name) :
	BasePropertyObject(parent, name)
{
	headers = {
			{ "objectName", QStringLiteral("名称")},
			{"bjjgz", QStringLiteral("进给轴")},
			{ "bjmcs", QStringLiteral("步进进给脉冲数") },
	};
	enumHeaders = {
		{ "X", QStringLiteral("X")},
		{ "Y", QStringLiteral("Y")},
		{ "Z", QStringLiteral("Z")},
	};
}

std::vector<uint16_t> DDYDPropertyObject::GetValues() const
{
	std::vector<uint16_t> ret;

	ret.push_back((uint16_t)m_bjjgz);

	ret.push_back(m_bjmcs);

	assert(ret.size() == 2);

	return ret;
}

int DDYDPropertyObject::GetCmdAddress() const
{
	return TMBS_MAP0_ID_BJGJSJ;
}

DJSNPropertyObject::DJSNPropertyObject(QObject* parent, const QString& name) :
	BasePropertyObject(parent, name)
{
	headers = {
			{ "objectName", QStringLiteral("名称")},
			{ "djsn", QStringLiteral("电机使能") },
	};
}

std::vector<uint16_t> DJSNPropertyObject::GetValues() const
{
	std::vector<uint16_t> ret;

	ret.push_back(m_djsn ? 0x1 : 0x0);

	assert(ret.size() == 1);

	return ret;
}
int DJSNPropertyObject::GetCmdAddress() const
{
	return TMBS_MAP0_ID_DJSNKZQ;
}

FmqPropertyObject::FmqPropertyObject(QObject* parent, const QString& name) :
	BasePropertyObject(parent, name)
{
	headers = {
			{ "objectName", QStringLiteral("名称")},
			{"mjfs", QStringLiteral("鸣叫方式")},
			{ "data2", QStringLiteral("Data2") },
			{ "data3", QStringLiteral("Data3") },
			{ "data4", QStringLiteral("Data4") },
	};
	enumHeaders = {
		{ "TYPE_OFF", QStringLiteral("关闭")},
		{ "TYPE1", QStringLiteral("鸣叫一声")},
		{ "TYPE2", QStringLiteral("等待按键按下过程中一直鸣叫")},
		{ "TYPE3", QStringLiteral("等待按键按下过程中间歇鸣叫")},
		{ "TYPE_ON", QStringLiteral("开启")},
	};
}

std::vector<uint16_t> FmqPropertyObject::GetValues() const
{
	std::vector<uint16_t> ret;

	ret.push_back((uint16_t)m_mjfs);

	ret.push_back(m_data2);
	ret.push_back(m_data3);
	ret.push_back(m_data4);

	assert(ret.size() == 4);

	return ret;
}
int FmqPropertyObject::GetCmdAddress() const
{
	return TMBS_MAP0_ID_FMQ;
}

DebugPropertyObject::DebugPropertyObject(QObject* parent, const QString& name) :
	BasePropertyObject(parent, name)
{
	headers = {
			{ "objectName", QStringLiteral("调试用途")},
			{"dl", QStringLiteral("短路")},
	};
}

std::vector<uint16_t> DebugPropertyObject::GetValues() const
{
	std::vector<uint16_t> ret;

	ret.push_back(m_dl);

	assert(ret.size() == 1);

	return ret;
}
int DebugPropertyObject::GetCmdAddress() const
{
	return 72;
}


ADCPropertyObject::ADCPropertyObject(QObject* parent, const QString& name) :
	BasePropertyObject(parent, name)
{
	headers = {
			{ "objectName", QStringLiteral("名称")},
			{"adczyxs", QStringLiteral("Adc增益系数")},
			{"adcjzsj", QStringLiteral("Adc校正数据")},
			{"sfdysc", QStringLiteral("伺服电压输出比例系数")},
			{"gobl", QStringLiteral("光耦比例系数")},
	};
}

std::vector<uint16_t> ADCPropertyObject::GetValues() const
{
	std::vector<uint16_t> ret;

	int a = (int)round(m_adczyxs * pow(2, 23));
	ret.push_back(a & 0xFFFF);
	ret.push_back(a >> 16);
	ret.push_back(m_adcjzsj & 0xFFFF);
	ret.push_back(0);

	a = (int)round(m_sfdysc * pow(2, 22));
	ret.push_back(a & 0xFFFF);
	ret.push_back(a >> 16);

	a = (int)round(m_gobl * pow(2, 20));
	ret.push_back(a & 0xFFFF);
	ret.push_back(a >> 16);

	assert(ret.size() == 8);

	return ret;
}
int ADCPropertyObject::GetCmdAddress() const
{
	return TMBS_MAP0_ID_ASDCS;
}

FDHLPropertyObject::FDHLPropertyObject(QObject* parent, const QString& name) :
	BasePropertyObject(parent, name)
{
	headers = {
			{ "objectName", QStringLiteral("名称")},
			{ "a1", QStringLiteral("开电流数（0.3A）") },
			{ "a2", QStringLiteral("开电流数（0.5A）") },
			{ "a3", QStringLiteral("开电流数（1A）") },
			{ "a4", QStringLiteral("开电流数（2A）") },
			{ "a5", QStringLiteral("开电流数（4A）") },
			{ "a6", QStringLiteral("开电流数（8A）") },
			{ "a7", QStringLiteral("开电流数（16A）") },
			{ "Slv1", QStringLiteral("辅助电流1（32A）") },
			{ "Slv2", QStringLiteral("辅助电流2（32A）") },
			{ "LV", QStringLiteral("低压电源档（LV）") },
			{ "Slv3", QStringLiteral("辅助电流3（32A）") },
			{ "kgygzs", QStringLiteral("开高压管子数") },
			{ "fddrxz", QStringLiteral("放电电容选择") },
			{ "dgsnw", QStringLiteral("电感失能位") },
			{ "dzsnw", QStringLiteral("电阻失能位") },
			{ "fjxjgw", QStringLiteral("正负极性控制位") },
			{ "fzdyxz", QStringLiteral("辅助电压选择") },
	};

	//enumHeaders = {
	//	{ "Enum1NONE", QStringLiteral("不开高压")},
	//	{ "Enum2NONE", QStringLiteral("无电容")},
	//	{ "DGJRHL", QStringLiteral("电感接入回路")},
	//	{ "DLDG", QStringLiteral("短路电感")},
	//	{ "DZJRHL", QStringLiteral("电阻接入回路")},
	//	{ "DLDZ", QStringLiteral("短路电阻")},
	//	{ "DJWZJ", QStringLiteral("电极为正极")},
	//	{ "DJWFJ", QStringLiteral("电极为负极")},
	//	{ "BWQGB", QStringLiteral("不完全关闭")},
	//	{ "ZYFZ", QStringLiteral("中压辅助")},
	//	{ "GZFZ", QStringLiteral("高压辅助")},
	//	{ "WQGB", QStringLiteral("完全关闭")},
	//};
}

std::vector<uint16_t> FDHLPropertyObject::GetValues() const
{
	std::vector<uint16_t> ret;

	// 先低位再高位，高位在后
	uint16_t uint1 = 0;
	uint1 |= m_a1 ? 0x1 : 0x0;
	uint1 |= m_a2 ? 0x2 : 0x0;
	uint1 |= m_a3 ? 0x4 : 0x0;
	uint1 |= m_a4 ? 0x8 : 0x0;
	uint1 |= m_a5 ? 0x10 : 0x0;
	uint1 |= m_a6 ? 0x20 : 0x0;
	uint1 |= m_a7 ? 0x40 : 0x0;
	uint1 |= m_Slv1 ? 0x80 : 0x0;
	uint1 |= m_Slv2 ? 0x100 : 0x0;
	//uint1 |= m_LLV ? 0x200 : 0x0;
	//uint1 |= m_HLV ? 0x400 : 0x0;
	//uint1 |= false ? 0x800 : 0x0;
	//uint1 |= false ? 0x1000 : 0x0;
	uint1 |= (m_LV << 9) & 0x0E00;
	uint1 |= m_Slv3 ? 0x2000 : 0x0;
	uint1 |= false ? 0x4000 : 0x0;
	uint1 |= false ? 0x8000 : 0x0;
	ret.push_back(uint1);

	uint16_t uint2 = 0;
	uint2 |= (uint16_t)m_kgygzs;
	uint2 |= (uint16_t)m_fddrxz << 4;
	uint2 |= m_dgsnw == 1 ? 0x0100 : 0x0;
	uint2 |= m_dzsnw == 1 ? 0x0200 : 0x0;
	uint2 |= m_fjxjgw == 1 ? 0x0400 : 0x0;
	uint2 |= false ? 0x0800 : 0x0;
	uint2 |= (uint16_t)m_fzdyxz << 12;
	ret.push_back(uint2);

	assert(ret.size() == 2);

	return ret;
}
int FDHLPropertyObject::GetCmdAddress() const
{
	return TMBS_MAP0_ID_FDHLXZCS;
}

FDJGPropertyObject::FDJGPropertyObject(QObject* parent, const QString& name) :
	BasePropertyObject(parent, name)
{
	headers = {
			{"fdjgcfdy", QStringLiteral("放电加工伺服电压") },
			{"jgmcmk", QStringLiteral("加工脉冲脉宽") },
			{"jgmcmj", QStringLiteral("加工脉冲脉间") },
			{"jgmcdnlsn", QStringLiteral("--加工脉冲等能量使能") },
			{"jgmcdyycjcsn", QStringLiteral("--加工脉冲电压异常检测使能") },
			{"jgmcfzmcxz", QStringLiteral("加工脉冲辅助脉冲选择") },
			{"jcycjcms", QStringLiteral("击穿异常检测模式")},
			{"jcycsjyz", QStringLiteral("击穿异常时间阈值") },
			{"fdcfsd", QStringLiteral("放电伺服速度") },
			{"fdcftzsd", QStringLiteral("放电伺服调整速度") },
	};
}

std::vector<uint16_t> FDJGPropertyObject::GetValues() const
{
	std::vector<uint16_t> ret;
	ret.push_back(m_fdjgcfdy);

	ret.push_back(m_jgmcmk);
	ret.push_back(m_jgmcmj);
	ret.push_back(m_jgmcdnlsn);
	ret.push_back(m_jgmcdyycjcsn < 63 ? m_jgmcdyycjcsn : 63);
	//ret.push_back(m_jgmcdnlsn ? 1 : 0);
	//ret.push_back(m_jgmcdyycjcsn ? 1 : 0);
	ret.push_back(m_jgmcfzmcxz >= 0 && m_jgmcfzmcxz <= 2 ? m_jgmcfzmcxz : 0);
	ret.push_back( ((m_jcycjcms & 0xFF) << 8) | (m_jcycjcms == 0 ? 0 : 1));

	ret.push_back(m_fdcfsd >= 0 && m_fdcfsd <= 9 ? m_fdcfsd : 0);
	ret.push_back(m_fdcftzsd >= 0 && m_fdcftzsd <= 9 ? m_fdcftzsd : 0);

	assert(ret.size() == 9);

	return ret;
}
int FDJGPropertyObject::GetCmdAddress() const
{
	return TMBS_MAP0_ID_FDJGCFDY;
}

void FDJGPropertyObject::ExecuteCmdsSetSv(NCMachine* ncMachine, int sv, int idx)
{
	if (idx != -1) {
		assert(idx >= 0 && idx < TASKQUEUE_MAX_COUNT);
		ncMachine->enterSetPriority(idx);
	}
	NCMachineParametersC::SetCurrentByName(QString("SV"), QString::number(sv));
	PropertyObjects::getInstance()->propertyObjectFdjg->setfdjgcfdy(sv);
	this->ExecuteCmds(ncMachine);

	if (idx != -1) {
		ncMachine->exitSetPriority();
	}
}

FDWCPropertyObject::FDWCPropertyObject(QObject* parent, const QString& name) :
	BasePropertyObject(parent, name)
{
	headers = {
			{"fdwcms", QStringLiteral("放电完成模式") },
			{"fddssj", QStringLiteral("放电定时时间") },
			{"tdfx", QStringLiteral("抬刀方向") },
			{"tdgd", QStringLiteral("抬刀高度") },
			{"tdsd", QStringLiteral("抬刀速度") },
			{"fdsj", QStringLiteral("放电时间") },
			{"ectdxydtdcs", QStringLiteral("二次抬刀需要的抬刀次数") },
			{"ectdgdbs", QStringLiteral("二次抬刀高度倍数") },
	};
}
std::vector<uint16_t> FDWCPropertyObject::GetValues() const
{
	std::vector<uint16_t> ret;
	ret.push_back(m_fdwcms >= 0 && m_fdwcms <= 3 ? m_fdwcms : 0);

	ret.push_back(m_fddssj & 0xFFFF);
	ret.push_back(m_fddssj >> 16);

	ret.push_back(m_tdfx >= 0 && m_tdfx <= 9 ? m_tdfx : 0);

	ret.push_back(m_tdgd & 0xFFFF);
	ret.push_back(m_tdgd >> 16);

	ret.push_back(m_tdsd & 0xFFFF);

	ret.push_back(m_fdsj & 0xFFFF);

	ret.push_back(m_ectdxydtdcs);
	ret.push_back(m_ectdgdbs);

	assert(ret.size() == 10);

	return ret;
}
int FDWCPropertyObject::GetCmdAddress() const
{
	return TMBS_MAP0_ID_FDWCMS;
}

YDPropertyObject::YDPropertyObject(QObject* parent, const QString& name) :
	BasePropertyObject(parent, name)
{
	headers = {
			{"ydfx", QStringLiteral("摇动方向") },
			{"zdydsd", QStringLiteral("最大摇动速度") },
			{"sdjcxfs", QStringLiteral("深度检测细分数") },
			{"ydfwcfms", QStringLiteral("摇动复位伺服模式") },
			{"ydfwdzms", QStringLiteral("摇动复位动作模式") },
			{"ydpm", QStringLiteral("摇动平面") },
			{"d1xxydm", QStringLiteral("第1象限摇动码") },
			{"d2xxydm", QStringLiteral("第2象限摇动码") },
			{"d3xxydm", QStringLiteral("第3象限摇动码") },
			{"d4xxydm", QStringLiteral("第4象限摇动码") },
			{"ydbj", QStringLiteral("摇动半径") },
			{"hzxtdgd", QStringLiteral("回中心抬刀高度") },
			{"hzxtdsd", QStringLiteral("回中心抬刀速度") },
			{"hzxtdjssj", QStringLiteral("回中心抬刀加速时间") },
			{"ztdjssj", QStringLiteral("主抬刀加速时间") },
			{"tcqhtjl", QStringLiteral("退出前回退距离") },
			{"tdqmshtjl", QStringLiteral("抬刀前慢速回退距离") },
			{"tdqmshtsd", QStringLiteral("抬刀前慢速回退速度") },
	};
}
std::vector<uint16_t> YDPropertyObject::GetValues() const
{
	std::vector<uint16_t> ret;

	// REG61-63
	ret.push_back(m_ydfx >= 0 && m_ydfx <= 2 ? m_ydfx : 2);
	ret.push_back(m_zdydsd);
	ret.push_back(m_sdjcxfs);

	// REG64
	uint16_t a = 0;
	a |= m_ydfwcfms >= 0 && m_ydfwcfms <= 2 ? m_ydfwcfms : 0;
	a |= (m_ydfwdzms >= 0 && m_ydfwdzms <= 4 ? m_ydfwdzms : 0) << 3;
	a |= (m_ydpm >= 0 && m_ydpm <= 3 ? m_ydpm : 0) << 6;
	ret.push_back(a);

	a = 0;
	a |= (m_d1xxydm >= 0 && m_d1xxydm <= 5 ? m_d1xxydm : 0);
	a |= (m_d2xxydm >= 0 && m_d2xxydm <= 5 ? m_d2xxydm : 0) << 4;
	a |= (m_d3xxydm >= 0 && m_d3xxydm <= 5 ? m_d3xxydm : 0) << 8;
	a |= (m_d4xxydm >= 0 && m_d4xxydm <= 5 ? m_d4xxydm : 0) << 12;
	ret.push_back(a);

	ret.push_back(m_ydbj & 0xFFFF);
	ret.push_back(m_ydbj >> 16);

	// REG65
	ret.push_back(m_hzxtdgd);
	ret.push_back(0);
	ret.push_back(m_hzxtdsd);
	ret.push_back(m_hzxtdjssj);

	ret.push_back(m_ztdjssj);

	ret.push_back(m_tcqhtjl & 0xFFFF);
	ret.push_back(m_tcqhtjl >> 16);

	ret.push_back(m_tdqmshtjl & 0xFFFF);
	ret.push_back(m_tdqmshtjl >> 16);

	ret.push_back(m_tdqmshtsd);

	assert(ret.size() == 17);
	return ret;
}
int YDPropertyObject::GetCmdAddress() const
{
	return TMBS_MAP0_ID_YDFX;
}

VersionPropertyObject::VersionPropertyObject(QObject* parent, const QString& name)
	: BasePropertyObject(parent, name)
{
	headers = {
			{ "versionSkh", QStringLiteral("手控盒版本") },
			{ "versionXskz", QStringLiteral("显示控制版本") },
			{ "versionXwj", QStringLiteral("下位机版本") },
			{ "versionXwjnh", QStringLiteral("下位机内核版本") },
	};
}

void VersionPropertyObject::SetValues1(std::vector<uint16_t> v)
{
	//m_version1 = (v[0] >> 8) & 0xFF;
	//m_version2 = (v[1] >> 8) & 0xFF;
	m_version1 = v[0];
	m_version2 = v[1];
}

void VersionPropertyObject::SetValues2(std::vector<uint16_t> v)
{
	m_version3 = v[0];
	m_version4 = v[1];
}


YouwenJianchePropertyObject::YouwenJianchePropertyObject(QObject* parent, const QString& name)
	: BasePropertyObject(parent, name)
{
	headers = {
			{ "youWen", QStringLiteral("油温检测") },
			{ "youWei", QStringLiteral("油位检测") },
			{ "huoYan", QStringLiteral("火焰检测") },
	};
}

std::vector<uint16_t> YouwenJianchePropertyObject::GetValues() const
{
	std::vector<uint16_t> ret;
	ret.push_back(m_ywjc ? 1 : 0);
	ret.push_back(m_ywjc2 ? 1 : 0);
	ret.push_back(m_hyjc ? 1 : 0);
	assert(ret.size() == 3);
	return ret;
}

int YouwenJianchePropertyObject::GetCmdAddress() const
{
	return TMBS_MAP0_ID_YOUWENJIANCHE;
}

AJCPropertyObject::AJCPropertyObject(QObject* parent, const QString& name) : BasePropertyObject(parent, name)
{
	headers = {
			{ "Ajc", QStringLiteral("Ajc") },
			
	};
}

std::vector<uint16_t> AJCPropertyObject::GetValues() const
{
	std::vector<uint16_t> ret;
	ret.push_back(m_ajc);
	assert(ret.size() == 1);
	return ret;
}

int AJCPropertyObject::GetCmdAddress() const
{
	return TMBS_MAP0_ID_AJC;
}


Reg78PropertyObject::Reg78PropertyObject(QObject* parent, const QString& name) : BasePropertyObject(parent, name)
{
	headers = {
			{ "fsycgbsj", QStringLiteral("风扇延迟关闭时间") },

	};
}

std::vector<uint16_t> Reg78PropertyObject::GetValues() const
{
	std::vector<uint16_t> ret;
	ret.push_back(m_fsycgbsj);
	assert(ret.size() == 1);
	return ret;
}

int Reg78PropertyObject::GetCmdAddress() const
{
	return TMBS_MAP0_ID_REG78;
}

Reg81PropertyObject::Reg81PropertyObject(QObject* parent, const QString& name) : BasePropertyObject(parent, name)
{
}

std::vector<uint16_t> Reg81PropertyObject::GetValues() const
{
	DataForm* dataForm = DataForms::getInstance()->getDataForm("xitongshezhi5");
	int n = 10;

	std::vector<uint16_t> ret;
	for (int i = 0; i < n; ++i) {
		QString s = QString("KZSDBS%1").arg(i + 1);
		ret.push_back(dataForm->getValue(s).toInt());
	}
	for (int i = 0; i < n; ++i) {
		QString s = QString("SFSDBS%1").arg(i + 1);
		ret.push_back(static_cast<int>(dataForm->getValue(s).toDouble() * 100));
	}
	assert(ret.size() == 2 * n);
	return ret;
}

int Reg81PropertyObject::GetCmdAddress() const
{
	return TMBS_MAP0_ID_REG81;
}

std::vector<uint16_t> RegOnePropertyObject::GetValues() const
{
	std::vector<uint16_t> ret;
	ret.push_back(m_v);
	assert(ret.size() == 1);
	return ret;
}


Reg83PropertyObject::Reg83PropertyObject(QObject* parent, const QString& name) : BasePropertyObject(parent, name)
{
}

std::vector<uint16_t> Reg83PropertyObject::GetValues() const
{
	std::vector<uint16_t> ret;
	ret.push_back(m_ppgw);
	assert(ret.size() == 1);
	return ret;
}

int Reg83PropertyObject::GetCmdAddress() const
{
	return TMBS_MAP0_ID_REG83;
}

///////////////////////////////////////////////////////
PropertyObjects* PropertyObjects::getInstance() {
	//if (instance == nullptr) {
	//	instance = new PropertyObjects();
	//}
	return &instance;
}

PropertyObjects::PropertyObjects()
{
	propertyObjectJog = new JogPropertyObject(nullptr, QStringLiteral("JOG配置"));
	propertyObjectLoc = new LocPropertyObject(nullptr, QStringLiteral("LOC配置"));
	propertyObjectCmvTouch = new CmvTouchPropertyObject(nullptr, QStringLiteral("对刀"));
	propertyObjectCmvToLMT = new CmvTolmtPropertyObject(nullptr, QStringLiteral("去极限"));
	propertyObjectCmvFRMLMT = new CmvFrmlmtPropertyObject(nullptr, QStringLiteral("退出极限"));
	propertyObjectCmvToZSig = new CmvTozsigPropertyObject(nullptr, QStringLiteral("找零位"));
	propertyObjectSPK = new SPKPropertyObject(nullptr, QStringLiteral("SPK"));
	propertyObjectFanddianIO = new FDQDPropertyObject(nullptr, QStringLiteral("放电IO"));
	propertyObjectFuzhuIO = new FZQDPropertyObject(nullptr, QStringLiteral("辅助IO"));
	propertyObjectShouKongHe = new SKHKZPropertyObject(nullptr, QStringLiteral("手控盒控制"));
	for(int i=0; i< LJBC_AXIS_LEN; ++i)
		propertyObjectZLJBC[i] = new ZLJBCPropertyObject(nullptr, QStringLiteral("轴螺距补偿"));
	propertyObjectDianDong = new DDYDPropertyObject(nullptr, QStringLiteral("点动移动"));
	propertyObjectDianJiShiNeng = new DJSNPropertyObject(nullptr, QStringLiteral("电机使能"));
	propertyObjectFengMingQi = new FmqPropertyObject(nullptr, QStringLiteral("蜂鸣器"));
	propertyObjectDebug = new DebugPropertyObject(nullptr, QStringLiteral("调试用途"));
	propertyObjectADC = new ADCPropertyObject(nullptr, QStringLiteral("ADC"));
	propertyObjectFdhl = new FDHLPropertyObject(nullptr, QStringLiteral("放电回路"));
	propertyObjectFdjg = new FDJGPropertyObject(nullptr, QStringLiteral("放电加工"));
	propertyObjectFdwc = new FDWCPropertyObject(nullptr, QStringLiteral("放电完成"));
	propertyObjectYd = new YDPropertyObject(nullptr, QStringLiteral("摇动"));

	propertyObjectVersion = new VersionPropertyObject(nullptr, QStringLiteral("版本号"));
	propertyObjectYouwenJianche = new YouwenJianchePropertyObject(nullptr, QStringLiteral("油温检测"));
	propertyObjectAJC = new AJCPropertyObject(nullptr, QStringLiteral("AJC"));

	propertyObjectReg78 = new Reg78PropertyObject(nullptr, QStringLiteral("Reg78"));
	propertyObjectReg81 = new Reg81PropertyObject(nullptr, QStringLiteral("Reg81"));
	propertyObjectReg83 = new Reg83PropertyObject(nullptr, QStringLiteral("Reg83"));
	propertyObjectReg84 = new Reg84PropertyObject(nullptr, QStringLiteral("Reg84"));
	propertyObjectReg85 = new Reg85PropertyObject(nullptr, QStringLiteral("Reg85"));
	propertyObjectReg86 = new Reg86PropertyObject(nullptr, QStringLiteral("Reg86"));
}
PropertyObjects::~PropertyObjects() {
	if (propertyObjectJog)
		delete propertyObjectJog;
	if (propertyObjectLoc)
		delete propertyObjectLoc;
	if (propertyObjectCmvTouch)
		delete propertyObjectCmvTouch;
	if (propertyObjectCmvToLMT)
		delete propertyObjectCmvToLMT;
	if (propertyObjectCmvFRMLMT)
		delete propertyObjectCmvFRMLMT;
	if (propertyObjectCmvToZSig)
		delete propertyObjectCmvToZSig;
	if (propertyObjectSPK)
		delete propertyObjectSPK;
	if (propertyObjectFanddianIO)
	  delete propertyObjectFanddianIO;
	if (propertyObjectFuzhuIO)
	  delete propertyObjectFuzhuIO;
	if (propertyObjectShouKongHe)
		delete propertyObjectShouKongHe;
	for (int i = 0; i < LJBC_AXIS_LEN; ++i)
	{
		if (propertyObjectZLJBC[i])
			delete propertyObjectZLJBC[i];
	}
	if (propertyObjectDianDong)
		delete propertyObjectDianDong;
	if (propertyObjectDianJiShiNeng)
		delete propertyObjectDianJiShiNeng;
	if (propertyObjectFengMingQi)
		delete propertyObjectFengMingQi;
	if (propertyObjectDebug)
		delete propertyObjectDebug;

	if (propertyObjectADC)
		delete propertyObjectADC;
	if (propertyObjectFdhl)
		delete propertyObjectFdhl;
	if (propertyObjectFdjg)
		delete propertyObjectFdjg;
	if (propertyObjectFdwc)
		delete propertyObjectFdwc;
	if (propertyObjectYd)
		delete propertyObjectYd;

	if (propertyObjectVersion)
		delete propertyObjectVersion;

	if (propertyObjectYouwenJianche)
		delete propertyObjectYouwenJianche;

	if (propertyObjectAJC)
		delete propertyObjectAJC;

	if (propertyObjectReg78)
		delete propertyObjectReg78;
	if (propertyObjectReg81)
		delete propertyObjectReg81;
	if (propertyObjectReg83)
		delete propertyObjectReg83;
	if (propertyObjectReg84)
		delete propertyObjectReg84;
	if (propertyObjectReg85)
		delete propertyObjectReg85;
	if (propertyObjectReg86)
		delete propertyObjectReg86;
	//if (instance != nullptr) {
	//	delete instance;
	//}
}

void PropertyObjects::LoadData()
{
	DataForm* dataForm = DataForms::getInstance()->getDataForm("xitongshezhi2");
	propertyObjectJog->setxV(dataForm->getValue("SPEED_JOG_V_X").toInt());
	propertyObjectJog->setyV(dataForm->getValue("SPEED_JOG_V_Y").toInt());
	propertyObjectJog->setzV(dataForm->getValue("SPEED_JOG_V_Z").toInt());
	propertyObjectJog->setuV(dataForm->getValue("SPEED_JOG_V_U").toInt());
	propertyObjectJog->setxaccl(dataForm->getValue("SPEED_JOG_T_X").toInt());
	propertyObjectJog->setyaccl(dataForm->getValue("SPEED_JOG_T_Y").toInt());
	propertyObjectJog->setzaccl(dataForm->getValue("SPEED_JOG_T_Z").toInt());
	propertyObjectJog->setuaccl(dataForm->getValue("SPEED_JOG_T_U").toInt());

	propertyObjectLoc->setV(dataForm->getValue("SPEED_LOC_V").toInt());
	propertyObjectLoc->setaccl(dataForm->getValue("SPEED_LOC_T").toInt());

	propertyObjectCmvToLMT->setxV(dataForm->getValue("SPEED_LOC_V").toInt());
	propertyObjectCmvToLMT->setyV(dataForm->getValue("SPEED_LOC_V").toInt());
	propertyObjectCmvToLMT->setxaccl(dataForm->getValue("SPEED_LOC_T").toInt());
	propertyObjectCmvToLMT->setyaccl(dataForm->getValue("SPEED_LOC_T").toInt());

	propertyObjectCmvTouch->setxV(dataForm->getValue("SPEED_CMV_V").toInt());
	propertyObjectCmvTouch->setyV(dataForm->getValue("SPEED_CMV_V").toInt());
	propertyObjectCmvTouch->setxaccl(dataForm->getValue("SPEED_CMV_T").toInt());
	propertyObjectCmvTouch->setyaccl(dataForm->getValue("SPEED_CMV_T").toInt());

	propertyObjectCmvFRMLMT->setxV(dataForm->getValue("SPEED_CMV_TOZIG_V").toInt());
	propertyObjectCmvFRMLMT->setyV(dataForm->getValue("SPEED_CMV_TOZIG_V").toInt());
	propertyObjectCmvToZSig->setxV(dataForm->getValue("SPEED_CMV_TOZIG_V").toInt());
	propertyObjectCmvToZSig->setyV(dataForm->getValue("SPEED_CMV_TOZIG_V").toInt());
	propertyObjectCmvFRMLMT->setxaccl(dataForm->getValue("SPEED_CMV_TOZIG_T").toInt());
	propertyObjectCmvFRMLMT->setyaccl(dataForm->getValue("SPEED_CMV_TOZIG_T").toInt());
	propertyObjectCmvToZSig->setxaccl(dataForm->getValue("SPEED_CMV_TOZIG_T").toInt());
	propertyObjectCmvToZSig->setyaccl(dataForm->getValue("SPEED_CMV_TOZIG_T").toInt());

	DataForm* dataForm3 = DataForms::getInstance()->getDataForm("xitongshezhi3");
	propertyObjectYd->setsdjcxfs(dataForm3->getValue("JGJSPDFGS").toInt());
	propertyObjectYd->settdqmshtjl(dataForm3->getValue("MTDGD").toInt());
	propertyObjectYd->settdqmshtsd(dataForm3->getValue("MTDSD").toInt());

	propertyObjectYd->settcqhtjl(dataForm3->getValue("TCQHTJL").toInt());

	propertyObjectYd->sethzxtdgd(dataForm3->getValue("HZXTDGD").toInt());
	propertyObjectYd->sethzxtdsd(dataForm3->getValue("HZXTDSD").toInt());
	propertyObjectYd->sethzxtdjssj(dataForm3->getValue("HZXTDJSSJ").toInt());

	propertyObjectYd->setztdjssj(dataForm3->getValue("ZTDJSSJ").toInt());

	DataForm* dataForm4 = DataForms::getInstance()->getDataForm("xitongshezhi4");
	propertyObjectReg78->setfsycgbsj(dataForm4->getValue("FSYCGBSJ").toInt());

	DataForm* dataForm5 = DataForms::getInstance()->getDataForm("xitongshezhi5");
	propertyObjectReg84->setv(dataForm5->getValue("SFHFJL").toInt());
	propertyObjectReg85->setv(dataForm5->getValue("AJCFHJL").toInt());

}

PropertyObjects PropertyObjects::instance;


