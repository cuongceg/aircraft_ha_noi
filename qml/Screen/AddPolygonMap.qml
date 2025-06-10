import QtQuick 2.15
import QtLocation 5.14
import QtPositioning 5.14
import QtQuick.Controls 2.15

Item {
    visible: true
    property int currentIndex: -1
    property var initialPathPoints: []
    property var aircraftIndex

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

    ListModel{
        id: polygonmodel
    }
    Map {
        id: changeRouteMap
        anchors.fill: parent
        plugin: osmPlugin
        center: QtPositioning.coordinate(21.0285, 105.8542)
        zoomLevel: 7
        activeMapType: changeRouteMap.supportedMapTypes[changeRouteMap.supportedMapTypes.length-1]
        gesture.enabled: currentIndex == -1
        MapItemView{
            z: polygon.z + 1
            model: polygonmodel
            delegate: MapQuickItem{
                anchorPoint: Qt.point(sourceItem.width/2, sourceItem.height/2)
                coordinate: QtPositioning.coordinate(model.coords.latitude, model.coords.longitude)
                sourceItem: Image {
                    width: 40
                    height: 40
                    source: "qrc:/assets/marker_begin.png"
                    MouseArea{
                        anchors.fill: parent
                        acceptedButtons: Qt.LeftButton
                        propagateComposedEvents: true
                        onPressed: {
                            currentIndex = index
                            mouse.accepted = false
                        }
                    }
                }
            }
        }
        MapPolygon{
            id: polygon
            border.color: "green"
            border.width: 10
        }
        MouseArea{
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton | Qt.RightButton | Qt.MiddleButton
            onClicked: {
                var point = Qt.point(mouse.x, mouse.y)
                var coord = changeRouteMap.toCoordinate(point);
                if (mouse.button == Qt.RightButton)
                    addMarker(coord)
                else if(mouse.button == Qt.MiddleButton)
                    removePathPoint(currentIndex)
            }
            onPositionChanged: {
                if (currentIndex != -1){
                    var point = Qt.point(mouse.x, mouse.y)
                    var coord = changeRouteMap.toCoordinate(point);
                    if(coord.isValid)
                        moveMarker(currentIndex, coord)
                }
            }
            onReleased: {
                if (mouse.button == Qt.LeftButton && currentIndex != -1){
                    var point = Qt.point(mouse.x, mouse.y)
                    var coord = changeRouteMap.toCoordinate(point);
                    if(coord.isValid)
                        moveMarker(currentIndex, coord)
                    currentIndex = -1;
                }
            }
        }

        Component.onCompleted: {
            loadInitialPath(initialPathPoints);
        }
    }

    Button {
        anchors.top:parent.top
        anchors.right: parent.right
        text: "Thêm polygon"
        onClicked: {
            polygonModel.addPolygon(getCurrentPath())
            stackView.pop()
        }
    }

    function loadInitialPath(coordsList) {
        polygonmodel.clear();
        for (var i = 0; i < coordsList.length; ++i) {
            addMarker(coordsList[i]);
        }
    }
    function moveMarker(index, coordinate){
        polygonmodel.set(index, {"coords": coordinate})
        var path = polygon.path;
        path[index] = coordinate
        polygon.path = path
    }
    function addMarker(coordinate){
        polygonmodel.append({"coords": coordinate})
        polygon.addCoordinate(coordinate)
    }
    function removePathPoint(index) {
        if (index >= 0 && index < pathModel.count) {
            polygonmodel.remove(index);
            polygon.removeCoordinate(index)
        }
    }
    function getCurrentPath() {
        var pathArray = [];
        for (var i = 0; i < polygonmodel.count; ++i) {
            var coord = polygonmodel.get(i).coords;
            pathArray.push({
                latitude: coord.latitude,
                longitude: coord.longitude
            });
        }
        return pathArray;
    }
}
