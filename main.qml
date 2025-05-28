import QtQuick 2.15
import QtQuick.Window 2.15
import QtLocation 5.15
import QtPositioning 5.15
import QtQuick.Controls 2.15

ApplicationWindow {
    visible: true
    width: 800
    height: 600
    title: "Drawer Navigation Example"

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
        initialItem: pageHome // Màn hình ban đầu

        // Chuyển đến màn hình khi chọn một mục trong Drawer
        function navigateTo(page) {
            stackView.push(page)
        }
    }

    // Drawer cho các mục điều hướng
    Drawer {
        id: drawer
        width: parent.width * 0.75
        height: parent.height
        clip: true

        // Nội dung của Drawer
        ListView {
            width: drawer.width
            height: parent.height

            model: ListModel {
                ListElement { name: "Home" }
                ListElement { name: "Settings" }
                ListElement { name: "About" }
            }

            delegate: Item {
                width: parent.width
                height: 60

                Rectangle {
                    width: parent.width
                    height: 60
                    color: "lightgray"
                    border.color: "black"
                    radius: 5

                    Text {
                        anchors.centerIn: parent
                        text: model.name
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            if (model.name === "Home") {
                                stackView.replace(pageHome)
                            } else if (model.name === "Settings") {
                                stackView.replace(pageSettings)
                            } else if (model.name === "About") {
                                stackView.replace(pageAbout)
                            }
                            drawer.close()
                        }
                    }
                }
            }
        }
    }

    // Header với nút Drawer
    ToolBar {
        id: toolbar
        anchors.top: parent.top
        width: parent.width

        ToolButton {
            text: "☰"  // Nút Drawer
            onClicked: drawer.open()
        }

        ToolButton {
            text: "Home"
            anchors.right: parent.right
            onClicked: {
                stackView.push(pageHome)
            }
        }
    }

    // Các màn hình của ứng dụng
    Page {
        id: pageHome
        Map {
            id: mapview
            anchors.fill: parent
            plugin: osmPlugin
            center: QtPositioning.coordinate(21.0285, 105.8542) // Hà Nội
            zoomLevel: 12
            activeMapType: mapview.supportedMapTypes[mapview.supportedMapTypes.length-1]
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
