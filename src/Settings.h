#pragma once

#include <QObject>
#include <QQmlEngine>

class Settings final : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

public:
    explicit Settings(QObject *parent = nullptr);
};
