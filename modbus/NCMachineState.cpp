#include "NCMachineState.h"
#include <cbang/log/Logger.h>

#include "NCMachine.h"
#include "utils/LineLogger.h"

std::function<int()> NCMachineState::waitUntilNctState(uint16_t state)
{
	std::function<int()> func = [this, state]() {
		//if (m_g01Data.isRunning && m_g01Data.isPausing)
		//	return -2;
		if (m_stateDirty) {
			return -2;
		}

		if (m_state[2] == state) {
			return 1;
		}
		else {
			// 检测API状态的时候，只能检测m_state[2], 因为m_state[0] = 0是中间态
			if (state == 0) {
				return -2;
			}

			if ((m_state[0] & 0xFF00) == (state & 0xFF00)) {		// 同一个类型的指令执行结果数据
				// 正常
				uint16_t para = 0;
				uint16_t rslt = state;
				if ((m_state[0] >= rslt && m_state[0] <= rslt + 2) && (m_state[1] == para || para == 0)) {
					LOG_WARNING("NCMachine: Function waitUntilNctState(" << state << ") return 1, but not by nctState, it's by rslt. Now state is " << m_state[0] << ", " << m_state[1]);
					return 1;		// 完成任务
				}
				else {
					LOG_WARNING("NCMachine: Function waitUntilNctState(" << state << ") return -9. Now state is " << m_state[0] << ", " << m_state[1]);
					//if (doAfterFail != NULL) {
					//	doAfterFail();
					//}
					QString error = QString("ERROR:%1").arg(NCMachine::GetRLSTDesc(m_state[0], m_state[1])); // .arg(gcode.replace("\n", ""));
					LineLogger::instance().append(error);

					m_machine->enterSetPriority(0);
					m_machine->Beep();
					m_machine->exitSetPriority();

					if (doWhenWaitFail != NULL) {
						doWhenWaitFail();
						doWhenWaitFail = NULL;
					}

					return -9;		// 清空Modbus任务队列
				}
			}
			else
			{
				return -2;
			}

			//return -2;
		}
		};
	return func;
}

/*
RLST.u16Rslt 编码	RLST.u16Para 	结果说明	运行模式
( 2u )   + ( 0 )	( 1<<15 )	手动退出	JOG

( 1u )   + ( 512 )	( 0u )	正常完成退出	LOC
( 2u )   + ( 512 )	( 1<<15 )	手动退出	LOC
( 3u )   + ( 512 )	驱动器报警编号	驱动器报警退出	LOC
( 4u )   + ( 512 )	限位状态数据	触发限位退出	LOC
( 5u )   + ( 512 )	( 1<<15 )	短路退出	LOC
( 61u )  + ( 512 )	( 0u )	参数非法	LOC
( 62u )  + ( 512 )	( 0u )	校验错误	LOC
( 51u )  + ( 512 )	( 0u )	无效功能	LOC

( 1u )   + ( 768 )	( 0u )	正常完成退出	CMV
( 2u )   + ( 768 )	( 1<<15 )	手动退出	CMV
( 3u )   + ( 768 )	驱动器报警编号	驱动器报警退出	CMV
( 4u )   + ( 768 )	限位状态数据	触发限位退出	CMV
( 5u )   + ( 768 )	( 1<<15 )	短路退出	CMV
( 53u )  + ( 768 )	( 0u )	轴编码错误	CMV
( 54u )  + ( 768 )	( 0u )	操作码错误	CMV
( 51u )  + ( 768 )	( 0u )	无效功能	CMV

( 1u )  + ( 1024 )	( 0u )	正常完成退出	SPK
( 2u )  + ( 1024 )	( 1<<15 )	手动退出	SPK
( 3u )  + ( 1024 )	0xFFFF	驱动器报警退出	SPK
( 4u )  + ( 1024 )	D0~15：XL+;XL-;YL+;YL-
ZL+;Zl-…
置1：限位触发	触发限位退出	SPK
( 5u )  + ( 1024 )	( 0u )  	短路退出	SPK
( 53u ) + ( 1024 )	( 0u )	轴编码错误	SPK
( 54u ) + ( 1024 )	( 0u )	操作码错误	SPK
( 51u ) + ( 1024 )	( 0u )	无效功能	SPK

*/
std::function<int()> NCMachineState::waitUntilRLST(uint16_t rslt, uint16_t para)
{
	std::function<int()> func = [this, rslt, para]() {
		if (m_stateDirty) {
			return -2;
		}

		auto g01Data = m_machine->GetG01Data();
		if (g01Data.isRunning && g01Data.isPausing && rslt == NCT_STATE_SPK_EXTOC)
			return -2;
		// 调试时候用
		//if (m_state[0] == 768 + 5)
		//	m_state[0] = 768 + 1;

		if ((m_state[0] & 0xFF00) == (rslt & 0xFF00)) {		// 同一个类型的指令执行结果数据
			// 正常完成退出 or 手动退出 // 
			if ((m_state[0] >= rslt && m_state[0] <= rslt + 1) && (m_state[1] == para || para == 0)) {
				// for log current position
				//GetController()->getAbsolutePosition();
				return 1;		// 完成任务
			}
			else {
				//// 油位报警
				//if (m_state[0] == 6 + 1024 && m_state[1] == 2) {
				//	//auto func = convertWaitFunction([this]() {
				//	//	if (this->GetInputFlag(9))
				//	//		return -2;
				//	//	else
				//	//		return 1;
				//	//	});
				//	//auto desc = "wait for YOUWEI_FLAG";
				//	//ModbusTask* task = m_modbus->getTaskWait(func, desc);
				//	//m_modbus->addTask(task, m_currentTaskPriority);

				//	enterSetPriority(1);

				//	executeCmdWait(convertWaitFunction([this]() {
				//		if (this->GetInputFlag(9))
				//			return -2;
				//		else
				//			return 1;
				//		}), "wait for YOUWEI_FLAG");
				//	SPKPropertyObject* spkP = PropertyObjects::getInstance()->propertyObjectSPK;
				//	spkP->setzgj(3);
				//	auto v = spkP->GetValues();
				//	this->executeCmdsByTncOrder(TNC_ORDER0_SPK, v);

				//	executeCmdWait(convertWaitFunction(waitUntilNctState(NCT_STATE_SPK_RUN)), "wait for SPK_RUN");

				//	executeCmdWait(convertWaitFunction(waitUntilRLST(NCT_STATE_SPK_EXTOC, 0)), "wait for SPK_EXTOC");

				//	executeCmdWait(m_functionWaitUntilApirun, "wait for API_RUN");


				//	exitSetPriority();
				//	return 1;
				//}

				LOG_WARNING("NCMachine: Function waitUntilRLST(" << rslt << ", " << para << ") return -9. Now state is " << m_state[0] << ", " << m_state[1]);
				QString error = QString("ERROR:%1").arg(NCMachine::GetRLSTDesc(m_state[0], m_state[1])); // .arg(gcode.replace("\n", ""));
				LineLogger::instance().append(error);

				m_machine->enterSetPriority(0);
				m_machine->Beep();
				m_machine->exitSetPriority();

				if (doWhenWaitFail != NULL) {
					doWhenWaitFail();
					doWhenWaitFail = NULL;
				}

				return -9;		// 清空Modbus任务队列
			}
		}
		else {
			return -2;
		}
		};
	return func;
}