import QtQuick.Controls
import QtQuick.Layouts
import QtQuick
import WayCrateDM

Page {
    id: settings
    signal switchBackMainPage

    Dialog {
        id: desktopDialog
        x: Math.round((settings.width - width) / 2)
        y: Math.round(settings.height / 6)
        width: Math.round(Math.min(settings.width, settings.height) / 3 * 2)
        height: settings.height * 2 / 3
        ListView {
            anchors.fill: parent
            clip: true
            model: DesktopModel
            delegate: Button {
                width: ListView.view.width
                text: name
                onClicked: {
                    DesktopConfigModel.insertData(name);
                    desktopDialog.close();
                }
            }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        Item {
            Layout.preferredHeight: 50
        }
        ListView {
            id: desktopView
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            model: DesktopConfigModel
            delegate: SettingsItemDelegate {
                onRequestRemoveIndex: index => {
                    DesktopConfigModel.remove(index);
                }
            }
        }
    }

    RoundButton {
        icon.name: "go-previous"
        onClicked: {
            settings.switchBackMainPage();
        }
        anchors.margins: 10
        anchors.left: parent.left
        anchors.top: parent.top
    }

    RoundButton {
        text: "+"
        anchors.margins: 20
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        highlighted: true
        onClicked: {
            desktopDialog.open();
        }
    }
}
