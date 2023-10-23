#pragma once

#include <QLocalSocket>
#include <QObject>
#include <QQmlEngine>

class CommandLine final : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

public:
    explicit CommandLine(QObject *parent = nullptr);

    Q_PROPERTY(QString currentDate READ currentDate NOTIFY currentDateChanged)
    inline QString currentDate() { return m_currentDate; }

    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged)
    inline QString password() { return m_password; }
    void setPassword(const QString &password);

    Q_PROPERTY(QString userName READ userName NOTIFY userNameChanged)
    inline QString userName() { return m_userName; }

    Q_PROPERTY(QString errorMessage READ errorMessage NOTIFY errorMessageChanged)
    inline QString errorMessage() { return m_errorMessage; }

    Q_PROPERTY(QUrl background READ background NOTIFY backgroundChanged)
    inline QUrl background() { return m_backgroundImagePath; }

    Q_PROPERTY(double opacity READ opacity NOTIFY opacityChanged)
    inline double opacity() { return m_opacity; }

    Q_INVOKABLE void UnLock();
    Q_INVOKABLE void RequestUnlock();

signals:
    void currentDateChanged();
    void passwordChanged();
    void userNameChanged();
    void errorMessageChanged();
    void usePamChanged();
    void opacityChanged();
    void backgroundChanged();

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
};
