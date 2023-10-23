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

#include <sys/socket.h>
#include <sys/un.h>

CommandLine::CommandLine(QObject *parent)
  : QObject(parent)
  , m_currentDate(QLocale().toString(QDate::currentDate()))
  , m_backgroundImagePath(QUrl("qrc:/image/gangdamu.png"))
  , m_opacity(0.6)
  , m_greetd(nullptr)
{
    m_userName = QString::fromStdString(getlogin());
    connectToGreetd();
}

void
CommandLine::setPassword(const QString &password)
{
    m_password = password;
    Q_EMIT passwordChanged();
}

void
CommandLine::connectToGreetd()
{
    QString path = qgetenv("GREETD_SOCK");
    if (path.isEmpty()) {
        qWarning() << "Unable to retrieve GREETD_SOCK";
        m_errorMessage = "Cannot connect to greetd";
        Q_EMIT errorMessageChanged();
        return;
    }
    m_greetd = new QLocalSocket(this);
    m_greetd->connectToServer(path, QIODevice::ReadWrite | QIODevice::Unbuffered);
    m_greetd->waitForConnected();

    QVariantMap request;

    request["type"]     = "create_session";
    request["username"] = "testAccount";
    QJsonDocument json;
    json.setObject(QJsonObject::fromVariantMap(request));

    roundtrip(json.toJson().simplified());

    connect(m_greetd, &QLocalSocket::readyRead, this, [this] { qDebug() << m_greetd->readAll(); });
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
CommandLine::tryLogin()
{
    if (!m_greetd) {
        return;
    }
    QVariantMap request;

    request["type"]     = "create_session";
    request["username"] = "testAccount";
    QJsonDocument json;

    json.setObject(QJsonObject::fromVariantMap(request));

    roundtrip(json.toJson().simplified());
}

void
CommandLine::roundtrip(const QString &payload)
{
    qint32 length = payload.length();
    m_greetd->write((const char *)&length, sizeof(length));
    m_greetd->write(payload.toUtf8());
}

void
CommandLine::RequestUnlock()
{
    if (m_password.isEmpty()) {
        m_errorMessage = "password is needed";
        Q_EMIT errorMessageChanged();
        return;
    }
    tryLogin();
}
