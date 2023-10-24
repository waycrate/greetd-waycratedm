import QtQuick.Controls
import QtQuick.Layouts
import QtQuick
import WayCrateDM

Page {
    id: root
    anchors.fill: parent
    property bool isIn: false

    property int year
    property int month
    property int day

    property int hours
    property int minutes

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

    function timeChanged() {
        var date = new Date;
        year = date.getFullYear();
        month = date.getMonth() + 1;
        day = date.getDate();
        hours = date.getHours();
        minutes = date.getMinutes();
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
                    CommandLine.RequestUnlock();
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
                visible: root.isIn
                implicitWidth: 60
                implicitHeight: 60
                Layout.alignment: Qt.AlignHCenter
                icon.name: "unlock"
                onClicked: {
                    CommandLine.RequestUnlock();
                }
            }

            Item {
                Layout.fillHeight: true
            }

            Item {
                Layout.preferredHeight: 40
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
                    spacing: 10

                    Repeater {
                        model: DesktopModel
                        Button {
                            text: name
                        }
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
                }
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
}
