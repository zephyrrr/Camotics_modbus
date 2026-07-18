#include "SystemSettingsWrapper.h"

QString SystemSettingsWrapper::GetPath() const {
    return m_settings.GetPath();
}

QVariant SystemSettingsWrapper::GetValue(const QString& key) const {
    return m_settings.GetValue(key);
}