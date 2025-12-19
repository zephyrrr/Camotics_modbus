#pragma once
#include <QString>
#include <QObject>

class GCodeUtils
{
private:
	GCodeUtils() {}

public:
	static QString CleanGCode(QString gcode);
	static QString Duidao(QString axisName, double axisValue, int doCnt = 0, double ddgchtz = 0, double inJieChuHouFanZhuang = 0);
	static QString Duanmian(double inX, double inY, double inZ, double inU, double inJieChuHouFanZhuang, bool inSetToZero, int doCnt = 0, double ddgchtz = 0);
	static QString Zhuzhongxing(double inKuaiJingX, double inKuaiJingY, double inKuaiJingZ, double inJieChuHouFanZhuang, bool inSetToZero, bool inTouchSenseZ, int doCnt = 0, double ddgchtz = 0);
	static QString Kongzhongxing(double inKuaiJingX, double inKuaiJingY, double inKuaiJingZ, double inJieChuHouFanZhuang, bool inSetToZero, bool inTouchSenseZ, int doCnt = 0, double ddgchtz = 0);
	static QString Jiaodingwei(int dX, int dY, double inKuaiJingX, double inKuaiJingY, double inKuaiJingZ, double inJieChuHouFanZhuang, bool inSetToZero, bool inTouchSenseZ, int doCnt = 0, double ddgchtz = 0);
	static QString Sandiandingwei(double inKuaiJingR, double inKuaiJingZ, double inKuaiJingA1, double inKuaiJingA2, double inKuaiJingA3, double inJieChuHouFanZhuang, bool inWaiJing, int doCnt = 0, double ddgchtz = 0);
	static QString RunManual();
private:
	static QString subDuidao;
};

class GCodeUtilsWrapper : public QObject {

	Q_OBJECT

public Q_SLOTS:
	QString static_GCodeUtils_Duanmian(double inX, double inY, double inZ, double inU, double inJieChuHouFanZhuang, bool inSetToZero, int doCnt, double ddgchtz) { return GCodeUtils::Duanmian(inX, inY, inZ, inU, inJieChuHouFanZhuang, inSetToZero, doCnt, ddgchtz); }
};