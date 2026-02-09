import QtQuick
import QtQuick.Layouts
import WaterDashboard 1.0
import "components" // This allows the dashboard to see Tank, Pipe, and StatusCard

Item {
    id: dashboardRoot

    // Background Layer
    Rectangle {
        anchors.fill: parent
        color: "#121212" // Cleaner, deeper dark theme
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 20

        // Header Section [cite: 2]
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 64
            radius: 10
            color: "#1e1e1e"

            RowLayout {
                anchors.fill: parent
                anchors.margins: 15
                spacing: 12

                Text {
                    text: "ULTRAFILTRATION SYSTEM" // [cite: 3]
                    color: "white"
                    font.pixelSize: 22
                    font.bold: true
                }

                Item { Layout.fillWidth: true } // Spacer [cite: 4]

                // Operation Mode Buttons [cite: 5]
                Repeater {
                    model: [
                        { label: "AUTO",     mode: PlantController.Auto },
                        { label: "MANUAL",   mode: PlantController.Manual },
                        { label: "BACKWASH", mode: PlantController.Backwash },
                        { label: "CIP",      mode: PlantController.Cip }
                    ]

                    delegate: Rectangle {
                        width: 100; height: 32
                        radius: 6
                        color: (plant.mode === modelData.mode) ? "#2ecc71" : "#333333"

                        Text {
                            anchors.centerIn: parent
                            text: modelData.label
                            color: "white"
                            font.pixelSize: 11
                            font.bold: true
                        }

                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onClicked: {
                                if (modelData.mode === PlantController.Auto) plant.requestAuto()
                                else if (modelData.mode === PlantController.Manual) plant.requestManual()
                                else if (modelData.mode === PlantController.Backwash) plant.requestBackwash()
                                else plant.requestCip()
                            }
                        }
                    }
                }


            }
        }

        // 🔴 Alarm Banner (visible only if alarms exist)
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: plant.activeAlarms.length > 0 ? 44 : 0
            visible: plant.activeAlarms.length > 0
            radius: 10
            color: "#b71c1c"

            RowLayout {
                anchors.fill: parent
                anchors.margins: 12
                spacing: 10

                Text { text: "ALARM"; color: "white"; font.bold: true; font.pixelSize: 13 }

                Text {
                    text: plant.activeAlarms.length > 0 ? plant.activeAlarms[0] : ""
                    color: "white"
                    font.pixelSize: 13
                    elide: Text.ElideRight
                    Layout.fillWidth: true
                }

                // ACK button (demo)
                Rectangle {
                    width: 60; height: 24
                    radius: 6
                    color: "#ffffff22"

                    Text { anchors.centerIn: parent; text: "ACK"; color: "white"; font.pixelSize: 12 }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            if (plant.activeAlarms.length > 0)
                                plant.ackAlarm(plant.activeAlarms[0])
                        }
                    }
                }
            }
        }



        // Main Content Area [cite: 9]
        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 20

            // Left Side: Performance Analytics [cite: 10]
            Rectangle {
                Layout.preferredWidth: 300
                Layout.fillHeight: true
                radius: 12
                color: "#1e1e1e"

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 15
                    spacing: 15

                    Text {
                        text: "UF PERFORMANCE" // [cite: 12]
                        color: "#888888"
                        font.pixelSize: 12
                        font.bold: true
                    }

                    GridLayout {
                        columns: 2
                        columnSpacing: 10
                        rowSpacing: 10
                        Layout.fillWidth: true

                        // Data cards from components folder [cite: 14, 73]
                        StatusCard { title: "Feed Flow"; value: plant.feedFlow.toFixed(1); unit: "m³/h"; accent: "#4fc3f7" }
                        StatusCard { title: "Inlet PSI"; value: plant.inletPsi.toFixed(2); unit: "bar"; accent: "#2ecc71" }
                        StatusCard { title: "Outlet PSI"; value: plant.outletPsi.toFixed(2); unit: "bar"; accent: "#2ecc71" }
                        StatusCard { title: "TMP"; value: plant.tmp.toFixed(2); unit: "bar"; accent: "#ffb300" }
                        StatusCard { title: "Recovery"; value: "92"; unit: "%"; accent: "#ab47bc" }
                        StatusCard { title: "Health"; value: "GOOD"; unit: ""; accent: "#2ecc71" }

                    }
                    Item { Layout.fillHeight: true } // Push content up [cite: 11]
                }
            }

            // Right Side: Process Flow Visualization [cite: 21]
            Rectangle {
                id: processArea
                Layout.fillWidth: true
                Layout.fillHeight: true
                radius: 12
                color: "#181818" // Deep background for process area [cite: 24]

                Item {
                    id: canvas
                    anchors.fill: parent
                    anchors.margins: 30

                    // Flowing Pipes from components/Pipe.qml [cite: 27, 38]
                    Pipe { x: 50; y: 100; width: 350; direction: 1; running: true }
                    Pipe { x: 550; y: 280; width: 350; direction: 1; running: true }

                    // Tank from components/Tank.qml [cite: 29, 82]
                    Tank {
                        id: tank1
                        x: 400; y: 60
                        width: 150; height: 300
                        level: plant.tankLevel // Binding to property in Tank.qml [cite: 30, 82]
                    }

                    // Symbols from components folder [cite: 32, 80, 81]
                    SymbolPump { x: 80; y: 85 }
                    SymbolValve { x: 330; y: 85 }

                    // Dynamic Labels [cite: 34]
                    Text { x: 100; y: 130; text: "Feed Line"; color: "#666"; font.pixelSize: 11 }
                    Text { x: 450; y: 30; text: "Storage Tank"; color: "#666"; font.pixelSize: 11 }
                }
            }
        }
    }
}
