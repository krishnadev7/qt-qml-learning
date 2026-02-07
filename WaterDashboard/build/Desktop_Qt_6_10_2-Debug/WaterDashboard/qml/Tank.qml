import QtQuick

Item {
    property real level: 0
    width: 120
    height: 220

    Rectangle {
        anchors.fill: parent
        border.color: "white"
        border.width: 2
        color: "transparent"
        radius: 6
    }

    Rectangle {
        width: parent.width
        height: (level / 100) * parent.height
        y: parent.height - height
        color: "#00bfff"

        Behavior on height {
            NumberAnimation { duration: 500 }
        }
    }
}
