#pragma once

#include <QAbstractListModel>
#include <QList>
#include <QObject>
#include <QQmlEngine>
#include <algorithm>

class DesktopConfigModel final : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

public:
    struct DesktopConfig
    {
        QString name;
        bool hasAlias;
        QString execAlias;
        QStringList envs;
    };
    explicit DesktopConfigModel(QObject *parent = nullptr);

    enum DesktopConfigRole
    {
        Name = Qt::DisplayRole,
        HasAlias,
        ExecAlias,
        Envs
    };

    Q_INVOKABLE void insertData(const QString &desktop);
    Q_INVOKABLE bool
    setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    Q_INVOKABLE void remove(int row);

    Q_INVOKABLE bool dataIsExist(const QString &desktop) const;

    Q_INVOKABLE QVariantMap getFromName(const QString &name) const
    {
        QVariantMap data;
        for (int i = 0; i < m_configs.length(); ++i) {
            if (m_configs[i].name == name) {
                data.insert("name", m_configs[i].name);
                data.insert("hasAlias", m_configs[i].hasAlias);
                data.insert("execAlias", m_configs[i].execAlias);
                data.insert("envs", m_configs[i].envs);
                break;
            }
        }
        return data;
    }

private:
    int rowCount(const QModelIndex & = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void get_file_config();
    void save_file_config();

private:
    QList<DesktopConfig> m_configs;
};
