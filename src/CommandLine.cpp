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

static PassWordInfo *PASSWORDINFO_INSTANCE = nullptr;

enum PamStatus
{
    PamEndFailed,
    Successed,
    Failed,

};

PassWordInfo::PassWordInfo(QObject *parent)
  : QObject(parent)
  , m_password(QString())
{
}

PassWordInfo *
PassWordInfo::instance()
{
    if (!PASSWORDINFO_INSTANCE) {
        PASSWORDINFO_INSTANCE = new PassWordInfo;
    }
    return PASSWORDINFO_INSTANCE;
}

void
PassWordInfo::setPassword(const QString &password)
{
    m_password = password;
}

static int
handle_conversation(int num_msg,
                    const struct pam_message **msg,
                    struct pam_response **resp,
                    void *data)
{
    /* PAM expects an array of responses, one for each message */
    struct pam_response *pam_reply =
      static_cast<struct pam_response *>(calloc(num_msg, sizeof(struct pam_response)));
    if (pam_reply == NULL) {
        return PAM_ABORT;
    }
    *resp = pam_reply;
    for (int i = 0; i < num_msg; ++i) {
        switch (msg[i]->msg_style) {
        case PAM_PROMPT_ECHO_OFF:
        case PAM_PROMPT_ECHO_ON:
            pam_reply[i].resp =
              strdup(PassWordInfo::instance()->password().toLocal8Bit().data()); // PAM clears and
                                                                                 // frees this
            if (pam_reply[i].resp == NULL) {
                return PAM_ABORT;
            }
            break;
        case PAM_ERROR_MSG:
        case PAM_TEXT_INFO:
            break;
        }
    }
    return PAM_SUCCESS;
}

CommandLine::CommandLine(QObject *parent)
  : QObject(parent)
  , m_currentDate(QLocale().toString(QDate::currentDate()))
  , m_handle(nullptr)
  , m_usePam(true)
  , m_backgroundImagePath(QUrl("qrc:/image/gangdamu.png"))
  , m_opacity(0.6)
{
    m_userName = QString::fromStdString(getlogin());
    readConfig();
    if (!m_usePam) {
        return;
    }
    const struct pam_conv conv = {
      .conv        = &handle_conversation,
      .appdata_ptr = NULL,
    };
    if (pam_start("waycratelock", m_userName.toLocal8Bit().data(), &conv, &m_handle) !=
        PAM_SUCCESS) {
        qWarning() << "Cannot start pam";
        QTimer::singleShot(0, this, [this] { this->UnLock(); });
        return;
    }
}

void
CommandLine::readConfig()
{
    QString configpath = get_config_path();
    if (!QFile(configpath).exists()) {
        return;
    }
    try {
        auto tbl                              = toml::parse_file(configpath.toStdString());
        std::optional<bool> usePam            = tbl["needPassword"].value<bool>();
        std::optional<std::string> background = tbl["background"]["path"].value<std::string>();
        std::optional<double> opacity          = tbl["background"]["opacity"].value<float>();
        m_opacity                             = opacity.value_or(0.6);
        m_usePam                              = usePam.value_or(true);
        if (background.has_value()) {
            QString backgroundPath = QString::fromStdString(background.value());
            if (backgroundPath.startsWith("~")) {
                backgroundPath.remove(0, 1);
                QString home          = QDir::homePath();
                backgroundPath        = QString("%1/%2").arg(home).arg(backgroundPath);
                m_backgroundImagePath = QUrl::fromLocalFile(backgroundPath);
            } else {
                m_backgroundImagePath =
                  QUrl::fromLocalFile(QString::fromStdString(background.value()));
            }
        }
    } catch (const toml::parse_error &err) {
        m_errorMessage = "Something error with config file";
    }
}

void
CommandLine::setPassword(const QString &password)
{
    m_password = password;
    PassWordInfo::instance()->setPassword(password);
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
    if (!m_usePam) {
        UnLock();
        return;
    }

    if (m_password.isEmpty()) {
        m_errorMessage = "password is needed";
        Q_EMIT errorMessageChanged();
        return;
    }

    QtConcurrent::run([this] {
        std::lock_guard<std::mutex> guard(PAM_GUARD);
        int pam_status = pam_authenticate(m_handle, 0);
        if (pam_status != PAM_SUCCESS) {
            return PamStatus::Failed;
        }
        pam_setcred(m_handle, PAM_REFRESH_CRED);
        if (pam_end(m_handle, pam_status)) {
            qWarning() << "Pam end failer";
            return PamStatus::PamEndFailed;
        }
        return PamStatus::Successed;
    }).then([this](PamStatus value) {
        switch (value) {
        case PamEndFailed: {
            qWarning() << "Pam end failer";
            UnLock();
            break;
        }
        case Successed: {
            UnLock();
            break;
        }
        case Failed: {
            m_errorMessage = "password is error, failed to unlock";
            Q_EMIT errorMessageChanged();
            break;
        }
        }
    });
}
