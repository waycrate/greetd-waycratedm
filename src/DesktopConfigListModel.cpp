// Unit for desktop config
// { name: string,
//   hasAlias: bool,
//   alias: string,
//   envs: [string]
// }

#include "DesktopConfigListModel.h"
#include <QFile>
#include <QGuiApplication>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonValueRef>
#include <QStandardPaths>

#include <algorithm>
#include <format>

static QString
get_config_path()
{
    return QString::fromStdString(
      std::format("{}/{}/desktopconfig.json",
                  QStandardPaths::writableLocation(QStandardPaths::ConfigLocation).toStdString(),
                  qApp->organizationName().toStdString()));
}

DesktopConfigModel::DesktopConfigModel(QObject *parent)
  : QAbstractListModel(parent)
{
    get_file_config();
}

void
DesktopConfigModel::get_file_config()
{
    QFile file(get_config_path());
    if (!file.exists() && !file.isReadable()) {
        return;
    }
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    auto bytes             = file.readAll();
    QJsonDocument document = QJsonDocument::fromJson(bytes);
    auto arrays            = document.array();
    for (auto array : arrays) {
        auto object = array.toObject();
        m_configs.push_back(DesktopConfig{.name      = object["name"].toString(),
                                          .hasAlias  = object["hasAlias"].toBool(),
                                          .execAlias = object["execAlias"].toString(),
                                          .envs      = object["envs"].toVariant().toStringList()});
    }
}

void
DesktopConfigModel::save_file_config()
{
    QJsonDocument document;
    QJsonArray array;
    for (auto config : m_configs) {
        array.push_back(QJsonObject({{"name", config.name},
                                     {"hasAlias", config.hasAlias},
                                     {"execAlias", config.execAlias},
                                     {"envs", QJsonArray::fromStringList(config.envs)}}));
    }
    document.setArray(array);
    QByteArray bytes = document.toJson(QJsonDocument::Indented);
    QFile file(get_config_path());
    if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        QTextStream iStream(&file);
        iStream.setEncoding(QStringConverter::Encoding::Utf8);
        iStream << bytes;
        file.close();
    } else {
        qWarning() << "Cannot save config";
    }
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
    save_file_config();
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
