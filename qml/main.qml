import QtQuick 2.15
import QtQuick.Window 2.15
import QtLocation 5.15
import QtPositioning 5.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "Screen" as Screen

ApplicationWindow {
    visible: true
    width: 800
    height: 600
    title: "Aircraft Ha Noi"

    property int currentTab: 0

    Column {
        anchors.fill: parent

        TabBar {
            id: tabBar
            width: parent.width

            TabButton {
                text: "Bản đồ"
                onClicked: stackView.switchTo(0)
            }
            TabButton {
                text: "Cài đặt"
                onClicked: stackView.switchTo(1)
            }
        }

        StackView {
            id: stackView
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.left: parent.left
            anchors.right: parent.right
            height: parent.height - tabBar.height

            initialItem: mapScreen

            function switchTo(index) {
                currentTab = index
                switch(index) {
                case 0: replace(mapScreen); break;
                case 1: replace(settingsScreen); break;
                }
            }
        }
    }

    Component {
        id: mapScreen

        Screen.MapScreen{}
    }

    Component {
        id: settingsScreen

        Screen.SearchScreen{}
    }
}
