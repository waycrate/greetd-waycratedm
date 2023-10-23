#pragma once

#include <QAbstractListModel>
#include <QList>
#include <QObject>
#include <QQmlEngine>

class DesktopModel final : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

public:
    struct DesktopInfo
    {
        QString name;
        QString exec;
    };


    explicit DesktopModel(QObject *parent = nullptr);

    enum DesktopRole
    {
        Name = Qt::DisplayRole,
        Exec,
    };
    int rowCount(const QModelIndex & = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    QList<DesktopInfo> m_infos;
};

QDebug
operator<<(QDebug d, const DesktopModel::DesktopInfo&);
