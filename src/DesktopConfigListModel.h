#pragma once

#include <QAbstractListModel>
#include <QList>
#include <QObject>
#include <QQmlEngine>

class DesktopConfigModel final : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

public:
    struct DesktopConfig
    {
        QString name;
        QString execAlias;
        QStringList envs;
    };
    explicit DesktopConfigModel(QObject *parent = nullptr);

    enum DesktopConfigRole
    {
        Name = Qt::DisplayRole,
        ExecAlias,
        Envs
    };

    Q_INVOKABLE void insertData(const QString &desktop);

private:
    int rowCount(const QModelIndex & = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    bool dataIsExist(const QString &desktop) const;

private:
    QList<DesktopConfig> m_configs;
};
