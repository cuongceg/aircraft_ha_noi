import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 1.4
import "../Component" as Component

Item {
    id: pageSettings
    property alias searchText: searchField.text
    property var selectedDate: null
    signal searchRequested(string searchText)

    Loader {
        id: calendarLoader
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 12

        RowLayout {
            width: parent.width
            spacing: 8

            TextField {
                id: searchField
                Layout.fillWidth: true
                placeholderText: "Nhập mã chuyến bay hoặc mã máy bay..."
                //onAccepted: searchRequested(searchField.text, fromDate, toDate)
            }

            Button {
                text: pageSettings.selectedDate === null ? "Chọn ngày" : Qt.formatDate(selectedDate, "dd/MM/yyyy")
                onClicked: {
                    calendarLoader.source = "../Component/CalendarDialog.qml"
                    calendarLoader.item.open()
                    calendarLoader.item.onAccepted.connect(function() {
                        pageSettings.selectedDate=new Date()
                        console.log("Ngày đã chọn:", calendarLoader.item.selectedDate)
                    })
                }
            }


            Button {
                text: "Tim kiem"
                onClicked: {
                    console.log("Tim kiem")
                }
            }
        }

        ListView {
            id: resultList
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 8
            model: flightListModel
            delegate: Component.FlightCard {
                title: model.flightId
                subtitle: model.timeInfo
                onViewDetails: {
                    // Emit signal or navigate to map view
                    console.log("Xem chi tiết:", model.flightId)
                }
            }
        }
    }

    ListModel {
        id: flightListModel
        ListElement { flightId: "VN123"; timeInfo: "12:00 - 14:00" }
        ListElement { flightId: "VN456"; timeInfo: "13:15 - 15:00" }
    }
}
