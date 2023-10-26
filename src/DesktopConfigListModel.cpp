#include "DesktopConfigListModel.h"
#include <algorithm>

DesktopConfigModel::DesktopConfigModel(QObject *parent)
  : QAbstractListModel(parent)
{
}

int
DesktopConfigModel::rowCount(const QModelIndex &index) const
{
    return m_configs.length();
}

QHash<int, QByteArray>
DesktopConfigModel::roleNames() const
{
    static const QHash<int, QByteArray> roles{{Name, "name"}, {ExecAlias, "exec"}, {Envs, "envs"}};
    return roles;
}

QVariant
DesktopConfigModel::data(const QModelIndex &index, int role) const
{
    switch (role) {
    case Name:
        return m_configs[index.row()].name;
    case ExecAlias:
        return m_configs[index.row()].execAlias;
    case Envs:
        return m_configs[index.row()].envs;
    default:
        return QVariant();
    }
}

bool
DesktopConfigModel::dataIsExist(const QString &desktop) const
{
    return std::any_of(m_configs.begin(), m_configs.end(), [desktop](const DesktopConfig &config) {
        return config.name == desktop;
    });
}

void
DesktopConfigModel::insertData(const QString &desktop)
{
    if (dataIsExist(desktop)) {
        return;
    }
    beginInsertRows(QModelIndex(), m_configs.length(), m_configs.length());
    m_configs.push_back(DesktopConfig{.name = desktop, .execAlias = "", .envs = {}});
    endInsertRows();
}
