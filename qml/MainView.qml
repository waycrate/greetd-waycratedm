import QtQuick.Controls
import QtQuick.Layouts
import QtQuick
import WayCrateDM

Page {
    id: root

    property bool isIn: false

    property int year
    property int month
    property int day

    property int hours
    property int minutes

    signal switchToSetting

    function leftPad(number) {
        var output = number + '';
        while (output.length < 2) {
            output = '0' + output;
        }
        return output;
    }

    background: Image {
        anchors.fill: root
        fillMode: Image.PreserveAspectCrop
        source: CommandLine.background
        opacity: CommandLine.opacity
    }

    Item {
        Timer {
            interval: 100
            running: true
            repeat: true
            onTriggered: root.timeChanged()
        }
    }

    DesktopDialog {
        id: desktopDialog
        x: Math.round((root.width - width) / 2)
        y: Math.round(root.height / 6)
        width: Math.round(Math.min(root.width, root.height) / 3 * 2)
        height: root.height * 2 / 3
        onCurrentSelected: index => {
            view.currentIndex = index;
        }
    }

    function timeChanged() {
        var date = new Date;
        year = date.getFullYear();
        month = date.getMonth() + 1;
        day = date.getDate();
        hours = date.getHours();
        minutes = date.getMinutes();
    }

    function login() {
        if (CommandLine.isAuthing) {
            return;
        }
        var currentDesktop = DesktopModel.get(view.currentIndex).name;
        if (DesktopConfigModel.dataIsExist(currentDesktop)) {
            var envs = DesktopConfigModel.getFromName(currentDesktop).envs;
            CommandLine.env = envs;
        }
        Settings.setStartSession(DesktopModel.get(view.currentIndex).name);
        Settings.setStartUser(user.text);
        CommandLine.RequestLogin();
    }

    TimePanel {
        x: 30
        y: 30
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onEntered: {
            root.isIn = true;
            root.forceActiveFocus();
        }
        onExited: {
            root.isIn = false;
        }

        ColumnLayout {
            anchors.fill: parent

            Item {
                Layout.preferredHeight: 30
            }

            RowLayout {
                Layout.fillWidth: true
                Item {
                    Layout.fillWidth: true
                }
                RoundButton {
                    icon.source: "qrc:/image/setting.svg"
                    onClicked: {
                        root.switchToSetting();
                    }
                }
                Item {
                    Layout.preferredWidth: 10
                }
            }
            Label {
                text: CommandLine.currentDate
                Layout.alignment: Qt.AlignHCenter
                font.pointSize: root.isIn ? 30 : 35
                font.bold: true
            }

            Item {
                Layout.fillHeight: true
            }

            Image {
                source: CommandLine.userIcon
                Layout.alignment: Qt.AlignHCenter
                Layout.preferredHeight: 100
                Layout.preferredWidth: 100
            }

            Item {
                Layout.preferredHeight: 30
            }

            TextField {
                id: user
                enabled: !CommandLine.isAuthing
                visible: root.isIn
                Layout.alignment: Qt.AlignHCenter
                text: CommandLine.userName
                onEditingFinished: {
                    CommandLine.userName = user.text;
                }
                placeholderText: "User"
                Layout.preferredWidth: 250
            }

            TextField {
                id: input
                enabled: !CommandLine.isAuthing
                visible: root.isIn
                Layout.alignment: Qt.AlignHCenter
                text: CommandLine.password
                placeholderText: "Password"
                onEditingFinished: {
                    CommandLine.password = input.text;
                }
                echoMode: TextInput.Password
                Layout.preferredWidth: 250
                onAccepted: {
                    root.login();
                }
            }

            Label {
                visible: CommandLine.errorMessage !== "" && root.isIn
                text: CommandLine.errorMessage
                Layout.alignment: Qt.AlignHCenter
                font.pointSize: 15
                color: "red"
            }

            RoundButton {
                id: loginBtn
                highlighted: true
                enabled: !CommandLine.isAuthing
                visible: root.isIn
                implicitWidth: 60
                implicitHeight: 60
                Layout.alignment: Qt.AlignHCenter
                icon.name: "unlock"
                onClicked: {
                    root.login();
                }
            }

            Item {
                Layout.fillHeight: true
            }

            RowLayout {
                Layout.alignment: Qt.AlignHCenter
                Button {
                    icon.name: "go-previous"
                    onClicked: {
                        if (view.currentIndex === 0) {
                            return;
                        }
                        view.currentIndex -= 1;
                    }
                }
                SwipeView {
                    id: view
                    clip: true
                    spacing: 10
                    Layout.preferredWidth: 250
                    Repeater {
                        model: DesktopModel
                        Button {
                            text: name
                        }
                    }
                    currentIndex: DesktopModel.get_currentIndex(Settings.startSession())
                    onCurrentIndexChanged: {
                        CommandLine.command = DesktopModel.get(view.currentIndex).exec;
                    }
                }
                Button {
                    icon.name: "go-next"
                    onClicked: {
                        if (view.currentIndex >= view.count - 1) {
                            return;
                        }
                        view.currentIndex += 1;
                    }
                }
            }
            RowLayout {
                Layout.alignment: Qt.AlignHCenter
                spacing: 5

                PageIndicator {
                    id: indicator
                    Layout.alignment: Qt.AlignHCenter

                    count: view.count
                    currentIndex: view.currentIndex
                }

                RoundButton {
                    icon.source: "qrc:/image/menu.svg"
                    onClicked: {
                        desktopDialog.open();
                    }
                }
            }

            TextField {
                id: commandField
                placeholderText: "Command"
                Layout.alignment: Qt.AlignHCenter
                text: {
                    var currentDesktop = DesktopModel.get(view.currentIndex).name;
                    if (!DesktopConfigModel.dataIsExist(currentDesktop)) {
                        return CommandLine.command;
                    }
                    var map = DesktopConfigModel.getFromName(currentDesktop);
                    if (!map.hasAlias) {
                        return CommandLine.command;
                    }
                    CommandLine.command = map.execAlias;
                    return map.execAlias;
                }
                onEditingFinished: {
                    CommandLine.command = commandField.text;
                }
                wrapMode: TextEdit.WordWrap
                Layout.preferredWidth: 350
            }

            Item {
                Layout.preferredHeight: 40
            }

            RowLayout {
                Layout.alignment: Qt.AlignHCenter
                Label {
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    text: root.leftPad(root.hours)
                    font.pointSize: root.isIn ? 30 : 35
                    font.bold: true
                    Layout.preferredWidth: 80
                }
                Label {
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    text: ":"
                    font.pointSize: root.isIn ? 30 : 35
                    font.bold: true
                }
                Label {
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    text: root.leftPad(root.minutes)
                    font.pointSize: root.isIn ? 30 : 35
                    font.bold: true
                    Layout.preferredWidth: 80
                }
            }

            Item {
                Layout.preferredHeight: 30
            }
        }
    }
    RoundButton {
        icon.source: "qrc:/image/system-shutdown.svg"
        anchors.margins: 20
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        highlighted: true
        onClicked: {
            CommandLine.RequestShutDown()
        }
        enabled: !CommandLine.isAuthing
    }
}
