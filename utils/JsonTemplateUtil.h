#ifndef JSONTEMPLATEUTIL_H
#define JSONTEMPLATEUTIL_H

#include <QString>
#include <QStringList>
#include <QMap>
#include <QVariant>

class JsonTemplateUtil
{
public:
    // 加载模板文件到字符串
    static bool loadTemplateFile(const QString& filePath, QString& outTemplateText);

    // 从模板文本中提取所有 {{param}} 参数，自动去重
    static QStringList extractAllParams(const QString& templateText);

    // 传入参数映射，替换模板中的 {{xxx}}
    static QString replaceTemplate(const QString& templateText, const QMap<QString, QVariant>& paramMap);

    // 校验：传入的参数是否覆盖模板所有参数，缺啥返回啥
    static QStringList checkMissingParams(const QStringList& templateParams, const QMap<QString, QVariant>& paramMap);
};

#endif // JSONTEMPLATEUTIL_H