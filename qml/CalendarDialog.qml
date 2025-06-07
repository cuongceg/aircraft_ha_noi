import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls 1.4

Dialog {
    id: calendarDialog
    title: "Chọn ngày"
    modal: true
    standardButtons: Dialog.Ok | Dialog.Cancel

    Calendar {
        id: calendar
        anchors.fill: parent
    }

    property date selectedDate: calendar.selectedDate
}
