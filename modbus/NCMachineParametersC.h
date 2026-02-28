#ifndef __NCMACHINEDPARAMETERSC_H
#define __NCMACHINEDPARAMETERSC_H

#include "string"
#include "vector"
#include "map"
#include <cbang/util/Singleton.h>
#include <QString>
#include <QRegularExpression>

class NCMachine;

constexpr int PARAMETERC_ROW_COUNT = 1000;
constexpr int PARAMETERC_COLUMN_COUNT = 22;

using namespace cb;

class NCMachineParametersC : public Singleton<NCMachineParametersC>
{
public:
	NCMachineParametersC(Inaccessible);

	bool SetByCIndex(int cIndex);
	std::map<std::string, std::string> GetByCIndex(int cIndex);

	//std::vector<std::string> GetNames() { return std::vector<std::string>(names, names + PARAMETERC_COLUMN_COUNT); }
	//std::vector<std::string> GetNamesRegexValidation() { return std::vector<std::string>(namesRegex, namesRegex + PARAMETERC_COLUMN_COUNT); }
	static std::string GetNamesAsString();
	//static std::string GetCurrentAsString();
	static std::string FormatCCode(std::string cCode);

	void SetValue(int cIndex, int rowIdx, std::string v);

	static bool SetCurrentByName(QString name, QString value);
	static bool SetCurrentByName(std::string name, std::string value);
	static std::string GetCurrentByName(std::string name);
	static int GetCurrentCIndex() { return instance().m_currentCIndex; }

	static void ExecuteCmds(NCMachine* ncMachine);

	bool deserialize();

public:
	// "FM": "^[0-3][0-9][0-9]$"
	// "DS": "\\d{1,9}"
	// "SJ": "^[0-9][0-9][0-9]$", 
	//      ON   OFF  MA  IP   SV UP 
	// DN  LN   STEP  PL V   HP   PP  
	// C   S   L   LP    AL   OC  LD  JM  JS: NO  "SJ"
	inline const static std::string names[PARAMETERC_COLUMN_COUNT] = {"ON", "OFF", "MA", "IP", "SV", "UP" \
		, "DN", "LN", "LP", "STEP", "PL", "V", "HP", "PP" \
		, "C", "S", "L", "AL", "JS", "JM", "OC", "LD" };
	// HP: "^[0-9][0-7][0-3]$", but not ok with input method
	const std::string namesRegex[PARAMETERC_COLUMN_COUNT] = { "^(?:0[0-9]{3}|1[0-9]{3}|2[0-9]{3}|3[0-9]{3}|4000)$", "^(?:0[0-9]{3}|1[0-9]{3}|2[0-4][0-9]{2}|2500)$", "^(?:0[0-9]{3}|1[0-9]{3}|2[0-9]{3}|3[0-9]{3}|4[0-9]{3}|5000)$", "\\d{4}", "^(?:0[0-9]{2}|1[0-7][0-9]|180)$", "^(?:0[0-9]{2}|1[01][0-9]|12[0-7])$" \
		, "^(?:0[0-9]{2}|[1-4][0-9]{2}|5[0][0-9]|510|511)$", "^[0-4][0-2][0-3]$", "^[0-5][0-5][0-5][0-5]$", "\\d{1,9}(\\.\\d+)?", "[+-]", "[0-9][1234]", "^[0-1][0-7][0-7]$", "^[0-5][0-5]$" \
		, "^[0-8]$", "^[0-9][0-9]$", "^[0-2][0-9]$", "^[0-5][0-9]|[6][0-3]$", "\\d{3}", "^[0-1][0-9]$", "^([0-4]\\d{0,3}|5000)$", "\\d{4}" };
	inline const static int namesLen[PARAMETERC_COLUMN_COUNT] = { 4, 4, 4, 4, 3, 3 \
		, 3, 3, 4, -1, 1, 2, 3, 2 \
		, 1, 2, 2, 2, 3, 2, 4, 4 };

	//const char namesChar[PARAMETERC_COLUMN_COUNT] = { 'B', 'F', 'W', 'I', 'R', 'U' \
 //           , 'D', 'K', 'P', 'T', 'Q', 'V', 'J', 'E' \
 //           , '=', 'S', 'L', 'A', '=', '=', '=', '='}; // X,Y,Z,C,G;N,O,M,H
	//const char namesChar[PARAMETERC_COLUMN_COUNT] = { 'B', 'F', 'W', 'I', 'R', 'U' \
	//		, 'D', 'K', 'P', 'T', 'Q', 'V', 'J', 'E' \
	//		, '=', 'S', 'L', 'A', 'X', 'X', 'X', 'X' }; // X,Y,Z,C,G;N,O,M,H
	const char namesChar[PARAMETERC_COLUMN_COUNT] = { '1', '1', '1', '1', '1', '1' \
		, '1', '1', '1', '1', '1', '2', '1', '1' \
		, '=', '2', '2', '1', '1', '1', '1', '1' }; // X,Y,Z,C,G;N,O,M,H

	std::map<std::string, int> namesMap2Index;
	QRegularExpression re4Search[PARAMETERC_COLUMN_COUNT];
	QString re4Replace[PARAMETERC_COLUMN_COUNT];
	QRegularExpression re4Search2[PARAMETERC_COLUMN_COUNT];
private:
	std::string m_allValues[PARAMETERC_ROW_COUNT][PARAMETERC_COLUMN_COUNT];

	std::map<std::string, std::string> m_currentValues;
	int m_currentCIndex = -1;
};

#endif

