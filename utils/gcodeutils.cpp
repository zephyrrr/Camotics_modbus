#include "gcodeutils.h"
#include <QStringList>
#include <QStringBuilder>
#include "utils/dataform.h"
#include "utils/MathTool.h"
#include "utils/DataTable.h"
#include "../modbus/NCMachineProperties.h"
#include "modbus/NCMachineParametersC.h"
#include "widgets/qlineedit4axisvalue.h"
#include "modbus/eutils.h"
// Todo: arg(v, 0, 'f', 3)

// (debug, #<total_v>)
//  "o900 call [doCnt] [1|2|11|22|21|22] [ddgchtz]\n" //"G80 Z-\n"
QString GCodeUtils::subDuidao = QString(R"(
o900 sub
    G91
    #<total_v> = 0

    #<while_i> = 0
    o901 while [#<while_i> LT #1]
        #<back_v> = #3
        #<while_i> = [#<while_i> + 1]
        o903 if [#<while_i> EQ #1]
			#<back_v> = 0
		o903 endif
        o902 if [#2 EQ 1]
            G80 X+
            #<total_v> = #<total_v> + #<_x>
            M05 G00 X[-#<back_v>]
        o902 elseif [#2 EQ 2]
            G80 X-
            #<total_v> = #<total_v> + #<_x>
            M05 G00 X[#<back_v>]
		o902 elseif [#2 EQ 11]
            G80 Y+
			#<total_v> = #<total_v> + #<_y>
			M05 G00 Y[-#<back_v>]
		o902 elseif [#2 EQ 12]
            G80 Y-  
			#<total_v> = #<total_v> + #<_y>
			M05 G00 Y[#<back_v>]
		o902 elseif [#2 EQ 21]
			G80 Z+
			#<total_v> = #<total_v> + #<_z>
			M05 G00 Z[-#<back_v>]
		o902 elseif [#2 EQ 22]
            G80 Z-
			#<total_v> = #<total_v> + #<_z>
			M05 G00 Z[#<back_v>]
        o902 endif
    o901 endwhile
    G90
    o900 return [#<total_v> / #1]
o900 endsub
)");

//QString GCodeUtils::Duidao(QString axisName, double axisValue, int doCnt, double ddgchtz)
//{
//    QString gcode = subDuidao;
//
//    int subDuidaoParam2 = 0;
//    if (axisName == "X") {
//		subDuidaoParam2 = axisValue > 0 ? 1 : 2;
//	}
//    else if (axisName == "Y") {
//		subDuidaoParam2 = axisValue > 0 ? 11 : 12;
//	}
//    else if (axisName == "Z") {
//		subDuidaoParam2 = axisValue > 0 ? 21 : 22;
//	}
//    gcode += QString("o900 call [%01] [%02] [%03]")
//.arg(doCnt)
//.arg(subDuidaoParam2)
//.arg(ddgchtz);
//    return gcode;
//    //gcode += QString("o103 repeat[%01]\n").arg(doCnt);
//
//    //gcode += QString("G80 %01%02\n").arg(axisI).arg(axisValues[k] > 0 ? "+" : "-");
//    //gcode += QString("G83 %01011\n").arg(axisI);
//    //gcode += QString("H012 = H012 + H011\n");
//    //gcode += QString("G91\nM05 G00 %01%02\nG90\n").arg(axisI).arg(ddgchtz * (axisValues[k] > 0 ? -1 : 1));
//    ////gcode += QString("(debug, #<_H12>)\n");
//    //gcode += QString("o103 endrepeat\n");
//}

QString GCodeUtils::CleanGCode(QString gcode)
{
    // 1. Split by newline (handles both \n and \r\n)
    QStringList lines = gcode.split(QRegularExpression("[\r\n]+"), Qt::KeepEmptyParts);
    QStringList filteredLines;

    bool addCommaAtEnd = false;
    DataForm* dataForm = DataForms::getInstance()->getDataForm("xitongshezhi5");
    if (dataForm->getValue("GDMMWZJFH") == "1") {
        addCommaAtEnd = true;
    }

    for (const QString& line : lines) {
        // Trim spaces to ensure a match even if there's leading whitespace
        if (line.trimmed().startsWith("(debug, ", Qt::CaseInsensitive)) {
        }
        else if (line.trimmed().startsWith("do_", Qt::CaseInsensitive)) {
        }
        else {
            if (line.isEmpty()) {
                filteredLines << line;
            }
            else {
                filteredLines << line + (addCommaAtEnd ? ";" : ""); // Keep original line (even if it was already empty)
            }
        }
    }

    
    // 3. Join back into a single multiline string
    return filteredLines.join("\n");
}

QString GCodeUtils::Duidao(QString axisName, double axisValue, int doCnt, double ddgchtz, double inJieChuHouFanZhuang)
{
    if (doCnt <= 0) {
        DataForm* dataForm = DataForms::getInstance()->getDataForm("xitongshezhi3");
        doCnt = dataForm->getValue("DDCS").toInt();
        if (doCnt <= 0)
            doCnt = 1;
        ddgchtz = dataForm->getValue("DDGCZDHTJL").toDouble() / 1000.0;
    }
    QString gcode = subDuidao;
    int subDuidaoParam2 = 0;
    if (axisName == "X") {
        subDuidaoParam2 = axisValue > 0 ? 1 : 2;
    }
    else if (axisName == "Y") {
        subDuidaoParam2 = axisValue > 0 ? 11 : 12;
    }
    else if (axisName == "Z") {
        subDuidaoParam2 = axisValue > 0 ? 21 : 22;
    }
    gcode += QString("o900 call [%01] [%02] [%03]\n")
        .arg(doCnt)
        .arg(subDuidaoParam2)
        .arg(ddgchtz, 0, 'f', 3);
    gcode += QString("M05 G90 G00 %01 #<_value>\n").arg(axisName);

    if (inJieChuHouFanZhuang != 0) {
        gcode += QString("M05 G91 G00 %01%02\n").arg(axisName).arg(inJieChuHouFanZhuang * (axisValue > 0 ? -1 : 1), 0, 'f', 3);
    }

    return gcode;
}

QString GCodeUtils::Duanmian(double inX, double inY, double inZ, double inU, double inJieChuHouFanZhuang, bool inSetToZero, int doCnt, double ddgchtz)
{
    if (doCnt <= 0) {
        DataForm* dataForm = DataForms::getInstance()->getDataForm("xitongshezhi3");
        doCnt = dataForm->getValue("DDCS").toInt();
        if (doCnt <= 0)
            doCnt = 1;
        ddgchtz = dataForm->getValue("DDGCZDHTJL").toDouble() / 1000.0;
    }

    QString gcode = subDuidao;

    QStringList axis;
    QList<double> axisValues;
    if (inX != 0) {
        axis.append("X");
        axisValues.append(inX);
    }
    if (inY != 0) {
        axis.append("Y");
        axisValues.append(inY);
    }
    if (inZ != 0) {
        axis.append("Z");
        axisValues.append(inZ);
    }
    if (inU != 0) {
        axis.append("U");
        axisValues.append(inU);
    }
    for (int k = 0; k < axis.length(); ++k)
    {
        QString axisName = axis.at(k);
        double axisValue = axisValues.at(k);
        int subDuidaoParam2 = 0;
        if (axisName == "X") {
            subDuidaoParam2 = axisValue > 0 ? 1 : 2;
        }
        else if (axisName == "Y") {
            subDuidaoParam2 = axisValue > 0 ? 11 : 12;
        }
        else if (axisName == "Z") {
            subDuidaoParam2 = axisValue > 0 ? 21 : 22;
        }
        gcode += QString("o900 call [%01] [%02] [%03]\n")
            .arg(doCnt)
            .arg(subDuidaoParam2)
            .arg(ddgchtz, 0, 'f', 3);

        gcode += QString("M05 G90 G00 %01 #<_value>\n").arg(axisName);
        if (inSetToZero) {
            gcode += QString("G92 %01 0\n").arg(axisName);
        }
        if (inJieChuHouFanZhuang != 0) {
            gcode += QString("M05 G91 G00 %01%02\n").arg(axisName).arg(inJieChuHouFanZhuang * (axisValues[k] > 0 ? -1 : 1), 0, 'f', 3);
        }
    }

    //for (int k = 0; k < axis.length(); ++k)
    //{
    //    gcode += QString("H012 = 0\n");
    //    QString axisI = axis.at(k);

    //    gcode += QString("o103 repeat[%01]\n").arg(doCnt);

    //    gcode += QString("G80 %01%02\n").arg(axisI).arg(axisValues[k] > 0 ? "+" : "-");
    //    gcode += QString("G83 %01011\n").arg(axisI);
    //    gcode += QString("H012 = H012 + H011\n");
    //    gcode += QString("G91\nM05 G00 %01%02\nG90\n").arg(axisI).arg(ddgchtz * (axisValues[k] > 0 ? -1 : 1));
    //    //gcode += QString("(debug, #<_H12>)\n");
    //    gcode += QString("o103 endrepeat\n");

    //    gcode += QString("G00 %01[#<_H12>/%02]\n").arg(axisI).arg(doCnt);
    //    if (inSetToZero) {
    //        gcode += QString("G92 %010\n").arg(axisI);
    //    }
    //    if (inJieChuHouFanZhuang != 0) {
    //        gcode += QString("G91\nM05 G00 %01%02\nG90\n").arg(axisI).arg(inJieChuHouFanZhuang * (axisValues[k] > 0 ? -1 : 1));
    //    }
    //}

    return gcode;
}

QString GCodeUtils::Zhuzhongxing(double inKuaiJingX, double inKuaiJingY, double inKuaiJingZ, double inJieChuHouFanZhuang, bool inSetToZero, bool inTouchSenseZ, int doCnt, double ddgchtz)
{
    if (doCnt <= 0) {
        DataForm* dataForm = DataForms::getInstance()->getDataForm("xitongshezhi3");
        doCnt = dataForm->getValue("DDCS").toInt();
        if (doCnt <= 0)
            doCnt = 1;
        ddgchtz = dataForm->getValue("DDGCZDHTJL").toDouble() / 1000.0;
    }

    QString gcode = subDuidao;

    /*
1、记录当前起始点1（对刀开始的位置）
2、X-方向快速进给
3、Z轴快速进给（Z轴下降）
4、记录当前点2（X-方向对刀完成后需要返回到这个位置）
5、开始对刀
6、对刀完成后，快速返回记录点2
7、Z轴回升
8、返回记录点1
*/
    gcode += QString("\no901 sub\n");

    gcode += QString("G91\n");
    if (inTouchSenseZ) {
        gcode += QString(
            "o900 call [%03] [%05] [%04]\n" //"G80 Z-\n"
            "%2\n"
            "M05 G91 G00 Z+%1\n")
            .arg(inJieChuHouFanZhuang, 0, 'f', 3)
            .arg(inSetToZero ? "G92 Z0" : "")
            .arg(doCnt)
            .arg(ddgchtz, 0, 'f', 3)
            .arg("22");;
    };

    char axis[2] = { 'X','Y' };
    char axisSign[2] = { '-','+' };

    for (int i = 0; i < 2; ++i) {
        if (i == 0 && inKuaiJingX == 0)
            continue;
        if (i == 1 && inKuaiJingY == 0)
            continue;

        for (int j = 0; j < 2; ++j) {
            // 1、记录当前起始点1（对刀开始的位置）。H011
            gcode += QString("G83 %01011\n").arg(axis[i]);

            int subDuidaoParam2 = 0;
            if (i == 0) {
                subDuidaoParam2 = j == 0 ? 1 : 2;
            }
            else if (i == 1) {
                subDuidaoParam2 = j == 0 ? 11 : 12;
            }
            QString gcodecall = QString("o900 call [%01] [%02] [%03]\n")
                .arg(doCnt)
                .arg(subDuidaoParam2)
                .arg(ddgchtz, 0, 'f', 3);
            gcodecall += QString("#<_H%01>=#<_value>\n")
                .arg(1 + i * 2 + j);

            // 2,3,4
            gcode += QString(
                "G91 G00 %01%02%03\n"
                "G91 G00 Z-%04\n"
                "G83 %01013\n")
                .arg(axis[i])
                .arg(axisSign[j])
                .arg(i == 0 ? inKuaiJingX : inKuaiJingY)
                .arg(inKuaiJingZ, 0, 'f', 3);
            // 5
            gcode += gcodecall;
            // 6,7
            gcode += QString("M05 G91 G00 %01%02%03\n"
                "G90 G00 %01H013\n"
                "G91 G00 Z+%4\n")
                .arg(axis[i])
                .arg(axisSign[j])
                .arg(inJieChuHouFanZhuang)
                .arg(inKuaiJingZ, 0, 'f', 3);

            // 8
            if (j == 0) {
                gcode += QString("G90 G00 %01H011\n")
                    .arg(axis[i]);
            }
        }

        if (i == 0)
            gcode += QString("G90 G00 X[[H001+H002]/2]\n");
        else
            gcode += QString("G90 G00 Y[[H003+H004]/2]\n");
    }

    // 只对X/Y操作，Z不操作（若有，在前面）
    // 在外面，可能有多次，多次取平均后再G92
    //if (inSetToZero) {
    //    gcode += QString("G92 X0Y0\n");
    //}
    gcode += QString(
        "H009 = [[H001+H002]/2]\n"
        "H007 = [[H003+H004]/2]\n");
    gcode += inKuaiJingX != 0 ? "H008 = [[H002-H001]/2]\n" : "H008 = 0\n";
    gcode += inKuaiJingY != 0 ? "H010 = [[H004-H003]/2]\n" : "H010 = 0\n";

    gcode += QString("(debug, do_call_ui #<_H9>,#<_H7>,#<_H8>,#<_H10>)\n"
        "G90\n");

    //gcode = "G80 X+\nG83 X001\n(debug, #001)\n";
    //gcode += "(debug, #001,#002)\n";

    gcode += QString("o901 endsub\n");

    return gcode;
}

QString GCodeUtils::Kongzhongxing(double inKuaiJingX, double inKuaiJingY, double inKuaiJingZ, double inJieChuHouFanZhuang, bool inSetToZero, bool inTouchSenseZ, int doCnt, double ddgchtz)
{
    if (doCnt <= 0) {
        DataForm* dataForm = DataForms::getInstance()->getDataForm("xitongshezhi3");
        doCnt = dataForm->getValue("DDCS").toInt();
        if (doCnt <= 0)
            doCnt = 1;
        ddgchtz = dataForm->getValue("DDGCZDHTJL").toDouble() / 1000.0;
    }

    QString gcode = subDuidao;
    gcode += QString("\no902 sub\n");

    gcode += QString("G91\n");
    if (inTouchSenseZ) {
        gcode += QString(
            "o900 call [%03] [%05] [%04]\n" //"G80 Z-\n"
            "%2\n"
            "M05 G91 G00 Z+%1\n")
            .arg(inJieChuHouFanZhuang, 0, 'f', 3)
            .arg(inSetToZero ? "G92 Z0" : "")
            .arg(doCnt)
            .arg(ddgchtz, 0, 'f', 3)
            .arg("22");;
    };

    gcode += QString("G91 G00 Z-%1\n").arg(inKuaiJingZ, 0, 'f', 3);

    if (inKuaiJingX != 0) {
        gcode += QString(
            "G83 X011\n"

            "G91 G00 X-%2\n"
            "o900 call [%3] [2] [%4]\n"//"G80 X-\n"
            "#<_H1>=#<_value>\n"//"G83 X001\n"
            //"M05 G91 G00 X+%1\n"  // 忽略，直接返回到初始点

            "M05 G90 G00 XH011\n"

            "G91 G00 X+%2\n"
            "o900 call [%3] [1] [%4]\n"//"G80 X+\n"
            "#<_H2>=#<_value>\n"//"G83 X002\n"
            "M05 G91 G00 X-%1\n"
        )
            .arg(inJieChuHouFanZhuang, 0, 'f', 3)
            .arg(inKuaiJingX, 0, 'f', 3)
            .arg(doCnt)
            .arg(ddgchtz, 0, 'f', 3);

        gcode += QString("G90 G00 X[[H001+H002]/2]\n");
    }

    if (inKuaiJingY != 0) {
        gcode += QString(
            "G83 Y012\n"

            "G91 G00 Y-%2\n"
            "o900 call [%3] [12] [%4]\n"//"G80 Y-\n"
            "#<_H3>=#<_value>\n"//"G83 Y003\n"
            //"M05 G91 G00 Y+%1\n" // 忽略，直接返回到初始点

            "M05 G90 G00 YH012\n"

            "G91 G00 Y+%2\n"
            "o900 call [%3] [11] [%4]\n"//"G80 Y+\n"
            "#<_H4>=#<_value>\n"//"G83 Y004\n"
            "M05 G91 G00 Y-%1\n"
        )
            .arg(inJieChuHouFanZhuang, 0, 'f', 3)
            .arg(inKuaiJingY, 0, 'f', 3)
            .arg(doCnt)
            .arg(ddgchtz, 0, 'f', 3);

        gcode += QString("G90 G00 Y[[H003+H004]/2]\n");
    }

    gcode += QString("G91 G00 Z+%1\n").arg(inKuaiJingZ, 0, 'f', 3);

    //if (inSetToZero) {
    //    gcode += QString("G92 X0Y0\n");
    //}

    gcode += QString(
        "H009 = [[H001+H002]/2]\n"
        "H007 = [[H003+H004]/2]\n");
    gcode += inKuaiJingX != 0 ? "H008 = [[H002-H001]/2]\n" : "H008 = 0\n";
    gcode += inKuaiJingY != 0 ? "H010 = [[H004-H003]/2]\n" : "H010 = 0\n";
    gcode += QString("(debug, do_call_ui #<_H9>,#<_H7>,#<_H8>,#<_H10>)\n"
        "G90\n");

    gcode += QString("o902 endsub\n");

    return gcode;
}

QString GCodeUtils::Jiaodingwei(int dX, int dY, double inKuaiJingX, double inKuaiJingY, double inKuaiJingZ, double inJieChuHouFanZhuang, bool inSetToZero, bool inTouchSenseZ, int doCnt, double ddgchtz)
{
    if (doCnt <= 0) {
        DataForm* dataForm = DataForms::getInstance()->getDataForm("xitongshezhi3");
        doCnt = dataForm->getValue("DDCS").toInt();
        if (doCnt <= 0)
            doCnt = 1;
        ddgchtz = dataForm->getValue("DDGCZDHTJL").toDouble() / 1000.0;
    }

    QString gcode = subDuidao;
    gcode += QString("\no903 sub\n");

    gcode += QString("G91\n");
    if (inTouchSenseZ) {
        gcode += QString(
            "o900 call [%03] [%05] [%04]\n" //"G80 Z-\n"
            "%2\n"
            "M05 G91 G00 Z+%1\n")
            .arg(inJieChuHouFanZhuang, 0, 'f', 3)
            .arg(inSetToZero ? "G92 Z0" : "")
            .arg(doCnt)
            .arg(ddgchtz, 0, 'f', 3)
            .arg("22");
    };

    char dn[2] = { '+', '-' };
    gcode += QString(
        "G83 X011\n"
        "G83 Y012\n"

        "G91 G00 X%2\n"
        "G91 G00 Z-%3\n"
        "G83 X013\n"
        "o900 call [%04] [%06] [%05]\n"//"G80 X-%4\n"
        "#<_H1>=#<_value>\n"//"G83 X001\n"
        "M05 G91 G00 X%07%01\n"
        "G90 G00 XH013\n"
        "G91 G00 Z+%3\n"
        "G90 G00 X[H011]Y[H012]\n"
    )
        .arg(inJieChuHouFanZhuang, 0, 'f', 3)
        .arg(inKuaiJingX, 0, 'f', 3)
        .arg(inKuaiJingZ, 0, 'f', 3)
        .arg(doCnt)
        .arg(ddgchtz, 0, 'f', 3)
        .arg(dX == 1 ? "2" : "1")
        .arg(dn[dX]);

    gcode += QString(
        "G91 G00 Y%2\n"
        "G91 G00 Z-%3\n"
        "G83 Y014\n"
        "o900 call [%04] [%06] [%05]\n"//"G80 Y-%4\n"
        "#<_H2>=#<_value>\n"//"G83 Y002\n"
        "M05 G91 G00 Y%07%01\n"
        "G90 G00 YH014\n"
        "G91 G00 Z+%3\n"
        "G90 G00 X[H011]Y[H012]\n"
    )
        .arg(inJieChuHouFanZhuang, 0, 'f', 3)
        .arg(inKuaiJingY, 0, 'f', 3)
        .arg(inKuaiJingZ, 0, 'f', 3)
        .arg(doCnt)
        .arg(ddgchtz, 0, 'f', 3)
        .arg(dY == 1 ? "12" : "11")
        .arg(dn[dY]);

    //if (inSetToZero) {
    //    gcode += QString("G92 X0Y0\n");
    //}
    gcode += QString(
        //"H001 = H001\n"
        //"H002 = H002\n"
        "(debug, do_call_ui #<_H1>,#<_H2>)\n"
        "G90\n");

    gcode += QString("o903 endsub\n");

    return gcode;
}

QString GCodeUtils::Sandiandingwei(double inKuaiJingR, double inKuaiJingZ, double inKuaiJingA1, double inKuaiJingA2, double inKuaiJingA3, double inJieChuHouFanZhuang, bool inWaiJing, int doCnt, double ddgchtz)
{
    double inKuaiJingAs[3] = { inKuaiJingA1 ,inKuaiJingA2, inKuaiJingA3 };

    if (doCnt <= 0) {
        DataForm* dataForm = DataForms::getInstance()->getDataForm("xitongshezhi3");
        doCnt = dataForm->getValue("DDCS").toInt();
        if (doCnt <= 0)
            doCnt = 1;
        ddgchtz = dataForm->getValue("DDGCZDHTJL").toDouble() / 1000.0;
    }

    QString gcode = subDuidao;
    gcode += QString("\no904 sub\n");

    gcode += QString("G91\n");

    if (inWaiJing) {
        
        gcode += QString(
            "G83 X011Y012\n");
        for (int i = 0; i < 3; ++i) {
            double dy = inKuaiJingR * sin(inKuaiJingAs[i] / 180 * cb::Math::PI);
            double dx = inKuaiJingR * cos(inKuaiJingAs[i] / 180 * cb::Math::PI);
            int subDuidaoParam2 = dx < 0 ? 1 : 2;
            gcode += QString(
                "G91 G00 X%01Y%02\n"    // 到快进点
				"G91 G00 Z-%03\n"       // Z轴下降
				"G83 X021Y022\n"        // 记录坐标到H21,H22
                "o900 call [%08] [%09] [%10]\n"
                "#<_H%06>=#<_value>\n"  // H1=对刀点
                "#<_H%07>=#<_H22>\n"   // H2=Y, 没变
                "M05 G91 G00 X%04%05\n" // 回退
				"G90 G00 XH021YH022\n"  // 回到记录点
				"G91 G00 Z+%03\n"       // Z回升
				"G90 G00 XH011YH012\n"  // 回到起始点
            )
                .arg(dx, 0, 'f', 3)
                .arg(dy, 0, 'f', 3)
                .arg(inKuaiJingZ, 0, 'f', 3)
                .arg(dx < 0 ? '-' : '+')
                .arg(inJieChuHouFanZhuang, 0, 'f', 3)
                .arg(1 + 2 * i)
                .arg(2 + 2 * i)
                .arg(doCnt)
                .arg(subDuidaoParam2)
                .arg(ddgchtz, 0, 'f', 3);
        }
    }
    else {
        gcode += QString(
            "G83 X011Y012\n");
        for (int i = 0; i < 3; ++i) {
            double dy = inKuaiJingR * sin(inKuaiJingAs[i] / 180 * cb::Math::PI);
            double dx = inKuaiJingR * cos(inKuaiJingAs[i] / 180 * cb::Math::PI);
            int subDuidaoParam2 = dx < 0 ? 2 : 1;

            gcode += QString(
				"G91 G00 Z-%03\n"       // Z轴下降
				"G91 G00 X%01Y%02\n"    // 到快进点
				"G83 X021Y022\n"        // 记录坐标到H21,H22
                "o900 call [%08] [%09] [%10]\n"
                "#<_H%06>=#<_value>\n"  // H1=对刀点
                "#<_H%07>=#<_H22>\n"   // H2=Y, 没变
				"M05 G91 G00 X%04%05\n"     // 回退
				"G90 G00 XH021YH022\n"      // 回到记录点
                "G90 G00 XH011YH012\n"
                "G91 G00 Z+%03\n"
            )
                .arg(dx, 0, 'f', 3)
                .arg(dy, 0, 'f', 3)
                .arg(inKuaiJingZ, 0, 'f', 3)
                .arg(dx < 0 ? '+' : '-')
                .arg(inJieChuHouFanZhuang, 0, 'f', 3)
                .arg(1 + 2 * i)
                .arg(2 + 2 * i)
                .arg(doCnt)
                .arg(subDuidaoParam2)
                .arg(ddgchtz, 0, 'f', 3);
        }
    }
    gcode += QString(
        "(debug, do_call_ui #<_H1>,#<_H2>,#<_H3>,#<_H4>,#<_H5>,#<_H6>)\n"
    );

    gcode += QString("o904 endsub\n");

    return gcode;
}

//QString GCodeUtils::RunManual()
//{
//    DataTable table11;
//    DataTable table22;
//	DataTable* table1 = &table11;
//	DataTable* table2 = &table22;
//
//    QString filePath2 = SystemSettings::GetDataFilePath("sdjg");
//    filePath2 = SystemSettings::AppendDataFilePath(filePath2, "sdjg_table2");
//    table2->deserialize(filePath2);
//
//    QString filePath1 = SystemSettings::GetDataFilePath("sdjg");
//    filePath1 = SystemSettings::AppendDataFilePath(filePath1, "sdjg_table1");
//    table1->deserialize(filePath1);
//
//    DataForm* dataForm = DataForms::getInstance()->getDataForm("sdjg");
//
//    //
//    QStringList toAxis, toAxisLength;
//    for (int i = 0; i < 3; ++i) {
//        if (table1->getValue(i, -1) != "True") {
//            continue;
//        }
//
//        QString s = table1->getValue(i, 0);
//        if (!s.isEmpty()) {
//            //double d = s.toDouble();
//            toAxis.append(QString("XYZ").at(i));
//            toAxisLength.append(s);
//            break;
//        }
//    }
//    if (toAxis.isEmpty() || toAxis.count() > 3) {
//        return "";
//    }
//
//    QString gcode;
//    for (int i = 0; i < toAxis.size(); ++i) {
//        gcode += QString("H%1   = %2\n").arg(100 + i).arg(toAxisLength[i], 0, 'f', 3);
//    }
//
//    gcode += QString("G11\n");
//
//    //if (ui.inAbsolute->isChecked())
//    if (dataForm->getValue("inAbsolute").toLower() == "true")
//        gcode += "G90\n";
//    else
//        gcode += "G91\n";
//
//    gcode += QString(R"(M98P0000;
//;
//)");
//    gcode += QString(R"(T85;
//M02;
//;
//N0000;
//)");
//
//
//    gcode += "T84;\n";
//    QString cCode;
//    cCode += QString::fromStdString(NCMachineParametersC::GetNamesAsString());
//
//    QHash<int, QString> cNos;
//    for (int i = 0; i < table2->getDataCount(); ++i) {
//        if (!QLineEditLikeButton::IsYes(table2->getValue(i, 0))) {
//            continue;
//        }
//        if (table2->getValue(i, 1).isEmpty()) {
//            continue;
//        }
//
//        if (table2->getValue(i, 6) == "T") {
//            gcode += QString("G85T%1;\n").arg(table2->getValue(i, 7));
//        }
//        else if (table2->getValue(i, 6) == "Z") {
//            gcode += QString("G85Z%1;\n").arg(table2->getValue(i, 7));
//        }
//        QString s;
//        if (table2->getDataYCount() >= 10) {
//            int row = i;
//            int nowcNo = table2->getValue(row, 1).toInt();
//            QString c = table2->getValue(i, 9);
//            if (c.isEmpty()) {
//                // will not happen
//                continue;
//            }
//
//            c = QString::fromStdString(NCMachineParametersC::FormatCCode(EUtils::QString2StdString(c)));
//
//            if (cNos.contains(nowcNo)) {
//                if (cNos[nowcNo] == c) {
//                    // ok, it's same
//                }
//                else {
//                    int nextcNo = nowcNo + 10;
//                    while (true) {
//                        if (!cNos.contains(nextcNo)) {
//                            c = c.replace("C" + QString::number(nowcNo).rightJustified(3, '0') + " = ", "C" + QString::number(nextcNo).rightJustified(3, '0') + " = ");
//                            nowcNo = nextcNo;
//
//                            cCode += c;
//                            cNos[nowcNo] = c;
//                            break;
//                        }
//                        else {
//                            nextcNo += 10;
//                        }
//                    }
//                }
//            }
//            else {
//                cCode += c;
//                cNos[nowcNo] = c;
//            }
//
//            QString gcode3;
//            for (int j = 0; j < toAxis.size(); ++j) {
//                gcode3 += QString("%1H%2+%3 ").arg(toAxis[j]).arg(100 + j).arg(table2->getValue(i, 5));
//            }
//
//            s = QString("C%1 STEP%4 LN%2 LP%3 G01 %5 M04;\n")
//                .arg(QString::number(nowcNo).rightJustified(3, '0'))
//                .arg(table2->getValue(i, 2))
//                .arg(table2->getValue(i, 3))
//                .arg(table2->getValue(i, 4))
//                .arg(gcode3);
//        }
//        else {
//            QString gcode3;
//            for (int j = 0; j < toAxis.size(); ++j) {
//                gcode3 += QString("%1H%2+%3 ").arg(toAxis[j]).arg(100 + j).arg(table2->getValue(i, 5));
//            }
//
//            s = QString("C%1 STEP%4 LN%2 LP%3 G01 %5 M04;\n")
//                .arg(QString::number(table2->getValue(i, 1).toInt()).rightJustified(3, '0'))
//                .arg(table2->getValue(i, 2))
//                .arg(table2->getValue(i, 3))
//                .arg(table2->getValue(i, 4))
//                .arg(gcode3);
//        }
//        gcode += s;
//    }
//    //gcode += QString("G00 %1+0.000;\n").arg(toAxis);
//    gcode += QString("M99; ");
//
//    if (!cCode.isEmpty()) {
//        gcode = cCode + "\n" + gcode;
//    }
//    return gcode;
//}