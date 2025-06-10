import QtQuick 2.15
import QtQuick.Controls 2.15
import QtLocation 5.15
import QtPositioning 5.15

Item {
    id: routeMap
    property var routeCoordinates: []
    property var startCoordinate
    property var endCoordinate

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
        id: mapView
        anchors.fill: parent
        plugin: osmPlugin
        zoomLevel: 10

        center: startCoordinate

        MapPolyline {
            id: routeLine
            line.width: 4
            line.color: "blue"

            path: routeCoordinates.map(function(coord) {
                return QtPositioning.coordinate(coord.latitude, coord.longitude);
            })
        }

        MapQuickItem {
            id: markerStart
            anchorPoint.x: imageMarkerBegin.width / 2
            anchorPoint.y: imageMarkerBegin.height
            coordinate: startCoordinate
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
            sourceItem: Image {
                id: imageMarkerEnd
                source: "qrc:/assets/marker_end.png"
                width: 32
                height: 32
            }
        }
    }
}
