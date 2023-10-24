import QtQuick.Controls
import QtQuick
import WayCrateDM

Dialog {
    id: desktopDialog
    signal currentSelected(int index)
    ListView {
        anchors.fill: parent
        clip: true
        model: DesktopModel
        delegate: Button {
            width: ListView.view.width
            text: name
            onClicked: {
                currentSelected(index)
                desktopDialog.close();
            }
        }
    }
}
