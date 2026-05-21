#include "JsonTemplateUtil.h"
#include <QFile>
#include <QRegularExpression>
#include <QDebug>

bool JsonTemplateUtil::loadTemplateFile(const QString& filePath, QString& outTemplateText)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "打开模板文件失败:" << filePath;
        return false;
    }
    outTemplateText = file.readAll();
    file.close();
    return true;
}

QStringList JsonTemplateUtil::extractAllParams(const QString& templateText)
{
    QStringList params;
    // 匹配 {{变量名}}  变量允许字母、数字、下划线
    static const QRegularExpression reg(R"(\{\{\s*([a-zA-Z0-9_]+)\s*\}\})");

    QRegularExpressionMatchIterator iter = reg.globalMatch(templateText);
    while (iter.hasNext())
    {
        auto match = iter.next();
        QString p = match.captured(1).trimmed();
        if (!params.contains(p))
            params << p;
    }
    return params;
}

QString JsonTemplateUtil::replaceTemplate(const QString& templateText, const QMap<QString, QVariant>& paramMap)
{
    QString res = templateText;
    for (auto it = paramMap.constBegin(); it != paramMap.constEnd(); ++it)
    {
        QString placeholder = "{{" + it.key() + "}}";
        QString val = it.value().toString();
        res.replace(placeholder, val);
    }
    return res;
}

QStringList JsonTemplateUtil::checkMissingParams(const QStringList& templateParams, const QMap<QString, QVariant>& paramMap)
{
    QStringList missing;
    for (const QString& p : templateParams)
    {
        if (!paramMap.contains(p))
            missing << p;
    }
    return missing;
}