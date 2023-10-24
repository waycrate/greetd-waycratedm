#include "Settings.h"

Settings::Settings(QObject *parent)
  : QObject(parent)
  , m_setting(QSettings())
{
}

QString
Settings::startSession() const
{
    return m_setting.value("session").toString();
}

void
Settings::setStartSession(const QString &sessionName)
{
    m_setting.setValue("session", sessionName);
}
