#include <cmath>
#include "NCMachineProperties.h"
#include "NCMachineDefines.h"
#include "NCMachine.h"
#include "utils/dataform.h"
#include "NCCommand.h"
#include "NCMachineParametersC.h"


BasePropertyObject::BasePropertyObject(QObject* parent, const QString& name) :
	QObject(parent)
{
	setObjectName(name);

	headers["objectName"] = tr("MC");
}

QList<ModbusTask*> BasePropertyObject::ExecuteCmds(NCMachine* ncMachine, int idx) const
{
	QList<ModbusTask*> tasks;
	if (idx != -1) {
		assert(idx >= 0 && idx < TASKQUEUE_MAX_COUNT);
		ncMachine->enterSetPriority(idx);
	}
	int n = GetCmdAddress();

	int connectionIndex = 0;
	if (n >= TCPIPBASEADDR) {
		n -= TCPIPBASEADDR;
		connectionIndex = 1;
	}

	if (n >= TMBS_MAP0_ID_MAX) {
		tasks = ncMachine->executeCmdsByTncOrder(n - TMBS_MAP0_ID_MAX, GetValues());
	}
	else {
		tasks = ncMachine->executeCmdsByAddress(GetCmdAddress(), GetValues());
	}
	if (connectionIndex > 0) {
		for (auto t : tasks) {
			t->connectionIndex = connectionIndex;
		}
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
// [AUTO-TRANSLATION-COMMENT] 名称
			{ "objectName", tr("MC")},
// [AUTO-TRANSLATION-COMMENT] JOG使能
			{ "JOGENABLE", tr("JSN") },
// [AUTO-TRANSLATION-COMMENT] 抬刀动作失能位
			{ "tddzsnw", tr("TDDZSNW") },
// [AUTO-TRANSLATION-COMMENT] 放电暂停位
			{ "fdzlw", tr("FDZTW") },
// [AUTO-TRANSLATION-COMMENT] 模式
			{ "ms", tr("MS") },
// [AUTO-TRANSLATION-COMMENT] 监测短路
			{ "jcdl", tr("JCDL") },
// [AUTO-TRANSLATION-COMMENT] 公制
			{ "isgz", tr("GZ") },
// [AUTO-TRANSLATION-COMMENT] X轴速度mm/min
			{ "xV", tr("XZSDM") },
// [AUTO-TRANSLATION-COMMENT] 加速时间ms
			{ "xaccl", tr("JSSJM") },
// [AUTO-TRANSLATION-COMMENT] Y轴速度mm/min
			{ "yV", tr("YZSDM") },
// [AUTO-TRANSLATION-COMMENT] Y轴加速时间ms
			{ "yaccl", tr("YZJSSJM") },
// [AUTO-TRANSLATION-COMMENT] Z轴速度mm/min
			{ "zV", tr("ZZSDM") },
// [AUTO-TRANSLATION-COMMENT] Z轴加速时间ms
			{ "zaccl", tr("ZZJSSJM") },
// [AUTO-TRANSLATION-COMMENT] U轴速度mm/min
			{ "uV", tr("UZSDM") },
// [AUTO-TRANSLATION-COMMENT] U轴加速时间ms
			{ "uaccl", tr("UZJSSJM") },
	};

	enumHeaders = {
// [AUTO-TRANSLATION-COMMENT] 手动模式
		{ "SDMS", tr("SDMS")},
// [AUTO-TRANSLATION-COMMENT] 点动模式
		{ "DDMS", tr("DDMS")},
// [AUTO-TRANSLATION-COMMENT] 手轮模式
		{ "SLMS", tr("SLMS")},
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
// [AUTO-TRANSLATION-COMMENT] 名称
			{ "objectName", tr("MC")},
// [AUTO-TRANSLATION-COMMENT] X轴目标坐标
			{ "x", tr("XZMBZB") },
// [AUTO-TRANSLATION-COMMENT] Y轴目标坐标
			{ "y", tr("YZMBZB") },
// [AUTO-TRANSLATION-COMMENT] Z轴目标坐标
			{ "z", tr("ZZMBZB") },
// [AUTO-TRANSLATION-COMMENT] U轴目标坐标
			{ "u", tr("UZMBZB") },
// [AUTO-TRANSLATION-COMMENT] 监测短路
			{ "jcdl", tr("JCDL") },
// [AUTO-TRANSLATION-COMMENT] 轴速度mm/min
			{ "V", tr("ZSDM") },
// [AUTO-TRANSLATION-COMMENT] 加速时间ms
			{ "accl", tr("JSSJM") },
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
// [AUTO-TRANSLATION-COMMENT] 名称
			{ "objectName", tr("MC")},
// [AUTO-TRANSLATION-COMMENT] X轴速度mm/min
			{ "xV", tr("XZSDM") },
// [AUTO-TRANSLATION-COMMENT] X轴加速时间ms
			{ "xaccl", tr("XZJSSJM") },
// [AUTO-TRANSLATION-COMMENT] Y轴速度mm/min
			{ "yV", tr("YZSDM") },
// [AUTO-TRANSLATION-COMMENT] Y轴加速时间ms
			{ "yaccl", tr("YZJSSJM") },
// [AUTO-TRANSLATION-COMMENT] 对刀方向
			{ "ddfx", tr("DDFX") },
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
// [AUTO-TRANSLATION-COMMENT] 名称
			{ "objectName", tr("MC")},
// [AUTO-TRANSLATION-COMMENT] 轴速度mm/min
			{ "xV", tr("ZSDM") },
// [AUTO-TRANSLATION-COMMENT] 加速时间ms
			{ "xaccl", tr("JSSJM") },
// [AUTO-TRANSLATION-COMMENT] 对刀方向
			{ "ddfx", tr("DDFX") },
// [AUTO-TRANSLATION-COMMENT] 监测短路
			{ "jcdl", tr("JCDL") },
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
// [AUTO-TRANSLATION-COMMENT] 名称
			{ "objectName", tr("MC")},
// [AUTO-TRANSLATION-COMMENT] 轴速度mm/min
			{ "xV", tr("ZSDM") },
// [AUTO-TRANSLATION-COMMENT] 加速时间ms
			{ "xaccl", tr("JSSJM") },
// [AUTO-TRANSLATION-COMMENT] 对刀方向
			{ "ddfx", tr("DDFX") },
// [AUTO-TRANSLATION-COMMENT] 监测短路
			{ "jcdl", tr("JCDL") },
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
// [AUTO-TRANSLATION-COMMENT] 名称
			{ "objectName", tr("MC")},
// [AUTO-TRANSLATION-COMMENT] 轴速度mm/min
			{ "xV", tr("ZSDM") },
// [AUTO-TRANSLATION-COMMENT] 加速时间ms
			{ "xaccl", tr("JSSJM") },
// [AUTO-TRANSLATION-COMMENT] 对刀方向
			{ "ddfx", tr("DDFX") },
// [AUTO-TRANSLATION-COMMENT] 监测短路
			{ "jcdl", tr("JCDL") },
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
			{"x", QString("X") },
			{"y", QString("Y") },
			{"z", QString("Z") },
// [AUTO-TRANSLATION-COMMENT] 子轨迹
			{"zgj", tr("ZGJ") },
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
// [AUTO-TRANSLATION-COMMENT] 名称
			{ "objectName", tr("MC")},
// [AUTO-TRANSLATION-COMMENT] 开电流数
			{ "a1", tr("KDLS") + "0.3A"},
// [AUTO-TRANSLATION-COMMENT] 开电流数
			{ "a2", tr("KDLS") + "0.5A"},
// [AUTO-TRANSLATION-COMMENT] 开电流数
			{ "a3", tr("KDLS") + "1A"},
// [AUTO-TRANSLATION-COMMENT] 开电流数
			{ "a4", tr("KDLS") + "2A"},
// [AUTO-TRANSLATION-COMMENT] 开电流数
			{ "a5", tr("KDLS") + "4A"},
// [AUTO-TRANSLATION-COMMENT] 开电流数
			{ "a6", tr("KDLS") + "8A"},
// [AUTO-TRANSLATION-COMMENT] 开电流数
			{ "a7", tr("KDLS") + "16A"},
// [AUTO-TRANSLATION-COMMENT] 辅助电流1（32A）
			{ "Slv1", tr("FZDL1") },
// [AUTO-TRANSLATION-COMMENT] 辅助电流2（32A）
			{ "Slv2", tr("FZDL2") },
// [AUTO-TRANSLATION-COMMENT] 低压电源1档（LLV）
			{ "LLV", tr("DYDY1D") + "LLV"},
// [AUTO-TRANSLATION-COMMENT] 低压电源2档（HLV）
			{ "HLV", tr("DYDY2D") + "HLV"},
// [AUTO-TRANSLATION-COMMENT] 辅助电流3（32A）
			{ "Slv3", tr("FZDL3") },
// [AUTO-TRANSLATION-COMMENT] 开高压管子数
			{ "kgygzs", tr("KGYGZS") },
// [AUTO-TRANSLATION-COMMENT] 放电电容选择
			{ "fddrxz", tr("FDDRXZ") },
// [AUTO-TRANSLATION-COMMENT] 电感失能位
			{ "dgsnw", tr("DGSNW") },
// [AUTO-TRANSLATION-COMMENT] 电阻失能位
			{ "dzsnw", tr("DZSNW") },
// [AUTO-TRANSLATION-COMMENT] 反极性加工位
			{ "fjxjgw", tr("FJXJGW") },
// [AUTO-TRANSLATION-COMMENT] 辅助电压选择
			{ "fzdyxz", tr("FZDYXZ") },
	};

	enumHeaders = {
// [AUTO-TRANSLATION-COMMENT] 不开高压
		{ "Enum1NONE", tr("BKGY")},
// [AUTO-TRANSLATION-COMMENT] 无电容
		{ "Enum2NONE", tr("WDR")},
// [AUTO-TRANSLATION-COMMENT] 电感接入回路
		{ "DGJRHL", tr("DGJRHL")},
// [AUTO-TRANSLATION-COMMENT] 短路电感
		{ "DLDG", tr("DLDG")},
// [AUTO-TRANSLATION-COMMENT] 电阻接入回路
		{ "DZJRHL", tr("DZJRHL")},
// [AUTO-TRANSLATION-COMMENT] 短路电阻
		{ "DLDZ", tr("DLDZ")},
// [AUTO-TRANSLATION-COMMENT] 电极为正极
		{ "DJWZJ", tr("DJWZJ")},
// [AUTO-TRANSLATION-COMMENT] 电极为负极
		{ "DJWFJ", tr("DJWFJ")},
// [AUTO-TRANSLATION-COMMENT] 不完全关闭
		{ "BWQGB", tr("BWQGB")},
// [AUTO-TRANSLATION-COMMENT] 中压辅助
		{ "ZYFZ", tr("ZYFZ")},
// [AUTO-TRANSLATION-COMMENT] 高压辅助
		{ "GZFZ", tr("GYFZ")},
// [AUTO-TRANSLATION-COMMENT] 完全关闭
		{ "WQGB", tr("WQGB")},
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
// [AUTO-TRANSLATION-COMMENT] 名称
			{ "objectName", tr("MC")},
// [AUTO-TRANSLATION-COMMENT] 油泵1
			{ "yb1", tr("YB1") },
// [AUTO-TRANSLATION-COMMENT] 油泵2
			{ "yb2", tr("YB2") },
// [AUTO-TRANSLATION-COMMENT] 红色报警灯
			{ "out1", tr("HSBJD") },
// [AUTO-TRANSLATION-COMMENT] 绿色报警灯
			{ "out2", tr("LSBJD") },
			{ "out3", tr("OUT3") },
			{ "out4", tr("OUT4") },
			//{ "bjd", tr("BJD") }
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
// [AUTO-TRANSLATION-COMMENT] 名称
			{ "objectName", tr("MC")},
// [AUTO-TRANSLATION-COMMENT] 状态控制
			{ "ztkz", tr("ZTKZ") },
// [AUTO-TRANSLATION-COMMENT] 显示使能
			{ "xs", tr("XSSN") },
// [AUTO-TRANSLATION-COMMENT] 键盘锁定
			{ "jpsd", tr("JPSD") },
// [AUTO-TRANSLATION-COMMENT] 轴操作使能
			{ "sczsn", tr("ZCZSN") },
// [AUTO-TRANSLATION-COMMENT] 上传32位键码
			{ "sc32wjm", tr("SC3WJM") },
// [AUTO-TRANSLATION-COMMENT] 公制（mm）
			{ "isgz", tr("GZ") },
// [AUTO-TRANSLATION-COMMENT] 显示/操作轴个数
			{ "zgs", tr("XS/CZZGS") },
// [AUTO-TRANSLATION-COMMENT] 速度显示控制
			{ "sdxskz", tr("SDXSKZ") },
// [AUTO-TRANSLATION-COMMENT] 点动/手动显示控制
			{ "ddsdkz", tr("DD/SDXSKZ") }
	};
	enumHeaders = {
// [AUTO-TRANSLATION-COMMENT] 无状态
		{ "Enum1NONE", tr("WZT")},
// [AUTO-TRANSLATION-COMMENT] 暂停加工状态
		{ "ZTJGZT", tr("ZTJGZT")},
// [AUTO-TRANSLATION-COMMENT] 加工状态
		{ "JGZT", tr("JGZT")},
// [AUTO-TRANSLATION-COMMENT] 手动（连续移动）
		{ "SD", tr("SDLXYD")},
// [AUTO-TRANSLATION-COMMENT] 点动移动
		{ "DD", tr("DDYD")},
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
// [AUTO-TRANSLATION-COMMENT] 名称
			{ "objectName", tr("MC")},
// [AUTO-TRANSLATION-COMMENT] 补偿间隔
			{ "bcjg", tr("BCJG") },
// [AUTO-TRANSLATION-COMMENT] 补偿参考点
			{ "bcckd", tr("BCCKD") },
// [AUTO-TRANSLATION-COMMENT] 正方向补偿使用的最大记录
			{ "zfxbczdjl", tr("ZFXBCSYDZDJL") },
// [AUTO-TRANSLATION-COMMENT] 负方向补偿使用的最小记录
			{ "ffxbczdjl", tr("FFXBCSYDZXJL") },
// [AUTO-TRANSLATION-COMMENT] 反向间隙补偿数据
			{ "fxjxbc", tr("FXJXBCSJ") },
// [AUTO-TRANSLATION-COMMENT] 单位时间最大补偿数据
			{ "dwsjzdbc", tr("DWSJZDBCSJ") },
// [AUTO-TRANSLATION-COMMENT] 补偿生效
			{ "bcsx", tr("BCSX") }
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
// [AUTO-TRANSLATION-COMMENT] 名称
			{ "objectName", tr("MC")},
// [AUTO-TRANSLATION-COMMENT] 进给轴
			{ "bjjgz", tr("JJZ")},
// [AUTO-TRANSLATION-COMMENT] 步进进给脉冲数
			{ "bjmcs", tr("BJJJMCS") },
	};
	enumHeaders = {
		{ "X", QString("X")},
		{ "Y", QString("Y")},
		{ "Z", QString("Z")},
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
// [AUTO-TRANSLATION-COMMENT] 名称
			{ "objectName", tr("MC")},
// [AUTO-TRANSLATION-COMMENT] 电机使能
			{ "djsn", tr("DJSN") },
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
// [AUTO-TRANSLATION-COMMENT] 名称
			{ "objectName", tr("MC")},
// [AUTO-TRANSLATION-COMMENT] 鸣叫方式
			{"mjfs", tr("MJFS")},
			{ "data2", QString("Data2") },
			{ "data3", QString("Data3") },
			{ "data4", QString("Data4") },
	};
	enumHeaders = {
// [AUTO-TRANSLATION-COMMENT] 关闭
		{ "TYPE_OFF", tr("GB")},
// [AUTO-TRANSLATION-COMMENT] 鸣叫一声
		{ "TYPE1", tr("MJYS")},
// [AUTO-TRANSLATION-COMMENT] 等待按键按下过程中一直鸣叫
		{ "TYPE2", tr("DDAJAXGCZYZMJ")},
// [AUTO-TRANSLATION-COMMENT] 等待按键按下过程中间歇鸣叫
		{ "TYPE3", tr("DDAJAXGCZJXMJ")},
// [AUTO-TRANSLATION-COMMENT] 开启
		{ "TYPE_ON", tr("KQ")},
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
// [AUTO-TRANSLATION-COMMENT] 调试用途
			{ "objectName", tr("TSYT")},
// [AUTO-TRANSLATION-COMMENT] 短路
			{"dl", tr("DL")},
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
// [AUTO-TRANSLATION-COMMENT] 名称
			{ "objectName", tr("MC")},
// [AUTO-TRANSLATION-COMMENT] Adc增益系数
			{"adczyxs", tr("AZYXS")},
// [AUTO-TRANSLATION-COMMENT] Adc校正数据
			{"adcjzsj", tr("AJZSJ")},
// [AUTO-TRANSLATION-COMMENT] 伺服电压输出比例系数
			{"sfdysc", tr("CFDYSCBLXS")},
// [AUTO-TRANSLATION-COMMENT] 光耦比例系数
			{"gobl", tr("GOBLXS")},
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
// [AUTO-TRANSLATION-COMMENT] 名称
			{ "objectName", tr("MC")},
// [AUTO-TRANSLATION-COMMENT] 开电流数
			{ "a1", tr("KDLS") + "0.3A"},
// [AUTO-TRANSLATION-COMMENT] 开电流数
			{ "a2", tr("KDLS") + "0.5A" },
// [AUTO-TRANSLATION-COMMENT] 开电流数
			{ "a3", tr("KDLS") + "1A" },
// [AUTO-TRANSLATION-COMMENT] 开电流数
			{ "a4", tr("KDLS") + "2A" },
// [AUTO-TRANSLATION-COMMENT] 开电流数
			{ "a5", tr("KDLS") + "4A" },
// [AUTO-TRANSLATION-COMMENT] 开电流数
			{ "a6", tr("KDLS") + "8A" },
// [AUTO-TRANSLATION-COMMENT] 开电流数
			{ "a7", tr("KDLS") + "16A" },
// [AUTO-TRANSLATION-COMMENT] 辅助电流1（32A）
			{ "Slv1", tr("FZDL1") },
// [AUTO-TRANSLATION-COMMENT] 辅助电流2（32A）
			{ "Slv2", tr("FZDL2") },
// [AUTO-TRANSLATION-COMMENT] 低压电源1档
			{ "LV", tr("DYDYD") },
// [AUTO-TRANSLATION-COMMENT] 辅助电流3（32A）
			{ "Slv3", tr("FZDL3") },
// [AUTO-TRANSLATION-COMMENT] 开高压管子数
			{ "kgygzs", tr("KGYGZS") },
// [AUTO-TRANSLATION-COMMENT] 放电电容选择
			{ "fddrxz", tr("FDDRXZ") },
// [AUTO-TRANSLATION-COMMENT] 电感失能位
			{ "dgsnw", tr("DGSNW") },
// [AUTO-TRANSLATION-COMMENT] 电阻失能位
			{ "dzsnw", tr("DZSNW") },
// [AUTO-TRANSLATION-COMMENT] 正负极性控制位
			{ "fjxjgw", tr("ZFJXKZW") },
// [AUTO-TRANSLATION-COMMENT] 辅助电压选择
			{ "fzdyxz", tr("FZDYXZ") },
	};

	//enumHeaders = {
// [AUTO-TRANSLATION-COMMENT] 不开高压
	//	{ "Enum1NONE", tr("BKGY")},
// [AUTO-TRANSLATION-COMMENT] 无电容
	//	{ "Enum2NONE", tr("WDR")},
// [AUTO-TRANSLATION-COMMENT] 电感接入回路
	//	{ "DGJRHL", tr("DGJRHL")},
// [AUTO-TRANSLATION-COMMENT] 短路电感
	//	{ "DLDG", tr("DLDG")},
// [AUTO-TRANSLATION-COMMENT] 电阻接入回路
	//	{ "DZJRHL", tr("DZJRHL")},
// [AUTO-TRANSLATION-COMMENT] 短路电阻
	//	{ "DLDZ", tr("DLDZ")},
// [AUTO-TRANSLATION-COMMENT] 电极为正极
	//	{ "DJWZJ", tr("DJWZJ")},
// [AUTO-TRANSLATION-COMMENT] 电极为负极
	//	{ "DJWFJ", tr("DJWFJ")},
// [AUTO-TRANSLATION-COMMENT] 不完全关闭
	//	{ "BWQGB", tr("BWQGB")},
// [AUTO-TRANSLATION-COMMENT] 中压辅助
	//	{ "ZYFZ", tr("ZYFZ")},
// [AUTO-TRANSLATION-COMMENT] 高压辅助
	//	{ "GZFZ", tr("GYFZ")},
// [AUTO-TRANSLATION-COMMENT] 完全关闭
	//	{ "WQGB", tr("WQGB")},
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
// [AUTO-TRANSLATION-COMMENT] 放电加工伺服电压
			{"fdjgcfdy", tr("FDJGCFDY") },
// [AUTO-TRANSLATION-COMMENT] 加工脉冲脉宽
			{"jgmcmk", tr("JGMCMK") },
// [AUTO-TRANSLATION-COMMENT] 加工脉冲脉间
			{"jgmcmj", tr("JGMCMJ") },
// [AUTO-TRANSLATION-COMMENT] --加工脉冲等能量使能
			{"jgmcdnlsn", tr("-JGMCDNLSN") },
// [AUTO-TRANSLATION-COMMENT] --加工脉冲电压异常检测使能
			{"jgmcdyycjcsn", tr("-JGMCDYYCJCSN") },
// [AUTO-TRANSLATION-COMMENT] 加工脉冲辅助脉冲选择
			{"jgmcfzmcxz", tr("JGMCFZMCXZ") },
// [AUTO-TRANSLATION-COMMENT] 击穿异常检测模式
			{"jcycjcms", tr("JCYCJCMS")},
// [AUTO-TRANSLATION-COMMENT] 击穿异常时间阈值
			{"jcycsjyz", tr("JCYCSJYZ") },
// [AUTO-TRANSLATION-COMMENT] 放电伺服速度
			{"fdcfsd", tr("FDCFSD") },
// [AUTO-TRANSLATION-COMMENT] 放电伺服调整速度
			{"fdcftzsd", tr("FDCFTZSD") },
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
// [AUTO-TRANSLATION-COMMENT] 放电完成模式
			{"fdwcms", tr("FDWCMS") },
// [AUTO-TRANSLATION-COMMENT] 放电定时时间
			{"fddssj", tr("FDDSSJ") },
// [AUTO-TRANSLATION-COMMENT] 抬刀方向
			{"tdfx", tr("TDFX") },
// [AUTO-TRANSLATION-COMMENT] 抬刀高度
			{"tdgd", tr("TDGD") },
// [AUTO-TRANSLATION-COMMENT] 抬刀速度
			{"tdsd", tr("TDSD") },
// [AUTO-TRANSLATION-COMMENT] 放电时间
			{"fdsj", tr("FDSJ") },
// [AUTO-TRANSLATION-COMMENT] 二次抬刀需要的抬刀次数
			{"ectdxydtdcs", tr("ECTDXYDTDCS") },
// [AUTO-TRANSLATION-COMMENT] 二次抬刀高度倍数
			{"ectdgdbs", tr("ECTDGDBS") },
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
// [AUTO-TRANSLATION-COMMENT] 摇动方向
			{"ydfx", tr("YDFX") },
// [AUTO-TRANSLATION-COMMENT] 最大摇动速度
			{"zdydsd", tr("ZDYDSD") },
// [AUTO-TRANSLATION-COMMENT] 深度检测细分数
			{"sdjcxfs", tr("SDJCXFS") },
// [AUTO-TRANSLATION-COMMENT] 摇动复位伺服模式
			{"ydfwcfms", tr("YDFWCFMS") },
// [AUTO-TRANSLATION-COMMENT] 摇动复位动作模式
			{"ydfwdzms", tr("YDFWDZMS") },
// [AUTO-TRANSLATION-COMMENT] 摇动平面
			{"ydpm", tr("YDPM") },
// [AUTO-TRANSLATION-COMMENT] 第1象限摇动码
			{"d1xxydm", tr("D1XXYDM") },
// [AUTO-TRANSLATION-COMMENT] 第2象限摇动码
			{"d2xxydm", tr("D2XXYDM") },
// [AUTO-TRANSLATION-COMMENT] 第3象限摇动码
			{"d3xxydm", tr("D3XXYDM") },
// [AUTO-TRANSLATION-COMMENT] 第4象限摇动码
			{"d4xxydm", tr("D4XXYDM") },
// [AUTO-TRANSLATION-COMMENT] 摇动半径
			{"ydbj", tr("YDBJ") },
// [AUTO-TRANSLATION-COMMENT] 回中心抬刀高度
			{"hzxtdgd", tr("HZXTDGD") },
// [AUTO-TRANSLATION-COMMENT] 回中心抬刀速度
			{"hzxtdsd", tr("HZXTDSD") },
// [AUTO-TRANSLATION-COMMENT] 回中心抬刀加速时间
			{"hzxtdjssj", tr("HZXTDJSSJ") },
// [AUTO-TRANSLATION-COMMENT] 主抬刀加速时间
			{"ztdjssj", tr("ZTDJSSJ") },
// [AUTO-TRANSLATION-COMMENT] 退出前回退距离
			{"tcqhtjl", tr("TCQHTJL") },
// [AUTO-TRANSLATION-COMMENT] 抬刀前慢速回退距离
			{"tdqmshtjl", tr("TDQMSHTJL") },
// [AUTO-TRANSLATION-COMMENT] 抬刀前慢速回退速度
			{"tdqmshtsd", tr("TDQMSHTSD") },
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
		{ "objectName", tr("MC")},
// [AUTO-TRANSLATION-COMMENT] 手控盒版本
		{ "versionSkh", tr("SKHBB") },
// [AUTO-TRANSLATION-COMMENT] 显示控制版本
		{ "versionXskz", tr("XSKZBB") },
// [AUTO-TRANSLATION-COMMENT] 下位机版本
		{ "versionXwj", tr("XWJBB") },
// [AUTO-TRANSLATION-COMMENT] 下位机内核版本
		{ "versionXwjnh", tr("XWJNHBB") },
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
		{ "objectName", tr("MC")},
// [AUTO-TRANSLATION-COMMENT] 油温检测
		{ "youWen", tr("YWJC") },
// [AUTO-TRANSLATION-COMMENT] 油温检测
		{ "youWei", tr("YWJC") },
// [AUTO-TRANSLATION-COMMENT] 火焰检测
		{ "huoYan", tr("HYJC") },
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
		{ "Ajc", QString("Ajc") },	
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
		{ "objectName", tr("MC")},
// [AUTO-TRANSLATION-COMMENT] 风扇延迟关闭时间
		{ "fsycgbsj", tr("FSYCGBSJ") },

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

PLCOperationPropertyObject::PLCOperationPropertyObject(QObject* parent, const QString& name) : BasePropertyObject(parent, name)
{
	headers = {
		{ "objectName", tr("MC")},
		{ "operation", tr("operation") },
		{ "param1", tr("param") + "1"},
		{ "param2", tr("param") + "2" },
		{ "param3", tr("param") + "3" },
	};
}

std::vector<uint16_t> PLCOperationPropertyObject::GetValues() const
{
	std::vector<uint16_t> ret;
	ret.push_back(m_operation);
	ret.push_back(m_param1);
	ret.push_back(m_param2);
	ret.push_back(m_param3);
	return ret;
}

int PLCOperationPropertyObject::GetCmdAddress() const
{
	return TMBS_MAP1_ID_PLCOPERATION;
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
	if (propertyObjectPLCOperation)
		delete propertyObjectPLCOperation;
	//if (instance != nullptr) {
	//	delete instance;
	//}
}

void PropertyObjects::CreateData()
{
// [AUTO-TRANSLATION-COMMENT] JOG配置
	propertyObjectJog = new JogPropertyObject(nullptr, QObject::tr("JPZ"));
// [AUTO-TRANSLATION-COMMENT] LOC配置
	propertyObjectLoc = new LocPropertyObject(nullptr, QObject::tr("LPZ"));
// [AUTO-TRANSLATION-COMMENT] 对刀
	propertyObjectCmvTouch = new CmvTouchPropertyObject(nullptr, QObject::tr("DD"));
// [AUTO-TRANSLATION-COMMENT] 去极限
	propertyObjectCmvToLMT = new CmvTolmtPropertyObject(nullptr, QObject::tr("QJX"));
// [AUTO-TRANSLATION-COMMENT] 退出极限
	propertyObjectCmvFRMLMT = new CmvFrmlmtPropertyObject(nullptr, QObject::tr("TCJX"));
// [AUTO-TRANSLATION-COMMENT] 找零位
	propertyObjectCmvToZSig = new CmvTozsigPropertyObject(nullptr, QObject::tr("ZLW"));
	propertyObjectSPK = new SPKPropertyObject(nullptr, QObject::tr("SPK"));
// [AUTO-TRANSLATION-COMMENT] 放电IO
	propertyObjectFanddianIO = new FDQDPropertyObject(nullptr, QObject::tr("FDI"));
// [AUTO-TRANSLATION-COMMENT] 辅助IO
	propertyObjectFuzhuIO = new FZQDPropertyObject(nullptr, QObject::tr("FZI"));
// [AUTO-TRANSLATION-COMMENT] 手控盒控制
	propertyObjectShouKongHe = new SKHKZPropertyObject(nullptr, QObject::tr("SKHKZ"));
	for (int i = 0; i < LJBC_AXIS_LEN; ++i)
// [AUTO-TRANSLATION-COMMENT] 轴螺距补偿
		propertyObjectZLJBC[i] = new ZLJBCPropertyObject(nullptr, QObject::tr("ZLJBC"));
// [AUTO-TRANSLATION-COMMENT] 点动移动
	propertyObjectDianDong = new DDYDPropertyObject(nullptr, QObject::tr("DDYD"));
// [AUTO-TRANSLATION-COMMENT] 电机使能
	propertyObjectDianJiShiNeng = new DJSNPropertyObject(nullptr, QObject::tr("DJSN"));
// [AUTO-TRANSLATION-COMMENT] 蜂鸣器
	propertyObjectFengMingQi = new FmqPropertyObject(nullptr, QObject::tr("FMQ"));
// [AUTO-TRANSLATION-COMMENT] 调试用途
	propertyObjectDebug = new DebugPropertyObject(nullptr, QObject::tr("TSYT"));
	propertyObjectADC = new ADCPropertyObject(nullptr, QObject::tr("ADC"));
// [AUTO-TRANSLATION-COMMENT] 放电回路
	propertyObjectFdhl = new FDHLPropertyObject(nullptr, QObject::tr("FDHL"));
// [AUTO-TRANSLATION-COMMENT] 放电加工
	propertyObjectFdjg = new FDJGPropertyObject(nullptr, QObject::tr("FDJG"));
// [AUTO-TRANSLATION-COMMENT] 放电完成
	propertyObjectFdwc = new FDWCPropertyObject(nullptr, QObject::tr("FDWC"));
// [AUTO-TRANSLATION-COMMENT] 摇动
	propertyObjectYd = new YDPropertyObject(nullptr, QObject::tr("YD"));

// [AUTO-TRANSLATION-COMMENT] 版本号
	propertyObjectVersion = new VersionPropertyObject(nullptr, QObject::tr("BBH"));
// [AUTO-TRANSLATION-COMMENT] 油温检测
	propertyObjectYouwenJianche = new YouwenJianchePropertyObject(nullptr, QObject::tr("YWJC"));
	propertyObjectAJC = new AJCPropertyObject(nullptr, QObject::tr("AJC"));

	propertyObjectReg78 = new Reg78PropertyObject(nullptr, QObject::tr("Reg78"));
	propertyObjectReg81 = new Reg81PropertyObject(nullptr, QObject::tr("Reg81"));
	propertyObjectReg83 = new Reg83PropertyObject(nullptr, QObject::tr("Reg83"));
	propertyObjectReg84 = new Reg84PropertyObject(nullptr, QObject::tr("Reg84"));
	propertyObjectReg85 = new Reg85PropertyObject(nullptr, QObject::tr("Reg85"));
	propertyObjectReg86 = new Reg86PropertyObject(nullptr, QObject::tr("Reg86"));

	propertyObjectPLCOperation = new PLCOperationPropertyObject(nullptr, QObject::tr("PLCOPERATION"));
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


