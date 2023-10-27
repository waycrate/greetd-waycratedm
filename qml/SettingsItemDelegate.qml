import QtQuick.Controls
import QtQuick.Layouts
import QtQuick

ItemDelegate {
    id: root
    width: parent ? parent.width : 100
    signal requestRemoveIndex(index: int);
    contentItem: ColumnLayout {
        id: delegateTop
        function removeEnv(value: string) {
            model.envs = Array.from(envs).filter(r => r!== value)
        }
        RowLayout {
            Label {
                text: name
                font.pointSize: 25
            }
            Item {
                Layout.fillWidth: true
            }
            RoundButton {
                text: "x"
                onClicked: {
                    root.requestRemoveIndex(index)
                }
            }
        }
        RowLayout {
            CheckBox {
                text: "alias Exec"
                checked: model.hasAlias
                onToggled: model.hasAlias = checked
            }
            Item {
                Layout.preferredWidth: 40
            }
            TextField {
                id: aliasField
                Layout.fillWidth: true
                visible: model.hasAlias
                text: model.execAlias
                onEditingFinished: {
                    model.execAlias = aliasField.text;
                }
                placeholderText: "ExecAlias"
            }
        }
        TextField {
            id: envField
            Layout.fillWidth: true
            placeholderText: "env, use key:value form"
            onAccepted: {
                var newmodel = model.envs;
                if (newmodel.includes(envField.text)) {
                    return;
                }
                newmodel.push(envField.text);
                model.envs = newmodel;
            }
        }
        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Repeater {
                model: envs
                delegate: RowLayout {
                    Label {
                        text: modelData
                    }
                    Item {
                        Layout.fillWidth: true
                    }
                    RoundButton {
                        text: "x"
                        onClicked: {
                            delegateTop.removeEnv(modelData)
                        }
                    }
                }
            }
        }
    }
}
