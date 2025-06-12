import QtQuick 2.15
import QtLocation 5.15
import QtPositioning 5.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    id:map
    property int selectedAircraftIndex: -1
    property var selectedCoordinate: undefined
    property var selectedPlaneId: undefined
    property var selectedStartTime: undefined
    property var selectedEndTime: undefined
    property var selectedRoutes: []
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

    Map {
        id: mapview
        anchors.fill: parent
        plugin: osmPlugin
        center: QtPositioning.coordinate(21.0285, 105.8542)
        zoomLevel: 7
        activeMapType: mapview.supportedMapTypes[mapview.supportedMapTypes.length-1]

        Loader{
            id: addLoader
        }

        MouseArea{
            anchors.fill: parent
            z:1
            acceptedButtons: Qt.LeftButton | Qt.RightButton
            onClicked:{
                if(mouse.button === Qt.LeftButton){
                    selectedAircraftIndex=-1
                    selectedCoordinate=undefined
                    routePath.path = []
                    selectedRoutes = []
                    markerStart.visible=false
                    markerEnd.visible=false
                }else if(mouse.button === Qt.RightButton){
                    mapDialog.visible = true
                    mapDialog.x=mouse.x
                    mapDialog.y=mouse.y
                }
            }
        }
        MapPolygon {
            path: hanoiPolygon
            color: "#2200ff00"
            border.width: 2
            border.color: "green"
        }

        Instantiator{
            model: polygonModel
            delegate: MapPolygon{
                path: model.path.map(c => QtPositioning.coordinate(c.latitude, c.longitude))
                color: "#2200ff00"
                border.width: 2
                z:100
                border.color: "green"

                MouseArea{
                    anchors.fill: parent
                    hoverEnabled: true
                    z:101
                    acceptedButtons: Qt.LeftButton | Qt.RightButton
                    onClicked: {
                        console.log("Tap")
                        stackView.push("AddPolygonMap.qml",{
                                           isChanged:true,
                                           initialPathPoints :model.path.map(c => QtPositioning.coordinate(c.latitude, c.longitude)),
                                           aircraftIndex: index
                                       })
                    }
                }
            }
            onObjectAdded: mapview.addMapItem(object)
            onObjectRemoved: mapview.removeMapItem(object)
        }


        Instantiator {
            model: aircraftModel
            delegate: MapQuickItem {
                coordinate: model.coordinate
                rotation: model.rotation
                z:2
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
                                          routePath.path = []
                                          selectedRoutes = []
                                          markerStart.visible=false
                                          markerEnd.visible=false
                                      } else {
                                          selectedAircraftIndex = index;
                                          selectedCoordinate = model.coordinate;
                                          routePath.path = model.routes
                                          selectedRoutes = model.routes
                                          selectedPlaneId = model.planeId
                                          selectedEndTime = Qt.formatDateTime(model.endTime, "hh:mm:ss dd/MM/yyyy")
                                          selectedStartTime = Qt.formatDateTime(model.startTime, "hh:mm:ss dd/MM/yyyy")
                                          markerStart.coordinate = model.startPoint
                                          markerEnd.coordinate = model.endPoint
                                          markerStart.visible=true
                                          markerEnd.visible=true
                                      }
                                  }else{
                                      selectedAircraftIndex = index;
                                      selectedCoordinate = model.coordinate;
                                      selectedRoutes = model.routes
                                      selectedPlaneId = model.planeId
                                      selectedEndTime = Qt.formatDateTime(model.endTime, "hh:mm:ss dd/MM/yyyy")
                                      selectedStartTime = Qt.formatDateTime(model.startTime, "hh:mm:ss dd/MM/yyyy")
                                      contextMenu.x = mouse.x
                                      contextMenu.y = mouse.y
                                      contextMenu.visible = true
                                  }
                              }
                }

                Popup {
                    id: contextMenu
                    width: 160
                    height: 150
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
                            text: "Bat dau"
                            width:100
                            height:20
                            background: Rectangle {
                                color: "white"
                            }
                            onClicked: {
                                contextMenu.visible = false
                                aircraftModel.startAircraft(index)
                            }
                        }
                        Button {
                            text: "Dung"
                            width:100
                            height:20
                            background: Rectangle {
                                color: "white"
                            }
                            onClicked: {
                                contextMenu.visible = false
                                aircraftModel.stopAircraft(index)
                            }
                        }
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
                                contextMenu.visible=false
                                stackView.push("ChangeRouteMap.qml",{
                                                   initialPathPoints :selectedRoutes,
                                                   aircraftIndex: selectedAircraftIndex
                                               })
                            }
                        }
                    }
                }

                onCoordinateChanged: {
                    if (index === selectedAircraftIndex)
                        selectedCoordinate = coordinate;
                }
            }
            onObjectAdded: mapview.addMapItem(object)
            onObjectRemoved: mapview.removeMapItem(object)
        }

        Popup{
            id: mapDialog
            width: 150
            height: 80
            modal: true
            focus: true

            background: Rectangle {
                color: "white"
                radius: 12
                border.color: "gray"
                border.width: 1
            }

            contentItem: ColumnLayout{
                anchors.fill: parent
                spacing:4
                Button {
                    text: "Them may bay"
                    width:100
                    height:20
                    background: Rectangle {
                        color: "white"
                    }
                    onClicked: {
                        stackView.push("ChangeRouteMap.qml",{
                                       isCreated: true
                                       })
                    }
                }

                Button {
                    text: "Them vung"
                    width:100
                    height:20
                    background: Rectangle {
                        color: "white"
                    }
                    onClicked: {
                        mapDialog.visible=false
                        stackView.push("AddPolygonMap.qml")
                    }
                }
            }
        }


        MapPolyline {
            id: routePath
            line.width: 4
            line.color: "#646464"
            path: []
        }

        MapQuickItem {
            id: markerStart
            anchorPoint.x: imageMarkerBegin.width / 2
            anchorPoint.y: imageMarkerBegin.height
            coordinate: QtPositioning.coordinate(0, 0)
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
            coordinate: QtPositioning.coordinate(0, 0)
            sourceItem: Image {
                id: imageMarkerEnd
                source: "qrc:/assets/marker_end.png"
                width: 32
                height: 32
            }
        }

        // Rectangle {
        //     id: selectHint
        //     width: parent.width
        //     height: 30
        //     color: "#ffc"
        //     visible: false
        //     z: 99
        //     anchors.top: parent.top
        //     anchors.topMargin: 20

        //     Text {
        //         anchors.centerIn: parent
        //         text: "Click lên bản đồ để chọn " + (selectingPoint === "start" ? "điểm bắt đầu" : (selectingPoint === "end" ? "điểm kết thúc" :""))
        //               + (selectingPointDialog === "start" ? "điểm bắt đầu" : (selectingPointDialog === "end"?"điểm kết thúc":""))
        //         color: "black"
        //     }
        // }

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
                      ? "Ma may bay:"+ selectedPlaneId +"| Kinh độ: " + selectedCoordinate.longitude.toFixed(2) +
                        "  |  Vĩ độ: " + selectedCoordinate.latitude.toFixed(2) + "| Thoi gian bat dau: " +selectedStartTime+"| Thoi gian ket thuc: "+selectedEndTime
                      : ""
                color: "white"
            }
        }
    }
}

