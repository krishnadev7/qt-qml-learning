import QtQuick

Item {
    id: root
    width: 300
    height: 18

    property bool running: true
    property int direction: 1
    property color waterColor: "#00d2ff"

    Rectangle {
        anchors.fill: parent
        radius: height / 2
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#222" }
            GradientStop { position: 0.5; color: "#555" }
            GradientStop { position: 1.0; color: "#111" }
        }

        Item {
            anchors.fill: parent
            anchors.margins: 4
            clip: true

            Rectangle {
                id: flow
                width: parent.width * 2
                height: parent.height
                x: root.direction === 1 ? -parent.width : 0
                visible: root.running

                gradient: Gradient {
                    orientation: Gradient.Horizontal
                    GradientStop { position: 0.0; color: root.waterColor }
                    GradientStop { position: 0.5; color: Qt.lighter(root.waterColor, 1.8) }
                    GradientStop { position: 1.0; color: root.waterColor }
                }

                PropertyAnimation on x {
                    from: root.direction === 1 ? -flow.width / 2 : 0
                    to: root.direction === 1 ? 0 : -flow.width / 2
                    duration: 1200
                    loops: Animation.Infinite
                    running: root.running
                }
            }
        }
    }
}
