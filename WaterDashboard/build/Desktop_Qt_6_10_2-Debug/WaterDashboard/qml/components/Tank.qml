import QtQuick

Item {
    id: root
    width: 150
    height: 300

    property real level: 0
    property color waterColor: "#12b6ff"

    Rectangle {
        id: body
        anchors.fill: parent
        radius: 12
        color: "#1a1a1a"
        border.color: "#333333"
        border.width: 2

        // Glass shine
        Rectangle {
            width: parent.width * 0.1
            anchors { top: parent.top; bottom: parent.bottom; left: parent.left; margins: 10 }
            color: "white"
            opacity: 0.05
            radius: 5
        }

        Item {
            anchors.fill: parent
            anchors.margins: 4
            clip: true

            Rectangle {
                id: fill
                width: parent.width
                height: (root.level / 100) * parent.height
                y: parent.height - height
                color: root.waterColor
                radius: 2

                gradient: Gradient {
                    GradientStop { position: 0.0; color: Qt.lighter(root.waterColor, 1.4) }
                    GradientStop { position: 1.0; color: root.waterColor }
                }

                Behavior on height { NumberAnimation { duration: 800; easing.type: Easing.InOutQuad } }
                Behavior on y { NumberAnimation { duration: 800; easing.type: Easing.InOutQuad } }

                // Water surface light
                Rectangle {
                    width: parent.width
                    height: 4
                    anchors.top: parent.top
                    color: "white"
                    opacity: 0.3
                }
            }
        }
    }

    Text {
        anchors.centerIn: parent
        text: Math.round(root.level) + "%"
        color: "white"
        font.bold: true
        font.pixelSize: 20
        opacity: 0.8
    }
}
