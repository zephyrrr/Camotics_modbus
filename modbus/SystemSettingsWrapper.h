#ifndef SYSTEMSETTINGSWRAPPER_H
#define SYSTEMSETTINGSWRAPPER_H

#include <QVariant>
#include <QString>
#include "SystemSettings.h"  // 包含原始 SystemSettings 类的头文件

class SystemSettingsWrapper {
public:
    QString GetPath() const;
    QVariant GetValue(const QString& key) const;

private:
    SystemSettings m_settings;  // 持有原始 SystemSettings 实例
};

#endif // SYSTEMSETTINGSWRAPPER_H