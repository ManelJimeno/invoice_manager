import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    property string statusBarText: "Ready"

    height: 768
    title: "Invoice manager"
    visible: true
    width: 1024

    footer: Rectangle {
        border.color: "#D0D0D0"
        border.width: 2
        color: "#F0F0F0"
        height: 30
        width: parent.width

        Text {
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.verticalCenter: parent.verticalCenter
            text: statusBarText
        }
    }
    menuBar: MenuBar {
        Menu {
            title: "File"

            MenuItem {
                text: "Exit"

                onTriggered: {
                    Qt.quit();
                }

                HoverHandler {
                    onHoveredChanged: {
                        if (hovered) {
                            statusBarText = "Exit the application";
                        } else {
                            statusBarText = "Ready";
                        }
                    }
                }
            }
        }
        Menu {
            title: "Help"

            MenuItem {
                text: "About this..."
                HoverHandler {
                    onHoveredChanged: {
                        if (hovered) {
                            statusBarText = "Learn more about the application";
                        } else {
                            statusBarText = "Ready";
                        }
                    }
                }
                onTriggered: {
                    mainWindow.showAboutDialog();
                }
            }
        }
    }
}
