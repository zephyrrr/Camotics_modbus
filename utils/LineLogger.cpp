#include "LineLogger.h"
#include <QStringList>
#include <QStringBuilder>

LineLogger::LineLogger(Inaccessible)
{
}

void LineLogger::append(const std::string& line) {
    QString s = QString::fromStdString(line);
    append(s);
}
void LineLogger::append(const QString& line) {
    int idx = line.indexOf("ERROR:");
    if (idx != -1) {
        QString s = line.mid(idx + 6);
        if (s.startsWith("unknown")) {
            return;
		}
        if (s.startsWith("std::exception: ")) {
			s = s.mid(16);
		}
		if (!m_newOtherLog) {
            if (errorLines.size() > 0) {
                QString s2 = errorLines[errorLines.size() - 1];
                errorLines.pop_back();
                errorLines.push_back(s2 + ". " + s);
            }
        }
        else {
			m_newOtherLog = false;
            errorLines.push_back(s);
        }
    }
    else
    {
        m_newOtherLog = true;
    }
}
void LineLogger::clear() {
    errorLines.clear();
}

QString LineLogger::getErrors(int n)
{
    //if (n == 1 && m_newOtherLog)
    //    return "";

    QStringList list;
    for (int i = errorLines.size() - 1; i >= 0 && n > 0; --i) {
        list << errorLines[i];
        n--;
    }
    return list.join("\n");
}

//QString LineLogger::getFirstLineOfError()
//{ 
//    return errorLines.size() > 0 ? errorLines[0] : ""; 
//}
