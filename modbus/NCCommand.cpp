#include <cassert>
#include <iostream>
#include <sstream>
#include <QString>
#include "NCCommand.h"
#include "NCMachineDefines.h"


std::string NCCommand::UIntsToString(std::vector<uint16_t> vs)
{
	std::string result;
	for (auto it = vs.begin(); it != vs.end(); ++it) {
		uint16_t n = *it;
		char buf[4];
		std::sprintf(buf, "%04X", n);
		std::string s = buf;
		s = std::string(4 - s.length(), '0') + s;
		result += s + " ";
	}
	if (!result.empty()) {
		result.pop_back(); // remove last space
	}
	return result;
}

std::string NCCommand::GetReadString(int address, int quantity, std::string action)
{
	std::ostringstream stringStream;
	stringStream << "{" << "\"action\" :\"" << action << "\", \"address\":" << address << "\", \"quantity\":" << quantity << "}";
	return stringStream.str();
}
std::string NCCommand::GetWriteString(int address, int quantity, std::vector<uint16_t> vs, std::string action)
{
	assert(vs.size() == quantity);

	auto v2 = NCCommand::UIntsToString(vs);

	std::string s01 = R"({
        "action": ")";
	std::string s02 = R"(",
        "address" : )";

	std::string s1 = R"(,
        "quantity" : )";
	std::string s2 = R"(,
        "data" : ")";
	std::string s3 = "\"}";
	std::ostringstream stringStream;
	stringStream << s01 << action << s02 << address << s1 << quantity << s2 << v2 << s3;

	return stringStream.str();
}

const std::string NCCommand::DisableAll = NCCommand::GetWriteString(TMBS_MAP0_ID_EXIT, TMBS_MAP0_ID_EXIT_LEN, { 0xFF });
const std::string NCCommand::Shutdown = NCCommand::GetWriteString(TMBS_MAP0_ID_MCUCTRL, TMBS_MAP0_ID_MCUCTRL_LEN, { 0x3F });