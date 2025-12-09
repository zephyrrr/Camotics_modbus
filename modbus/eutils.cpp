#include "eutils.h"
#include <thread>
#include <chrono>
#include <memory>
#include <string>
#include <stdexcept>
#include "libmodbus/modbus.h"

EUtils::EUtils()
{
}

std::string EUtils::ModbusDataTypeName(int fCode)
{
    switch (fCode)
    {
    case MODBUS_FC_READ_COILS:
        return "READ COILS";
    case MODBUS_FC_WRITE_SINGLE_COIL:
        return "WRITE_SINGLE_COIL";
    case MODBUS_FC_WRITE_MULTIPLE_COILS:
        return "WRITE_MULTIPLE_COILS";
    case MODBUS_FC_READ_DISCRETE_INPUTS:
        return "READ_DISCRETE_INPUTS";
    case MODBUS_FC_READ_HOLDING_REGISTERS:
        return "READ_HOLDING_REGISTERS";
    case MODBUS_FC_WRITE_SINGLE_REGISTER:
        return "WRITE_SINGLE_REGISTER";
    case MODBUS_FC_WRITE_MULTIPLE_REGISTERS:
        return "WRITE_MULTIPLE_REGISTERS";
    case MODBUS_FC_READ_INPUT_REGISTERS:
        return "READ_INPUT_REGISTERS";
    case MODBUS_FC_READ_FILE_RECORD:
        return "READ_FILE_RECORD";
    case MODBUS_FC_WRITE_FILE_RECORD:
        return "WRITE_FILE_RECORD";
    default:
        break;
    }
    return "Unknown";
}

bool EUtils::ModbusIsWriteFunction(int fCode)
{
    switch (fCode)
    {
    case MODBUS_FC_READ_COILS:
    case MODBUS_FC_READ_DISCRETE_INPUTS:
    case MODBUS_FC_READ_HOLDING_REGISTERS:
    case MODBUS_FC_READ_INPUT_REGISTERS:
    case MODBUS_FC_READ_FILE_RECORD:
        return false;

    case MODBUS_FC_WRITE_SINGLE_COIL:
    case MODBUS_FC_WRITE_MULTIPLE_COILS:
    case MODBUS_FC_WRITE_SINGLE_REGISTER:
    case MODBUS_FC_WRITE_MULTIPLE_REGISTERS:
    case MODBUS_FC_WRITE_FILE_RECORD:
        return true;

    default:
        break;
    }
    return false;
}

bool EUtils::ModbusIsWriteCoilsFunction(int fCode)
{
    switch (fCode)
    {
    case MODBUS_FC_READ_COILS:
    case MODBUS_FC_READ_DISCRETE_INPUTS:
    case MODBUS_FC_READ_HOLDING_REGISTERS:
    case MODBUS_FC_READ_INPUT_REGISTERS:
    case MODBUS_FC_WRITE_SINGLE_REGISTER:
    case MODBUS_FC_WRITE_MULTIPLE_REGISTERS:
        return false;

    case MODBUS_FC_WRITE_SINGLE_COIL:
    case MODBUS_FC_WRITE_MULTIPLE_COILS:
        return true;

    default:
        break;
    }
    return false;
}

bool EUtils::ModbusIsWriteRegistersFunction(int fCode)
{
    switch (fCode)
    {
    case MODBUS_FC_READ_COILS:
    case MODBUS_FC_READ_DISCRETE_INPUTS:
    case MODBUS_FC_READ_HOLDING_REGISTERS:
    case MODBUS_FC_READ_INPUT_REGISTERS:
    case MODBUS_FC_WRITE_SINGLE_COIL:
    case MODBUS_FC_WRITE_MULTIPLE_COILS:
        return false;

    case MODBUS_FC_WRITE_SINGLE_REGISTER:
    case MODBUS_FC_WRITE_MULTIPLE_REGISTERS:
        return true;

    default:
        break;
    }
    return false;
}

int EUtils::doUntilOk(std::function<int()> function, int& retryCnt) {
	int cnt = 0;
	int ret = -1;
	while (cnt < retryCnt) {
		ret = function();
		if (ret != -1)
			break;
        cnt++;
        if (cnt >= retryCnt)
			break;
		std::this_thread::sleep_for(std::chrono::milliseconds(50 * cnt));
	}
    retryCnt = cnt + 1;
	return ret;
}

void EUtils::sleep(int milliseconds)
{
#ifdef _WIN32
	/* usleep doesn't exist on Windows */
	Sleep(milliseconds);
#else
	/* usleep source code */
	struct timespec request, remaining;
	request.tv_sec = milliseconds / 1000;
	request.tv_nsec = (milliseconds % 1000) * 1000;
	while (nanosleep(&request, &remaining) == -1 && errno == EINTR) {
		request = remaining;
	}
#endif
}

QString EUtils::formatValue(int value, int frmt, bool is16Bit, bool isSigned = false)
{
    QString convertedValue;

    switch (frmt) {

    case 2://Binary
        if (is16Bit)
            if (isSigned)
                convertedValue = QString("%1").arg((signed short)value, 16, 2, QLatin1Char('0'));
            else
                convertedValue = QString("%1").arg((unsigned short)value, 16, 2, QLatin1Char('0'));
        else
            convertedValue = QString("%1").arg(value, 0, 2);
        break;

    case 10://Decimal - Unsigned Integer
        if (isSigned)
            convertedValue = QString("%1").arg((signed short)value, 0, 10);
        else
            convertedValue = QString("%1").arg((unsigned short)value, 0, 10);
        break;

    case 16://Hex
        if (is16Bit)
            convertedValue = QString("%1").arg(value, 4, 16, QLatin1Char('0'));
        else
            convertedValue = QString("%1").arg(value, 0, 16);
        break;

    default://Default
        convertedValue = QString("%1").arg(value, 0, 10);

    }

    return convertedValue.toUpper();
}


QString EUtils::libmodbus_strerror(int errnum)
{
    switch (errnum) {

        case EINVAL:
            return "Protocol context is NULL";
            break;

        case ETIMEDOUT:
            return "Timeout";
            break;

        case ECONNRESET:
            return "Connection reset";
            break;

        case ECONNREFUSED:
            return "Connection refused";
            break;

        case EPIPE:
            return "Socket error";
            break;

        default://Default
            return modbus_strerror(errno);

    }

}


//template<typename ... Args>
//std::string EUtils::string_format(const std::string& format, Args ... args)
//{
//    int size_s = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
//    if (size_s <= 0) { throw std::runtime_error("Error during formatting."); }
//    auto size = static_cast<size_t>(size_s);
//    std::unique_ptr<char[]> buf(new char[size]);
//    std::snprintf(buf.get(), size, format.c_str(), args ...);
//    return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
//}
