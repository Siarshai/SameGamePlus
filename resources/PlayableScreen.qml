import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12


Rectangle {
    id: container
    signal backToMainScreen();
    SystemPalette { id: activePalette }
    property int show_input_score_threshold: 0

    Item {
        width: parent.width
        anchors { top: parent.top; bottom: gameToolBar.top }

        Image {
            anchors.fill: parent
            source: "pics/screen2.png"
            fillMode: Image.PreserveAspectCrop
        }

        Item {
            id: gameCanvas
            objectName: "game_canvas"

            property date gameDuration
            property int blockSize: 40
            property int maxColumn: 10
            property int maxRow: 15

            width: blockSize * maxColumn
            height: blockSize * maxRow
            anchors.centerIn: parent

            function onNewGameStarted(max_column, max_row, threshold) {
                gameCanvas.maxColumn = max_column;
                gameCanvas.maxRow = max_row;
                show_input_score_threshold = threshold;
            }

            function onGameOver() {
                if (scoreLabel.score > show_input_score_threshold) {
                    winDialogWithInput.score = scoreLabel.score;
                    winDialogWithInput.resetInput();
                    winDialogWithInput.open();
                } else {
                    winDialog.score = scoreLabel.score;
                    winDialog.open();
                }
            }
        }
    }

    WinDialog {
        id: winDialog
    }

    WinDialogWithInput {
        id: winDialogWithInput
        objectName: "win_dialog_with_input"
    }

    Rectangle {
        id: gameToolBar
        height: restart_button.height
        width: parent.width;
        color: activePalette.window
        anchors { bottom: container.bottom }

        RowLayout {
            anchors.fill: parent
            Button {
                id: restart_button
                objectName: "restart_button"
                Layout.alignment: Qt.AlignHLeft | Qt.AlignVCenter
                height: 40
                width: height
                implicitWidth: 40
                implicitHeight: 40
                icon.color: "transparent"
                icon.source: "pics/noun_reload_185202.png"
            }
            Button {
                id: backToMenu
                objectName: "back_to_menu_button"
                Layout.alignment: Qt.AlignHLeft | Qt.AlignVCenter
                height: gameToolBar.height
                width: height
                implicitWidth: 40
                implicitHeight: 40
                icon.color: "transparent"
                icon.source: "pics/noun_back_275623.png"
                onClicked: container.backToMainScreen()
            }
            Item {
                Layout.fillWidth: true
            }
            Text {
                id: scoreLabel
                objectName: "score_label"
                Layout.alignment: Qt.AlignHRight | Qt.AlignVCenter
                rightPadding: 10
                text: "Score: " + score
                property int score: 0
                function onScoreChanged(new_score) {
                    score = new_score
                }
            }
        }
    }
}