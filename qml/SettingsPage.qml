import QtQuick
import QtQuick.Controls

Page {
    id: settings
    signal switchBackMainPage
    Column {
        RoundButton {
            icon.name: "go-previous"
            onClicked: {
                settings.switchBackMainPage();
            }
        }
    }
}
