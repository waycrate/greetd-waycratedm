import QtQuick
import WayCrateDM

Window {
    id: w
    MainView {
        anchors.fill: parent
        onSwitchToSetting: {
            settings.visible = true;
        }
    }

    SettingsPage {
        id: settings
        anchors.fill: parent
        visible: false
        onSwitchBackMainPage: {
            settings.visible = false;
        }
    }
}
