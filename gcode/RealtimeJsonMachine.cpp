
#include "RealtimeJsonMachine.h"

#include <cbang/Exception.h>
#include "../modbus/NCMachine.h"
#include "../modbus/NCMachineParametersC.h"

using namespace std;

namespace {
    struct dtos {
        double x;
        bool imperial;


        dtos(double x, bool imperial) : x(x), imperial(imperial) {
            if (isnan(x))
                THROW("Numerical error in GCode stream:  NaN, caused by a divide by "
                    "zero or other math error.");

            if (isinf(x))
                THROW("Numerical error in GCode stream: Infinite value");
        }


        string toString() const {
            // TODO Get precision from config
            string s = String::printf("%.*f", imperial ? 4 : 3, x);

            int chop = 0;
            for (auto it = s.rbegin(); it != s.rend() && *it == '0'; it++)
                chop++;

            if (chop) s = s.substr(0, s.length() - chop);

            return s == "-0." ? "0." : s;
        }
    };


    inline ostream& operator<<(ostream& stream, const dtos& d) {
        return stream << d.toString();
    }
}

void RealtimeJsonMachine::doTask(cb::JSON::ValuePtr sink, bool checkContinue)
{
    if (simpleMode) {
        // 简单模式下不支持任务线程模式
		throw std::runtime_error("not support async mode in simpleMode now.");
    }

    if (checkContinue) {
        // dont throw exception in endBlock
        if (m_ncMachine->GetController()->get("must_continue") == 1) {
            checkContinue = false;
        }
    }

    if (checkContinue) {
        if (!m_taskToContinue) {
            if (!m_exceptionThrown) {
                m_exceptionThrown = true;
                //throw GCode::EndProgram();
                throw GCodeInterruptException(EUtils::QString2StdString("User Cancelled"));
                //throw cb::Exception(EUtils::QString2StdString(tr("YHQX")), this->getLocation().getStart());
            }
            else {
                return;
            }
        }
    }

    if (m_doPerLineBefore)
    {
        m_doPerLineBefore(sink, line);
    }

    m_ncMachine->addTask(sink->toString(), line, m_machinePriority);
    waitUntilDone();
    
    if (m_doPerLineAfter)
    {
        int r = m_ncMachine->getModbus()->getTaskLastRet(m_machinePriority);
		sink->insert("modubs_ret", r);
        m_doPerLineAfter(sink, line);
    }

    if (checkContinue) {
        if (!m_taskToContinue) {
            if (!m_exceptionThrown) {
                m_exceptionThrown = true;
                throw GCodeInterruptException(EUtils::QString2StdString("User Cancelled"));
            }
            else {
                return;
            }
        }
    }
}

void RealtimeJsonMachine::waitUntilDone() const
{
    while (m_taskToContinue)
    {
        if (m_ncMachine->getTaskCnt(m_machinePriority) > 0)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(TASK_WAIT_MILLSECONDS));
        }
        else
        {
            break;
        }
    }
}

void RealtimeJsonMachine::start() {
    m_exceptionThrown = false;
    filename.clear();
    line = -1;
    axisSeen = 0;

    MachineAdapter::start();

    if (!simpleMode) {
        //sink.appendDict(true);
        //sink.insert("type", "units");
        //sink.insert("value", units.toString());
        //sink.endDict();

        cb::JSON::ValuePtr sink = JSON::Factory().createDict();
        sink->insert("type", "api");
        doTask(sink, false);
    }
}


void RealtimeJsonMachine::end() {
    MachineAdapter::end();

    if (!simpleMode)
    {
        {
            char selectedAxis = '0';
            double selectedPos = 0;
            JSON::ValuePtr value = JSON::Factory().createDict();
            value->insert("code", "G25");
            value->insert("type", "ajc");
            value->insert("axis", selectedAxis);
            value->insert("pos", selectedPos);

            cb::JSON::ValuePtr sink = JSON::Factory().createDict();
            sink->insert("type", "custom");
            //sink->insert("value", &value);
            sink->insert("value", value);

            doTask(sink, false);
        }

        {
            cb::JSON::ValuePtr sink = JSON::Factory().createDict();
            sink->insert("type", "jog");
            doTask(sink, false);
        }
    }

    
}


void RealtimeJsonMachine::setFeed(double feed) {
    double oldFeed = getFeed();
    MachineAdapter::setFeed(feed);

    if (feed != oldFeed) {
        //sink.appendDict(true);
        //sink.insert("type", "feed");
        //sink.insert("value", feed);
        //sink.endDict();
    }
}


void RealtimeJsonMachine::setFeedMode(feed_mode_t mode) {
    feed_mode_t oldMode = getFeedMode();
    MachineAdapter::setFeedMode(mode);

    if (oldMode != mode) {
        //sink.appendDict(true);
        //sink.insert("type", "feed-mode");
        //sink.insert("value", MachineEnum::toString(mode));
        //sink.endDict();
    }
}


void RealtimeJsonMachine::setSpeed(double speed) {
    double oldSpeed = getSpeed();
    MachineAdapter::setSpeed(speed);

    if (oldSpeed != speed) {
        //sink.appendDict(true);
        //sink.insert("type", "speed");
        //sink.insert("value", speed);
        //sink.endDict();
    }
}


void RealtimeJsonMachine::setSpinMode(spin_mode_t mode, double max) {
    double oldMax = 0;
    spin_mode_t oldMode = getSpinMode(&oldMax);
    MachineAdapter::setSpinMode(mode, max);

    if (oldMode != mode || (oldMax != max && mode == CONSTANT_SURFACE_SPEED)) {
        //sink.appendDict(true);
        //sink.insert("type", "spin-mode");
        //sink.insert("value", MachineEnum::toString(mode));
        //if (mode == CONSTANT_SURFACE_SPEED && max) sink.insert("max", max);
        //sink.endDict();
    }
}


void RealtimeJsonMachine::setPathMode(path_mode_t mode, double motionBlending,
    double naiveCAM) {
    MachineAdapter::setPathMode(mode, motionBlending, naiveCAM);

 /*   sink.appendDict(true);
    sink.insert("type", "path-mode");
    sink.insert("value", MachineEnum::toString(mode));

    if (mode == CONTINUOUS_MODE) {
        if (0 < motionBlending) sink.insert("blend", motionBlending);
        if (0 < naiveCAM) sink.insert("naive-cam", naiveCAM);
    }

    sink.endDict();*/
}


void RealtimeJsonMachine::changeTool(unsigned tool) {
    unsigned currentTool = get(TOOL_NUMBER, NO_UNITS);
    MachineAdapter::changeTool(tool);

    if (tool != currentTool) {
        cb::JSON::ValuePtr sink = JSON::Factory().createDict();
        sink->insert("type", "tool");
        sink->insert("value", tool);

        doTask(sink);
    }
}


void RealtimeJsonMachine::input(port_t port, input_mode_t mode, double timeout) {
    MachineAdapter::input(port, mode, timeout);

    //sink.appendDict(true);
    //sink.insert("type", "input");
    //sink.insert("port", MachineEnum::toString(port));
    //sink.insert("mode", MachineEnum::toString(mode));
    //sink.insert("timeout", timeout);
    //sink.endDict();
}


void RealtimeJsonMachine::seek(port_t port, bool active, bool error) {
    MachineAdapter::seek(port, active, error);

    //sink.appendDict(true);
    //sink.insert("type", "seek");
    //sink.insert("port", MachineEnum::toString(port));
    //sink.insertBoolean("active", active);
    //sink.insertBoolean("error", error);
    //sink.endDict();
}


void RealtimeJsonMachine::output(port_t port, double value) {
    MachineAdapter::output(port, value);

    //sink.appendDict(true);
    //sink.insert("type", "output");
    //sink.insert("port", MachineEnum::toString(port));
    //sink.insert("value", value);
    //sink.endDict();
}

void RealtimeJsonMachine::setPosition(const Axes& position)
{
    MachineAdapter::setPosition(position);
    this->position = getTransforms().transform(position);
}


void RealtimeJsonMachine::dwell(double seconds) {
    MachineAdapter::dwell(seconds);

    cb::JSON::ValuePtr sink = JSON::Factory().createDict();
    sink->insert("type", "dwell");
    sink->insert("seconds", seconds);

    doTask(sink);
}


void RealtimeJsonMachine::move(const Axes& _target, int axes, bool rapid, double time) {
    MachineAdapter::move(_target, axes, rapid, time);

    bool imperial = units == Units::IMPERIAL;
    Axes target = getTransforms().transform(_target);

    cb::JSON::ValuePtr sink = JSON::Factory().createDict();
    sink->insert("type", "move");
    if (rapid) sink->insertBoolean("rapid", true);

    sink->insertDict("to");

    bool hasAxisData = false;
	/*
    int axisLen = 0;
    for (const char* axis = Axes::AXES; *axis; axis++) {
        if (rapid && axisLen >= LOC_AXIS_LEN)
            break;
        if (!rapid && axisLen >= SPK_AXIS_LEN)
            break;

        if (strchr(ALL_AXES, *axis) == nullptr) {
            continue;
        }

        axisLen++;
        if (axes & MachineEnum::getVarType(*axis)) {
            if (!rapid) {
            }

            sink->getDict("to").insert(std::string(1, tolower(*axis)), target.get(*axis));
            hasAxisData = true;
        }
    }*/
    int axisLen = 0;
    for (const char* axis = Axes::AXES; *axis; axis++) {
        if (strchr(ALL_AXES, *axis) == nullptr) {
            continue;
        }
        if (rapid && axisLen >= LOC_AXIS_LEN)
            break;
        if (!rapid && axisLen >= SPK_AXIS_LEN)
            break;
        axisLen++;

        int axisVT = getVarType(*axis);
        bool wasSeen = axisSeen & axisVT;

        if (!wasSeen && !(axisVT & axes)) continue;

        axisSeen |= axisVT;

        string last = dtos(position.get(*axis), imperial).toString();
        string next = dtos(target.get(*axis), imperial).toString();

        // Always output axis the first time
        // SPK 可以相同坐标继续加工
        if (rapid) {
            if (wasSeen && last == next) continue;
        }

        sink->getDict("to").insert(std::string(1, tolower(*axis)), target.get(*axis));
        hasAxisData = true;
    }

    if (time) sink->insert("time", time);

    if (hasAxisData) {
        position = target;
        doTask(sink);
    }
    else {
		LOG_INFO(8, "EDM: RealtimeJsonMachine::move: no axis data to move, position and target is same. " << position);
    }
}


void RealtimeJsonMachine::arc(const Vector3D& offset, const Vector3D& target,
    double angle, plane_t plane) {
    MachineAdapter::arc(offset, target, angle, plane);

    //sink.appendDict(true);
    //sink.insert("type", "arc");
    //sink.beginInsert("offset");
    //offset.write(sink);
    //sink.beginInsert("to");
    //target.write(sink);
    //sink.insert("angle", angle);
    //sink.insert("plane", MachineEnum::toString(plane));
    //sink.endDict();
}


void RealtimeJsonMachine::pause(pause_t type) {
    MachineAdapter::pause(type);

    cb::JSON::ValuePtr sink = JSON::Factory().createDict();
    sink->insert("type", "pause");
    if (type == PAUSE_OPTIONAL) sink->insertBoolean("optional", true);
    if (type == PAUSE_PALLET_CHANGE) sink->insertBoolean("pallet-change", true);
    doTask(sink);
}


void RealtimeJsonMachine::setLocation(const cb::LocationRange& location) {
    MachineAdapter::setLocation(location);

    if (!withLocation) return;

    if (location.getStart().getFilename() != filename) {
        filename = location.getStart().getFilename();
    }

    if (location.getStart().getLine() != line) {
        line = location.getStart().getLine();
    }
}


void RealtimeJsonMachine::comment(const std::string& s) const {
    MachineAdapter::comment(s);
    LOG_INFO(8, "EDM: RealtimeJsonMachine::comment: " << s);

    //cb::JSON::ValuePtr sink = JSON::Factory().createDict();
    //sink->insert("type", "comment");
    //sink->insert("value", s);

    //doTask(sink);
}


void RealtimeJsonMachine::message(const std::string& s) {
    MachineAdapter::message(s);
    //std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::toupper(c); });

    LOG_INFO(8, "EDM: RealtimeJsonMachine::message: " << s);

    QString s2 = QString::fromStdString(s);
	// dont toUpper because of "do_message_yn" like things
    //s2 = s2.toUpper();
    if (s2.startsWith("LEJL") || s2.startsWith("LEJS") || s2.startsWith("MDIV") || s2.startsWith("AOD")) {
        cb::JSON::ValuePtr sink = JSON::Factory().createDict();
        sink->insert("type", "custom");

        JSON::ValuePtr value = JSON::Factory().createDict();
        value->insert("type", "setc2");
        if (s2.startsWith("LEJL")) {
            value->insert("v", s2.mid(4).toDouble());
            value->insert("n", "LEJL");
        }
        else if (s2.startsWith("LEJS")) {
			value->insert("v", s2.mid(4).toDouble());
			value->insert("n", "LEJS");
		}
        else if (s2.startsWith("MDIV")) {
			value->insert("v", s2.mid(4).toDouble());
			value->insert("n", "MDIV");
		}
        else if (s2.startsWith("AOD")) {
			value->insert("v", s2.mid(3).toDouble());
			value->insert("n", "AOD");
		}
        sink->insert("value", value);

        doTask(sink);
        return;
    }

    else if (s2.startsWith("do_")) {
        cb::JSON::ValuePtr sink = JSON::Factory().createDict();
        if (s2 == "do_api") {
            sink->insert("type", "api");
        }
        else if (s2 == "do_jog") {
            sink->insert("type", "jog");
        }
        else if (s2.startsWith("do_fromlimit")) {
            sink->insert("type", "custom");
            JSON::ValuePtr value = JSON::Factory().createDict();
            value->insert("type", "fromlimit");
            int idx = 12;
            if (s2.mid(idx, 1) == "n") {
                value->insert("axis", s2.mid(idx + 1).trimmed().toUpper().at(0).toLatin1());
                value->insert("pos", -1);
            }
            else {
                value->insert("axis", s2.mid(idx).trimmed().toUpper().at(0).toLatin1());
                value->insert("pos", 1);
            }
            sink->insert("value", value);
        }
        else if (s2.startsWith("do_tolimit")) {
            sink->insert("type", "custom");
            JSON::ValuePtr value = JSON::Factory().createDict();
            value->insert("type", "tolimit");
            int idx = 10;
            if (s2.mid(idx, 1) == "n") {
                value->insert("axis", s2.mid(idx + 1).trimmed().toUpper().at(0).toLatin1());
                value->insert("pos", -1);
            }
            else {
                value->insert("axis", s2.mid(idx).trimmed().toUpper().at(0).toLatin1());
                value->insert("pos", 1);
            }
            sink->insert("value", value);
        }
        else if (s2.startsWith("do_tozsig")) {
            sink->insert("type", "custom");
            JSON::ValuePtr value = JSON::Factory().createDict();
            value->insert("type", "tozsig");
            int idx = 9;
            if (s2.mid(idx, 1) == "n") {
                value->insert("axis", s2.mid(idx + 1).trimmed().toUpper().at(0).toLatin1());
                value->insert("pos", -1);
            }
            else {
                value->insert("axis", s2.mid(idx).trimmed().toUpper().at(0).toLatin1());
                value->insert("pos", 1);
            }
            sink->insert("value", value);
        }
        else if (s2.startsWith("do_backtozero")) {
            sink->insert("type", "custom");
            JSON::ValuePtr value = JSON::Factory().createDict();
            value->insert("type", "backtozero");
            int idx = 13;
            if (s2.mid(idx, 1) == "n") {
                value->insert("axis", s2.mid(idx + 1).trimmed().toUpper().at(0).toLatin1());
                value->insert("pos", -1);
            }
            else {
                value->insert("axis", s2.mid(idx).trimmed().toUpper().at(0).toLatin1());
                value->insert("pos", 1);
            }
            sink->insert("value", value);
        }
        else if (s2.startsWith("do_zeroaxis")) {
            sink->insert("type", "custom");
            JSON::ValuePtr value = JSON::Factory().createDict();
            value->insert("type", "zeroaxis");
            int idx = 11;
            value->insert("axis", s2.mid(idx).trimmed().at(0).digitValue());
            sink->insert("value", value);
        }
        else if (s2.startsWith("do_message_yn")) {
            sink->insert("type", "custom");

            JSON::ValuePtr value = JSON::Factory().createDict();
            value->insert("type", "message_yn");
            value->insert("n", s2.mid(13).trimmed().toStdString());
            sink->insert("value", value);
        }
        else if (s2.startsWith("do_call_ui")) {
            if (m_doWhenCallUI != NULL) {
				//QMetaObject::invokeMethod(m_doWhenCallUI, "doWhenCallUI", Qt::QueuedConnection, Q_ARG(QString, ""));
                m_doWhenCallUI(s.substr(10));
            }
        }
        else if (s2.startsWith("do_beep")) {
            sink->insert("type", "custom");

            JSON::ValuePtr value = JSON::Factory().createDict();
            value->insert("type", "beep");
            value->insert("ms", s2.mid(7).toStdString());
            sink->insert("value", value);
        }
        else if (s2.startsWith("do_light")) {
            sink->insert("type", "custom");

            JSON::ValuePtr value = JSON::Factory().createDict();
            value->insert("type", "light");
            value->insert("v", s2.mid(8).toStdString());
            sink->insert("value", value);
        }
        else if (s2.startsWith("do_jogspeed")) {
            sink->insert("type", "custom");

            JSON::ValuePtr value = JSON::Factory().createDict();
            value->insert("type", "jogspeed");
            value->insert("v", s2.mid(11).toStdString());
            sink->insert("value", value);
        }
        else {
            QStringList ss = s2.split(' ');
            JSON::ValuePtr value = JSON::Factory().createDict();
            value->insert("type", ss[0].mid(3).toStdString());
            value->insert("s", ss[1].toStdString());

            sink->insert("type", "custom");
            sink->insert("value", value);
        }
        if (sink->exists("type")) {
            doTask(sink);
        }
		return;
    }
    else {
        NCMachineParametersC& cInst = NCMachineParametersC::instance();
        for (const std::string& si : cInst.names) {
			QString s3 = QString::fromStdString(si);
            if (s2.startsWith(s3)) {
                cb::JSON::ValuePtr sink = JSON::Factory().createDict();
                sink->insert("type", "custom");
                JSON::ValuePtr value = JSON::Factory().createDict();
                value->insert("type", "setc2");
                value->insert("v", s.substr(s3.length()));
                value->insert("n", si);

                sink->insert("value", value);
                doTask(sink);
                break;
            }
        }
    }
    
    m_lastMessage = s;
}

void RealtimeJsonMachine::custom(const cb::JSON::ValuePtr& value) {
    MachineAdapter::custom(value);

    cb::JSON::ValuePtr sink = JSON::Factory().createDict();
    sink->insert("type", "custom");
    //sink->insert("value", &value);
    sink->insert("value", value);

    doTask(sink);
}


