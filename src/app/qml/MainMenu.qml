import QtQuick 2.15
import QtQuick.Controls 2.15

MenuBar {
    Menu {
        title: "&Application"
        Action {
            text: "Settings"
            onTriggered: console.log("Settings triggered")
            icon.source: "images/settings_24.svg"
        }
        MenuSeparator {}
        Action {
            text: "Exit"
            onTriggered: Qt.quit()
            icon.source: "images/logout_24.svg"
            shortcut: StandardKey.Quit;
        }
    }
    Menu {
        title: "&Help"
        Action {
            text: "About this..."
            onTriggered: controller.showAboutDialog()
            icon.source: "images/info_24.svg"
        }
    }
}
