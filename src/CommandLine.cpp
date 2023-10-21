#include "CommandLine.h"

#ifndef DEBUG_MODE
#include <SessionLockQt/command.h>
#endif

#include <QDate>
#include <QGuiApplication>
#include <QLocale>
#include <QTimer>
#include <QtConcurrent>

#include <format>
#include <mutex>
#include <string>
#include <toml++/toml.h>
#include <unistd.h>

constexpr static std::string CONFIG_FILE = "setting.toml";

constexpr static std::string CONFIGDIR = "waycratelock";

static std::mutex PAM_GUARD;

static QString
get_config_path()
{
    return QString::fromStdString(
      std::format("{}/{}/{}",
                  QStandardPaths::writableLocation(QStandardPaths::ConfigLocation).toStdString(),
                  CONFIGDIR,
                  CONFIG_FILE));
}

CommandLine::CommandLine(QObject *parent)
  : QObject(parent)
  , m_currentDate(QLocale().toString(QDate::currentDate()))
  , m_handle(nullptr)
  , m_backgroundImagePath(QUrl("qrc:/image/gangdamu.png"))
  , m_opacity(0.6)
{
    m_userName = QString::fromStdString(getlogin());
}

void
CommandLine::setPassword(const QString &password)
{
    m_password = password;
    Q_EMIT passwordChanged();
}

void
CommandLine::UnLock()
{
#ifndef DEBUG_MODE
    ExtSessionLockV1Qt::Command::instance()->unLockScreen();
#endif
    QTimer::singleShot(0, qApp, [] { QGuiApplication::quit(); });
}

void
CommandLine::RequestUnlock()
{
    if (m_password.isEmpty()) {
        m_errorMessage = "password is needed";
        Q_EMIT errorMessageChanged();
        return;
    }
    UnLock();
}
