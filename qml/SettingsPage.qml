import QtQuick.Controls

Page {
    id: settings
    signal switchBackMainPage
    RoundButton {
        icon.name: "go-previous"
        onClicked: {
            settings.switchBackMainPage();
        }
        anchors.margins: 10
        anchors.left: parent.left
        anchors.top: parent.top
    }
}
