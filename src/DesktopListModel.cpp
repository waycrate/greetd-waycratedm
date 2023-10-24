#include "DesktopListModel.h"
#include <QDir>
#include <QSettings>
#include <QStringList>

DesktopModel::DesktopModel(QObject *parent)
  : QAbstractListModel(parent)
{
    QStringList entrys;
    auto searchSessions = [&entrys, this](const QString &dir) {
        if (auto entryDir = QDir(dir); entryDir.exists()) {
            QStringList desktops = entryDir.entryList({"*.desktop"}, QDir::Files);
            for (QString &desktop : desktops) {
                if (entrys.contains(desktop)) {
                    continue;
                }
                entrys.push_back(desktop);
                auto path = QString("%1/%2").arg(dir).arg(desktop);
                QSettings settings(path, QSettings::IniFormat);
                settings.beginGroup("Desktop Entry");
                m_infos.append(DesktopInfo{.name     = settings.value("Name").toString().trimmed(),
                                           .exec     = settings.value("Exec").toString().trimmed(),
                                           .fileName = desktop});
            }
        }
    };
    searchSessions("/usr/local/share/wayland-sessions/");
    searchSessions("/usr/share/wayland-sessions/");
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
    case FileName:
        return m_infos[index.row()].fileName;
    default:
        return QVariant();
    }
}

QHash<int, QByteArray>
DesktopModel::roleNames() const
{
    static const QHash<int, QByteArray> roles{
      {Name, "name"}, {Exec, "exec"}, {FileName, "fileName"}};
    return roles;
}

int
DesktopModel::get_currentIndex(const QString &name)
{
    if (m_infos.length() == 0) {
        return -1;
    }
    int index = 0;
    for (const DesktopInfo &info : m_infos) {
        if (info.name == name) {
            return index;
        }
        index++;
    }
    return 0;
}
QDebug
operator<<(QDebug d, const DesktopModel::DesktopInfo &info)
{
    d << info.name << info.exec << info.fileName;
    return d;
}
