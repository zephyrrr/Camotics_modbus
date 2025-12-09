#ifndef LINELOGGER_H
#define LINELOGGER_H


#include <string>
#include <vector>
#include <QString>
#include <cbang/util/Singleton.h>

using namespace cb;

class LineLogger : public Singleton<LineLogger>
{
private:
    std::vector<QString> errorLines;
    bool m_newOtherLog = false;
public:
    LineLogger(Inaccessible);
    void append(const std::string& line);
    void append(const QString& line);
    void clear();
    QString getErrors(int n = 1);
    //QString getFirstLineOfError();
};

#endif
