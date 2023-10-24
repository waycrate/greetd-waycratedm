#pragma once

#include <QObject>
#include <QQmlEngine>
#include <QSettings>

class Settings final : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

public:
    explicit Settings(QObject *parent = nullptr);


    Q_INVOKABLE QString startSession() const;
    Q_INVOKABLE void setStartSession(const QString &sessionName);

private:
    QSettings m_setting;
};
