import QtQuick 2.15
import QtQuick.Window 2.15
import QtLocation 5.15
import QtPositioning 5.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ApplicationWindow {
    visible: true
    width: 800
    height: 600
    title: "Aircraft Ha Noi"
    property int selectedAircraftIndex: -1
    property var selectedCoordinate: undefined
    property string selectingPoint: ""
    property var startCoordinate
    property var endCoordinate
    property bool isHideMarker: true
    Plugin {
        id: osmPlugin
        name: "osm"

        PluginParameter { name: "osm.mapping.providersrepository.disabled"; value: true }
        PluginParameter { name: "osm.mapping.highdpi_tiles"; value: true }
        PluginParameter { name: "osm.mapping.custom.host"; value: "https://tile.openstreetmap.org/" }
        PluginParameter { name: "osm.mapping.custom.tileSize"; value: 256 }
        PluginParameter { name: "osm.mapping.custom.minZoomLevel"; value: 1 }
        PluginParameter { name: "osm.mapping.custom.maxZoomLevel"; value: 19 }
    }

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: pageHome
    }


    ToolBar {
        id: toolbar
        anchors.top: parent.top
        width: parent.width

        Row{
            spacing: 10
            anchors.centerIn: parent

            Button{
                text:"Home"
                onClicked: {
                    if(stackView.currentItem !== stackView.find(pageHome)){
                        stackView.replace(pageHome)
                    }
                }
            }

            Button{
                text: "Settings"
                onClicked: {
                    if(stackView.currentItem !== stackView.find(pageSettings)){
                        stackView.replace(pageSettings)
                    }
                }
            }

            Button{
                text:"About"
                onClicked: {
                    if(stackView.currentItem !== stackView.find(pageAbout)){
                        stackView.replace(pageAbout)
                    }
                }
            }
        }
    }

    Page {
        id: pageHome
        Map {
            id: mapview
            anchors.fill: parent
            plugin: osmPlugin
            center: QtPositioning.coordinate(21.0285, 105.8542)
            zoomLevel: 7
            activeMapType: mapview.supportedMapTypes[mapview.supportedMapTypes.length-1]

            MouseArea{
                anchors.fill: parent
                z:1
                acceptedButtons: Qt.LeftButton
                onClicked:{
                    if (selectingPoint === "start") {
                        startCoordinate = mapview.toCoordinate(Qt.point(mouse.x, mouse.y))
                        selectingPoint = ""
                        selectHint.visible = false
                        editDialog.visible = true
                    } else if (selectingPoint === "end") {
                        endCoordinate = mapview.toCoordinate(Qt.point(mouse.x, mouse.y))
                        selectingPoint = ""
                        selectHint.visible = false
                        editDialog.visible = true
                    }else{
                        selectedAircraftIndex=-1
                        selectedCoordinate=undefined}
                }
            }
            MapPolygon {
                path: hanoiPolygon
                color: "#2200ff00"
                border.width: 2
                border.color: "green"
            }
            Repeater {
                model: aircraftModel
                delegate: MapQuickItem {
                    coordinate: model.coordinate
                    z:2
                    rotation: model.rotation
                    anchorPoint.x: image.width / 2
                    anchorPoint.y: image.height / 2

                    sourceItem: Image {
                        id:image
                        source: model.insideHanoi ? (selectedAircraftIndex !== index  ? "qrc:/assets/aircraft_red.png" : "qrc:/assets/aircraft_orange.png")
                                                  : (selectedAircraftIndex !== index ? "qrc:/assets/aircraft_green.png" :"qrc:/assets/aircraft_blue.png")
                        width: 30
                        height: 30
                        smooth: true
                        antialiasing: true
                    }
                    MouseArea {
                        anchors.fill: parent
                        acceptedButtons: Qt.LeftButton | Qt.RightButton
                        z:3
                        onClicked:(mouse) => {
                                      if(mouse.button === Qt.LeftButton){
                                          if (selectedAircraftIndex === index) {
                                              selectedAircraftIndex = -1;
                                              selectedCoordinate = undefined;
                                          } else {
                                              selectedAircraftIndex = index;
                                              selectedCoordinate = model.coordinate;
                                          }
                                      }else{
                                          contextMenu.x = mouse.x
                                          contextMenu.y = mouse.y
                                          contextMenu.visible = true
                                      }
                                  }
                    }

                    Popup {
                        id: contextMenu
                        width: 150
                        height: 80
                        modal: true
                        focus: true
                        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
                        background: Rectangle {
                            color: "white"
                            radius: 8
                            border.color: "gray"
                            border.width: 1
                        }
                        contentItem: ColumnLayout{
                            anchors.fill: parent
                            Button {
                                text: "Chạy lại"
                                width:100
                                height:20
                                background: Rectangle {
                                    color: "white"
                                }
                                onClicked: {
                                    contextMenu.visible = false
                                    aircraftModel.resetProgress(index)
                                }
                            }
                            Button {
                                text: "Chỉnh sửa lộ trình"
                                width:150
                                height:20
                                background: Rectangle {
                                    color: "white"
                                }
                                onClicked: {
                                    contextMenu.visible = false
                                    editDialog.visible = true
                                    isHideMarker = false
                                    startCoordinate=model.startPoint
                                    endCoordinate=model.endPoint
                                }
                            }
                        }
                    }

                    onCoordinateChanged: {
                        if (index === selectedAircraftIndex)
                            selectedCoordinate = coordinate;
                    }
                }
            }

            Popup {
                id: editDialog
                width: 300
                height: 150
                anchors.centerIn: Overlay.overlay
                modal: true
                focus: true

                background: Rectangle {
                    color: "white"
                    radius: 12
                    border.color: "gray"
                    border.width: 1
                }

                contentItem: Column{
                    anchors.fill: parent
                    anchors.margins: 16
                    spacing: 10

                    // Điểm bắt đầu
                    RowLayout{
                        spacing: 10
                        Text {
                            text: "Điểm bắt đầu: " + (startCoordinate
                                ? startCoordinate.latitude.toFixed(2) + ", " + startCoordinate.longitude.toFixed(2)
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
                                selectingPoint = "start"
                                editDialog.visible = false
                                selectHint.visible = true
                            }
                        }
                    }

                    // End point
                    RowLayout{
                        spacing: 10
                        Text {
                            text: "Điểm kết thúc: " + (endCoordinate
                                ? endCoordinate.latitude.toFixed(2) + ", " + endCoordinate.longitude.toFixed(2)
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
                                selectingPoint = "end"
                                editDialog.visible = false
                                selectHint.visible = true
                            }
                        }
                    }

                    //Save button
                    Button {
                        text: "Lưu"
                        width:60
                        height:40
                        background: Rectangle {
                            color: "lightblue"
                            radius: 8
                        }
                        anchors.horizontalCenter: parent.horizontalCenter
                        onClicked: {
                            aircraftModel.updateFlights(selectedAircraftIndex,startCoordinate,endCoordinate)
                            console.log("I'm here")
                            isHideMarker = true
                            editDialog.visible = false
                        }
                    }
                }
            }


            MapQuickItem {
                id: markerStart
                anchorPoint.x: imageMarkerBegin.width / 2
                anchorPoint.y: imageMarkerBegin.height
                coordinate: startCoordinate
                visible: startCoordinate !== undefined && !isHideMarker
                sourceItem: Image {
                    id: imageMarkerBegin
                    source: "qrc:/assets/marker_begin.png"
                    width: 32
                    height: 32
                }
            }

            MapQuickItem {
                id: markerEnd
                anchorPoint.x: imageMarkerEnd.width / 2
                anchorPoint.y: imageMarkerEnd.height
                coordinate: endCoordinate
                visible: endCoordinate !== undefined && !isHideMarker
                sourceItem: Image {
                    id: imageMarkerEnd
                    source: "qrc:/assets/marker_end.png"
                    width: 32
                    height: 32
                }
            }


            Rectangle {
                id: selectHint
                width: parent.width
                height: 30
                color: "#ffc"
                visible: false
                z: 99
                anchors.top: parent.top
                anchors.topMargin: 40

                Text {
                    anchors.centerIn: parent
                    text: "Click lên bản đồ để chọn " + (selectingPoint === "start" ? "điểm bắt đầu" : "điểm kết thúc")
                    color: "black"
                }
            }

            Rectangle {
                id: statusBar
                width: parent.width
                height: 30
                color: "#222"
                opacity: selectedAircraftIndex >= 0 ? 0.9 : 0
                anchors.bottom: parent.bottom
                z: 100

                Label {
                    anchors.centerIn: parent
                    id: coordLabel
                    text: selectedCoordinate
                          ? "Kinh độ: " + selectedCoordinate.longitude.toFixed(2) +
                            "  |  Vĩ độ: " + selectedCoordinate.latitude.toFixed(2)
                          : ""
                    color: "white"
                }
            }
        }
    }

    Page {
        id: pageSettings
        title: "Settings Page"
        Rectangle {
            width: parent.width
            height: parent.height
            color: "lightgreen"

            Text {
                anchors.centerIn: parent
                text: "Settings Page"
                font.bold: true
                font.pointSize: 24
            }
        }
    }

    Page {
        id: pageAbout
        title: "About Page"
        Rectangle {
            width: parent.width
            height: parent.height
            color: "lightyellow"

            Text {
                anchors.centerIn: parent
                text: "About Page"
                font.bold: true
                font.pointSize: 24
            }
        }
    }
}
