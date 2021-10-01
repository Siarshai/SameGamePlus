import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Dialog {
    id: dialog
    signal addToScores(var name)

    function resetInput() {
        player_name.text = "Player"
    }

    height: 250
    width: 300
    x: parent.width / 2 - width / 2
    y: parent.height / 2 - height / 2

    focus: true
    modal: true
    title: qsTr("You won!")
    property int score: 0
    closePolicy: Popup.NoAutoClose

    contentItem: Item {
        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 12
            spacing: 8
            Label {
                text: "Your final score is " + score
            }
            Label {
                text: "Input your name for scoreboard:"
            }
            TextInput {
                id: player_name
                width: dialog.width - 20
                text: "Player"
                focus: true
                font.pointSize: 18
                validator: RegExpValidator { regExp: /[a-zA-Z0-9]+/ }
            }
            Item {
                Layout.fillHeight: true
            }
        }
    }

    standardButtons: Dialog.Ok

    onAccepted: {
        addToScores(player_name.text)
    }
}

