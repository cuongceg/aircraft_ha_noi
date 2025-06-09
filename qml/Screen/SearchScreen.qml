import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 1.4
import "../Component" as Component

Item {
    id: pageSettings
    property alias searchText: searchField.text
    property var selectedDate: null

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
                onAccepted:{
                    flightModel.searchFlights(searchText,selectedDate)
                }
            }

            Button {
                text: pageSettings.selectedDate === null ? "Chọn ngày" : Qt.formatDate(selectedDate, "dd/MM/yyyy")
                onClicked: {
                    calendarLoader.source = "../Component/CalendarDialog.qml"
                    calendarLoader.item.open()
                    calendarLoader.item.onAccepted.connect(function() {
                        pageSettings.selectedDate=calendarLoader.item.selectedDate
                        console.log("Ngày đã chọn:", calendarLoader.item.selectedDate)
                    })
                }
            }

            Button {
                text: "Tim kiem"
                onClicked: {
                    flightModel.searchFlights(searchText,selectedDate)
                }
            }
        }


        ListView {
            id: resultList
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 8
            model: flightModel
            delegate: Component.FlightCard {
                planeId: model.aircraftId
                inTime: Qt.formatDateTime(model.inTime, "hh:mm:ss dd/MM/yyyy")
                outTime: Qt.formatDateTime(model.outTime, "hh:mm:ss dd/MM/yyyy")
                startCoordinate: model.startPoint
                endCoordinate: model.endPoint
                route:model.route
                onViewDetails: {
                    stackView.push("RouteMap.qml", {
                                       routeCoordinates: route,
                                       startCoordinate: startCoordinate,
                                       endCoordinate: endCoordinate})
                }
            }
        }
    }

}
