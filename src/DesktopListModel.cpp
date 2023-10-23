#include "DesktopListModel.h"
#include <QDir>
#include <QSettings>
#include <QStringList>

DesktopModel::DesktopModel(QObject *parent)
  : QAbstractListModel(parent)
{
    QDir waylandapp("/usr/share/wayland-sessions/");
    QStringList desktops = waylandapp.entryList({"*.desktop"}, QDir::Files);
    for (QString &desktop : desktops) {
        auto path = QString("/usr/share/wayland-sessions/%1").arg(desktop);
        QSettings settings(path, QSettings::IniFormat);
        settings.beginGroup("Desktop Entry");
        m_infos.append(DesktopInfo{
          .name = settings.value("Name").toString().trimmed(),
          .exec = settings.value("Exec").toString().trimmed(),
        });
    }
}

int
DesktopModel::rowCount(const QModelIndex &) const
{
    return m_infos.length();
}

QVariant
DesktopModel::data(const QModelIndex &index, int role) const
{
    switch (role) {
    case Name:
        return m_infos[index.row()].name;
    case Exec:
        return m_infos[index.row()].exec;
    default:
        return QVariant();
    }
}

QHash<int, QByteArray>
DesktopModel::roleNames() const
{
    static const QHash<int, QByteArray> roles{{Name, "name"}, {Exec, "exec"}};
    return roles;
}

QDebug
operator<<(QDebug d, const DesktopModel::DesktopInfo &info)
{
    d << info.name << info.exec;
    return d;
}
