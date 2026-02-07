import QtQuick 2.15

Rectangle {
    signal tileClicked(string label)

    width: 200
    height: 80
    radius: 8

    property string labelText: "Label"
    property string valueText: "Value"
    property color tileColor: "#2e7d32"

    color: tileColor

    Column {
        anchors.centerIn: parent
        spacing: 4

        Text {
            text: labelText
            color: "white"
            font.pixelSize: 14
        }

        Text {
            text: valueText
            color: "white"
            font.pixelSize: 18
            font.bold: true
        }
    }

    MouseArea {
        anchors.fill: parent

        onClicked: {
            tileClicked(labelText)
        }
    }

}
