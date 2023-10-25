import QtQuick
import QtQuick.Controls

Page {
    id: settings
    signal switchBackMainPage
    Column {

        Button {
            onClicked: {
                settings.switchBackMainPage();
            }
        }
    }
}
