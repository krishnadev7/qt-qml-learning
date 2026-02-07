import QtQuick

Item {
    property bool running: false
    width: 300
    height: 20

    Rectangle {
        anchors.fill: parent
        radius: 10
        color: "#888"   // grey pipe
    }

    Rectangle {
        width: 40
        height: parent.height
        radius: 10
        color: "#00bfff"
        visible: running

        SequentialAnimation on x {
            loops: Animation.Infinite
            running: parent.running
            NumberAnimation { from: 0; to: parent.width; duration: 1000 }
        }
    }
}
