// Unit for desktop config
// { name: string,
//   hasAlias: bool,
//   alias: string,
//   envs: [string]
// }

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
    static const QHash<int, QByteArray> roles{
      {Name, "name"}, {HasAlias, "hasAlias"}, {ExecAlias, "execAlias"}, {Envs, "envs"}};
    return roles;
}

bool
DesktopConfigModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    switch (role) {
    case Name:
        m_configs[index.row()].name = value.toString();
        break;
    case HasAlias:
        m_configs[index.row()].hasAlias = value.toBool();
        break;
    case ExecAlias:
        m_configs[index.row()].execAlias = value.toString();
        break;
    case Envs:
        m_configs[index.row()].envs = value.toStringList();
        break;
    default:
        return false;
    }
    return true;
}

QVariant
DesktopConfigModel::data(const QModelIndex &index, int role) const
{
    switch (role) {
    case Name:
        return m_configs[index.row()].name;
    case HasAlias:
        return m_configs[index.row()].hasAlias;
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
    m_configs.push_back(
      DesktopConfig{.name = desktop, .hasAlias = false, .execAlias = "", .envs = {}});
    endInsertRows();
}

void
DesktopConfigModel::remove(int row)
{
    if (row < 0 || row >= m_configs.count())
        return;

    beginRemoveRows(QModelIndex(), row, row);
    m_configs.removeAt(row);
    endRemoveRows();
}
