import QtQuick 2.12
import QtQuick.Controls 2.12

Dialog {
    id: dialog

    height: 150
    width: 200
    x: parent.width / 2 - width / 2
    y: parent.height / 2 - height / 2

    focus: true
    modal: true
    title: qsTr("You won!")
    property int score: 0

    contentItem: Label {
        text: "Your final score is " + score
    }

    standardButtons: Dialog.Ok
}

