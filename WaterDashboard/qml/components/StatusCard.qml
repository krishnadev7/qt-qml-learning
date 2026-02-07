import QtQuick
import QtQuick.Layouts

Rectangle {
    id: card
    implicitWidth: 140
    implicitHeight: 80
    radius: 8
    color: "#2a2a2a"

    property string title: ""
    property string value: ""
    property string unit: ""
    property color accent: "cyan"

    Rectangle {
        width: 4
        anchors { left: parent.left; top: parent.top; bottom: parent.bottom }
        color: card.accent
        radius: 2
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 12
        spacing: 4

        Text { text: card.title.toUpperCase(); color: "#888"; font.pixelSize: 10; font.bold: true }
        RowLayout {
            spacing: 4
            Text { text: card.value; color: "white"; font.pixelSize: 20; font.bold: true }
            Text { text: card.unit; color: "#666"; font.pixelSize: 11; Layout.alignment: Qt.AlignBottom }
        }
    }
}
