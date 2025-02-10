#pragma once

#include <QJsonObject>
#include <QLocalSocket>
#include <QObject>
#include <QQmlEngine>

class CommandLine final : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    enum LoginStatus
    {
        Start,
        CreateSessionSuccessded,
        TryToLoginSession,
        TryToStartSession,
        LoginSuccessded,
        CancelSessionSuccessded,
        Errored,
    };

public:
    explicit CommandLine(QObject *parent = nullptr);

    Q_PROPERTY(QUrl userIcon READ userIcon NOTIFY userIconChanged)
    inline QUrl userIcon() { return m_userIcon; }

    Q_PROPERTY(QString currentDate READ currentDate NOTIFY currentDateChanged)
    inline QString currentDate() { return m_currentDate; }

    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged)
    inline QString password() { return m_password; }
    void setPassword(const QString &password);

    Q_PROPERTY(QString userName READ userName WRITE setUserName NOTIFY userNameChanged)
    inline QString userName() { return m_userName; }
    void setUserName(const QString &userName);

    Q_PROPERTY(QString command READ command WRITE setCommand NOTIFY commandChanged)
    inline QString command() { return m_command; }
    void setCommand(const QString &command);

    Q_PROPERTY(QStringList env READ env WRITE setEnv NOTIFY envChanged)
    inline QStringList env() { return m_env; }
    void setEnv(const QStringList &env);

    Q_PROPERTY(QString errorMessage READ errorMessage NOTIFY errorMessageChanged)
    inline QString errorMessage() { return m_errorMessage; }

    Q_PROPERTY(QUrl background READ background NOTIFY backgroundChanged)
    inline QUrl background() { return m_backgroundImagePath; }

    Q_PROPERTY(double opacity READ opacity NOTIFY opacityChanged)
    inline double opacity() { return m_opacity; }

    Q_PROPERTY(bool isAuthing READ isAuthing NOTIFY isAuthingChanged)
    inline bool isAuthing() { return m_isAuthing; }

    Q_INVOKABLE void UnLock();
    Q_INVOKABLE void RequestLogin();
    Q_INVOKABLE void RequestShutDown();

signals:
    void currentDateChanged();
    void passwordChanged();
    void userNameChanged();
    void errorMessageChanged();
    void usePamChanged();
    void opacityChanged();
    void backgroundChanged();
    void userIconChanged();
    void commandChanged();
    void isAuthingChanged();
    void envChanged();

private slots:
    void handleDataRead();
    void handleAuthPasswordMessage();
    void handleAuthMessageNone();

    void handleAuthError();
    void handleSuccessded();

private:
    void connectToGreetd();
    void tryLogin();
    void roundtrip(const QString &payload);

private:
    QString m_currentDate;
    QString m_password;
    QString m_userName;
    QString m_errorMessage;
    QUrl m_backgroundImagePath;
    double m_opacity;
    QLocalSocket *m_greetd;
    LoginStatus m_status;
    QUrl m_userIcon;
    QString m_command;
    bool m_isAuthing;
    QStringList m_env;
};
