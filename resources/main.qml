import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12


Rectangle {
    id: base
    objectName: "base"
    width: 490; height: 720
    visible: true

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: main_screen

        function popCurrentScreen() { pop() }
    }

    Rectangle {
        id: main_screen

        Image {
            id: main_background
            anchors.fill: parent
            source: "pics/screen1.png"
            fillMode: Image.PreserveAspectCrop
        }

        ColumnLayout {
            spacing: 15
            anchors.fill: parent

            Item {
                Layout.fillHeight: true
            }
            Item {
                height: 50
                width: 300
                Layout.alignment: Qt.AlignHCenter
                Image {
                    id: main_title
                    anchors.fill: parent
                    source: "pics/main_title.png"
                }
            }
            MenuSeparator {
                Layout.alignment: Qt.AlignHCenter
                topPadding: 2
                bottomPadding: 2
                contentItem: Rectangle {
                    implicitWidth: 200
                    implicitHeight: 1
                    color: "#1E000000"
                }
            }
            Button {
                objectName: "main_start_button"
                Layout.alignment: Qt.AlignHCenter
                text: "New Game"
                onClicked: stackView.push(playable_screen)
            }
            Button {
                objectName: "main_options_button"
                Layout.alignment: Qt.AlignHCenter
                text: "Options"
                onClicked: {
                    stackView.push(Qt.resolvedUrl("OptionsScreen.qml"), {})
                    stackView.currentItem.backToMainScreen.connect(stackView.popCurrentScreen)
                }
            }
            Button {
                objectName: "main_scoreboards_button"
                Layout.alignment: Qt.AlignHCenter
                text: "Scoreboards"
                onClicked: {
                    stackView.push(Qt.resolvedUrl("ScoreboardsScreen.qml"), {})
                    stackView.currentItem.backToMainScreen.connect(stackView.popCurrentScreen)
                }
            }
            Button {
                objectName: "main_exit_button"
                Layout.alignment: Qt.AlignHCenter
                text: "Exit"
            }
            Item {
                Layout.fillHeight: true
            }
        }
    }

    PlayableScreen {
        id: playable_screen
        onBackToMainScreen: stackView.pop()
    }
}