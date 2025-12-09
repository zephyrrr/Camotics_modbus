#ifndef __NCCOMMAND_H
#define __NCCOMMAND_H

#include <string>
#include <vector>

class NCCommand
{
public:
	static const std::string DisableAll;
	static const std::string Shutdown;

	static std::string UIntsToString(std::vector<uint16_t> vs);
	static std::string GetReadString(int address, int quantity, std::string action = "read");
	static std::string GetWriteString(int address, int quantity, std::vector<uint16_t> vs, std::string action = "write");
};



#endif //__NCCOMMAND_H