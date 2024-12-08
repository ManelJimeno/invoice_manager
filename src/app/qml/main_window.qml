import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Themes 1.0

ApplicationWindow {
    id: mainWindow

    property string statusBarText: "Ready"

    color: Theme.backgroundColor
    height: 768
    title: qsTr("Invoice manager")
    visible: true
    width: 1024

    footer: Rectangle {
        border.color: "#D0D0D0"
        border.width: 2
        color: mainMenu.background.color
        height: 30
        width: parent.width

        Text {
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.verticalCenter: parent.verticalCenter
            text: mainWindow.statusBarText
        }
    }

    MainMenu {
        id: mainMenu
        width: parent.width
    }
}
