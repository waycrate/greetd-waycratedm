import QtQuick
import WayCrateDM
import QtQuick.Controls

Window {
    id: w
    StackView {
        anchors.fill: parent
        initialItem: MainView {
        }
    }
}
