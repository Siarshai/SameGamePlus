import QtQuick 2.12
import QtQuick.Particles 2.12


Item {
    ParticleSystem {
        id: sys
        anchors.centerIn: parent
        ImageParticle {
            source: {
                if (type == 0)
                    return "pics/red_specks.png";
                else if (type == 1)
                    return "pics/blue_specks.png";
                else if (type == 2)
                    return "pics/green_specks.png";
                else
                    return "pics/yellow_specks.png";
            }
            rotationVelocityVariation: 360
        }

        Emitter {
            id: particles
            anchors.centerIn: parent
            emitRate: 0
            lifeSpan: 700
            velocity: AngleDirection {angleVariation: 360; magnitude: 80; magnitudeVariation: 40}
            size: 16
        }
    }

    id: block
    property bool dying: false
    property bool x_spring_animation_enabled: false
    property int type: 0
    property int row: 0
    property int column: 0

    states: [
        State {
            name: "AliveState";
            when: dying == false;
            PropertyChanges { target: img; opacity: 1 }
        },
        State {
            name: "DeathState";
            when: dying == true;
            StateChangeScript { script: particles.burst(50); }
            PropertyChanges { target: img; opacity: 0 }
            StateChangeScript { script: block.destroy(1000); }
        }
    ]

    Behavior on x {
        enabled: x_spring_animation_enabled;
        SpringAnimation { spring: 2; damping: 0.2 }
    }
    Behavior on y {
        SpringAnimation { spring: 2; damping: 0.2; }
    }

    Image {
        id: img
        anchors.fill: parent
        source: {
            if (type == 0)
                return "pics/red_block.png";
            else if (type == 1)
                return "pics/blue_block.png";
            else if (type == 2)
                return "pics/green_block.png";
            else
                return "pics/yellow_block.png";
        }

        opacity: 0.0

        Behavior on opacity {
            NumberAnimation {
                properties: "opacity"
                duration: 200
            }
        }
    }

    MouseArea {
        objectName: "clickable_block_area"
        width: parent.width
        height: parent.height
        hoverEnabled: true

        signal blockClick(var column, var row);

        onClicked: {
            if (parent.dying)
                return;
            blockClick(parent.column, parent.row);
        }
    }
}