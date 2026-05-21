#pragma once

#include <functional>
#include <cbang/geom/Vector.h>

class NCMachine;

class NCMachineState
{
public:
	NCMachineState(NCMachine* machine) : m_machine(machine) {}

	cb::Vector3U GetState() { return m_state; }
	uint16_t GetRLSTRslt() { return m_state[0]; }
	uint16_t GetRLSTPara() { return m_state[1]; }
	uint16_t GetNctState() { return m_state[2]; }
	void SetState4Debug(uint16_t t1, uint16_t t2, uint16_t t3) { m_stateDebug[0] = t1; m_stateDebug[1] = t2; m_stateDebug[2] = t3; }

	std::function<int()> waitUntilNctState(uint16_t state);
	std::function<int()> waitUntilRLST(uint16_t rslt, uint16_t para = 0);
	//std::function<int()> waitPlcUntilNctState(uint16_t state);
	//std::function<int()> waitPlcUntilRLST(uint16_t rslt, uint16_t para = 0);

private:
	NCMachine* m_machine;
	// 指令执行结果，NCTState
	// 2: 运行状态
	// 0: RLST.u16Rslt
	// 1: RLST.u16Para
	std::atomic<int> m_stateDirty = 1;
	cb::Vector<3, uint16_t> m_state;
	// 端口输入信息，限位、回零等
	cb::Vector<2, uint16_t> m_inputFlag;

	//// PLC state (connection 1 - TCP)
	//std::atomic<int> m_statePlcDirty = 1;
	//cb::Vector<3, uint16_t> m_statePlc;

	cb::Vector3U m_stateDebug;

	std::function<void()> doWhenWaitFail = NULL;
};


