import QtQuick 2.15
import QtQuick.Controls 2.15
import QtPositioning 5.15
import QtQuick.Layouts 1.15

Dialog {
    id: root
    modal: true
    width: 250
    height:implicitHeight
    title: "Tạo chuyến bay mới"
    x: (ApplicationWindow.window.width - width) / 2
    y: (ApplicationWindow.window.height - height) / 2

    property string flightId: ""
    property var startPoint: undefined
    property var endPoint: undefined

    signal saveClicked(string flightId, var startPoint, var endPoint)
    signal chooseStartPoint()
    signal chooseEndPoint()
    signal cancelClicked()

    ColumnLayout{
        spacing: 12

        RowLayout{
            spacing: 10
            Text {
                text: "Mã máy bay"
            }
            TextField {
                id: flightIdInput
                placeholderText: "Nhập mã máy bay"
                text: root.flightId
                onTextChanged: root.flightId = text
                background: Rectangle {
                    color: "white"
                    radius: 12
                    border.color: "grey"
                    border.width: 1
                }
            }
        }

        RowLayout{
            spacing: 10
            Text {
                text: "Điểm bắt đầu: " + (root.startPoint
                    ? root.startPoint.latitude.toFixed(2) + ", " + root.startPoint.longitude.toFixed(2)
                    : "Chưa chọn")
                color: "black"
            }
            Button {
                text: "\u270E"
                width:20
                height:20
                background: Rectangle {
                        color: "lightblue"
                        radius: 8
                }
                onClicked: {
                    root.chooseStartPoint()
                    root.close()
                }
            }
        }

        RowLayout{
            spacing: 10
            Text {
                text: "Điểm kết thúc: " + (root.endPoint
                    ? root.startPoint.latitude.toFixed(2) + ", " + root.startPoint.longitude.toFixed(2)
                    : "Chưa chọn")
                color: "black"
            }
            Button {
                text: "\u270E"
                width:20
                height:20
                background: Rectangle {
                    color: "lightblue"
                    radius: 8
                }
                onClicked:{
                    root.chooseEndPoint()
                    root.close()
                }
            }
        }

        Row {
            spacing: 10
            Button {
                text: "Hủy"
                onClicked: {
                    root.cancelClicked()
                    root.close()
                }
            }

            Button {
                text: "Lưu"
                enabled: root.flightId.length > 0 && startPoint !== undefined && endPoint !== undefined
                onClicked: {
                    root.saveClicked(flightId, startPoint, endPoint)
                    root.close()
                }
            }
        }
    }
}
