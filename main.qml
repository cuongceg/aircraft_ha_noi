import QtQuick 2.15
import QtQuick.Window 2.15
import QtLocation 5.15
import QtPositioning 5.15
import QtQuick.Controls 2.15

ApplicationWindow {
    visible: true
    width: 800
    height: 600
    title: "Aircraft Ha Noi"

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
            center: QtPositioning.coordinate(21.0285, 105.8542) // Hà Nội
            zoomLevel: 12
            activeMapType: mapview.supportedMapTypes[mapview.supportedMapTypes.length-1]
            MapPolygon {
                path: hanoiPolygon
                color: "#2200ff00"
                border.width: 2
                border.color: "green"
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
