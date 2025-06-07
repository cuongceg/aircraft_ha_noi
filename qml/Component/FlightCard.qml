import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Material 2.15

Item {
    id: root
    width: parent.width
    height: 80

    signal viewDetails()

    Rectangle {
        anchors.fill: parent
        color: Material.background
        radius: 12
        border.color: "#ccc"
        border.width: 1

        RowLayout {
            anchors.fill: parent
            anchors.margins: 12
            spacing: 10

            ColumnLayout {
                Layout.fillWidth: true
                spacing: 4

                Label {
                    text: "Ma may bay"
                    font.bold: true
                    font.pointSize: 14
                    Layout.alignment: Qt.AlignHCenter
                }

                Label {
                    text: root.title
                    font.pointSize: 13
                    color: "#666"
                    Layout.alignment: Qt.AlignHCenter
                }
            }

            ColumnLayout {
                Layout.fillWidth: true
                spacing: 4

                Label {
                    text: "Thoi gian vao vung"
                    font.bold: true
                    font.pointSize: 14
                    Layout.alignment: Qt.AlignHCenter
                }

                Label {
                    text: root.subtitle
                    font.pointSize: 13
                    color: "#666"
                    Layout.alignment: Qt.AlignHCenter
                }
            }

            ColumnLayout {
                Layout.fillWidth: true
                spacing: 4

                Label {
                    text: "Thoi gian ra vung"
                    font.bold: true
                    font.pointSize: 16
                    Layout.alignment: Qt.AlignHCenter
                }

                Label {
                    text: root.subtitle
                    font.pointSize: 13
                    color: "#666"
                    Layout.alignment: Qt.AlignHCenter
                }
            }

            Button {
                text:"Xem hanh trinh"
                onClicked: root.viewDetails()
                background: Rectangle {
                    color: "lightblue"
                    radius: 20
                }
            }
        }
    }

    property string title
    property string subtitle
}
