import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import Qt.labs.settings 1.0


Rectangle {
    id: container
    signal backToMainScreen();
    signal changeDifficulty(var difficulty);
    signal levelIdxChanged(var level_idx);

    property string difficulty: settings.difficulty;
    property int level_idx: settings.level_idx;
    property var level_names: settings.level_names;

    Settings {
        id: settings
        property string difficulty: "medium"
        property int level_idx: 0
        property var level_names: []
    }
    Component.onDestruction: {
        settings.level_idx = container.level_idx
        settings.difficulty = container.difficulty
    }

    Image {
        anchors.fill: parent
        source: "pics/screen3.png"
        fillMode: Image.PreserveAspectCrop
    }

    ColumnLayout {
        spacing: 15
        anchors.fill: parent

        Item {
            Layout.fillHeight: true
        }
        Button {
            Layout.alignment: Qt.AlignHCenter
            // in case number of level decreases
            text: "Level: " + level_names[level_idx >= level_names.length ? 0 : level_idx]
            onClicked: {
                ++level_idx
                if (level_idx >= level_names.length)
                    level_idx = 0;
                container.levelIdxChanged(level_idx)
            }
        }
        Button {
            Layout.alignment: Qt.AlignHCenter
            text: "Difficulty: " + container.difficulty
            onClicked: {
                container.difficulty = (container.difficulty == "medium" ? "hard" : "medium")
                container.changeDifficulty(container.difficulty)
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