#pragma once

#include <gcode/machine/MachineAdapter.h>
#include <gcode/Units.h>
#include <cbang/json/Sink.h>

class NCMachine;

using namespace cb;
using namespace GCode;

class RealtimeJsonMachine : public MachineAdapter {
    Axes position;
    Units units;
    int axisSeen = 0;

    bool withLocation;
    std::string filename;
    int line = -1;
    std::string m_lastMessage = "";
	bool simpleMode = false; // 是否是简单模式(不自动发送API,JOG)

    int m_machinePriority;
    NCMachine* m_ncMachine;

	std::atomic<bool> m_exceptionThrown = false;
    std::atomic<bool> m_taskToContinue = false;
    void doTask(cb::JSON::ValuePtr sink, bool checkContinue = true);
    void waitUntilDone() const;

    std::function<void(cb::JSON::ValuePtr sink, int line)> m_doPerLineBefore = NULL;
    std::function<void(cb::JSON::ValuePtr sink, int line)> m_doPerLineAfter = NULL;
    std::function<void(std::string s)> m_doWhenCallUI = NULL;
public:
    void initFunctions() {
        m_doPerLineBefore = NULL;
        m_doPerLineAfter = NULL;
        m_doWhenCallUI = NULL;
	}
    void setFunctionDoPerLineBefore(std::function<void(cb::JSON::ValuePtr sink, int line)> doFunc) { m_doPerLineBefore = doFunc; }
    void setFunctionDoPerLineAfter(std::function<void(cb::JSON::ValuePtr sink, int line)> doFunc) { m_doPerLineAfter = doFunc; }
    void setFunctionDoWhenCallUI(std::function<void(std::string s)> doFunc) { m_doWhenCallUI = doFunc; }

	void setCurrentPriority(int priority) { m_machinePriority = priority; }

 //   std::string getLastMessage() { return m_lastMessage; }
	//void clearLastMessage() { m_lastMessage.clear(); }

    void setTaskToContinue(bool b) { m_taskToContinue = b; }
	bool isTaskToContinue() { return m_taskToContinue; }    

    void setSimpleMode(bool b) { simpleMode = b; }

    RealtimeJsonMachine(NCMachine* ncMachine, Units units) :
        m_ncMachine(ncMachine), units(units), withLocation(true), m_machinePriority(-1) {}

    // From MachineInterface
    void start() override;
    void end() override;

    void setFeed(double feed) override;
    void setFeedMode(feed_mode_t mode) override;
    void setSpeed(double speed) override;
    void setSpinMode(spin_mode_t mode, double max) override;
    void setPathMode(path_mode_t mode, double motionBlending, double naiveCAM) override;
    void changeTool(unsigned tool) override;

    void input(port_t port, input_mode_t mode, double timeout) override;
    void seek(port_t port, bool active, bool error) override;
    void output(port_t port, double value) override;

    void setPosition(const Axes& position) override;

    void dwell(double seconds) override;
    void move(const Axes& position, int axes, bool rapid, double time) override;
    void arc(const cb::Vector3D& offset, const cb::Vector3D& target,
        double angle, plane_t plane) override;
    void pause(pause_t pause) override;

    void setLocation(const cb::LocationRange& location) override;

    void comment(const std::string& s) const override;
    void message(const std::string& s) override;
    void custom(const cb::JSON::ValuePtr& value) override;
};

class GCodeInterruptException : public std::exception {
public:
    // Constructor that takes an error message
    GCodeInterruptException(const std::string& message) : msg(message) {}

    // Override the virtual what() function to return the error message
    const char* what() const noexcept override {
        return msg.c_str();
    }

private:
    std::string msg;  // Error message
};
