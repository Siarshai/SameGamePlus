import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12


Rectangle {
    id: container
    signal backToMainScreen()
    property string difficulty: "medium"

    Image {
        anchors.fill: parent
        source: "pics/screen4.png"
        fillMode: Image.PreserveAspectCrop
    }

    ColumnLayout {
        spacing: 15
        anchors.fill: parent

        Item {
            Layout.fillHeight: true
        }
        Button {
            objectName: "main_scoreboards_button"
            Layout.alignment: Qt.AlignHCenter
            text: "Best Scores for Difficulty: " + difficulty
            onClicked: {
                difficulty = (difficulty == "medium" ? "hard" : "medium")
            }
        }

        Item {
            id: header
            Layout.alignment: Qt.AlignHCenter
            height: 25
            width: 300
            Item {
                id: column1
                height: 25
                width: 143
                implicitHeight: 25
                implicitWidth: 143
                Text {
                    anchors.fill: parent
                    font.pixelSize: 22
                    text: "Name"
                    horizontalAlignment: Text.AlignRight
                }
            }
            Item {
                height: 25
                width: 143
                implicitHeight: 25
                implicitWidth: 143
                anchors.left: column1.right
                anchors.leftMargin: 14
                Text {
                    anchors.fill: parent
                    font.pixelSize: 22
                    text: "Score"
                }
            }
        }

        TableView {
            id: tableView
            model: scores_application_data.getModel(difficulty)
            width: 300
            height: 250
            contentWidth: 300
            contentHeight: 250
            columnSpacing: 14
            clip: true

            Layout.alignment: Qt.AlignHCenter
            contentX: (contentWidth - width) / 2;
            contentY: (contentHeight - height) / 2;

            delegate: Item {
                implicitHeight: 25
                implicitWidth: 143
                Text {
                    text: modelData
                    anchors.fill: parent
                    horizontalAlignment: modelAlignment
                }
            }
        }
        Button {
            Layout.alignment: Qt.AlignHCenter
            text: "Back to Main Screen"
            onClicked: container.backToMainScreen()
        }
        Item {
            Layout.fillHeight: true
        }
    }
}