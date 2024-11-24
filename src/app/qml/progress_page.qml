import QtQuick 2.7
import QtQuick.Controls 2.0
import Themes 1.0

Page {
    id: progress

    background: Rectangle {
        color: Theme.backgroundColor
    }

    Rectangle {
        anchors.centerIn: parent
        border.color: "black"
        border.width: 4
        color: Theme.backgroundColor
        height: parent.height - 30
        width: parent.width - 20

        Column {
            anchors.centerIn: parent
            spacing: 20

            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                color: Theme.mainTextColor
                font.family: Theme.font_family
                font.pixelSize: 24
                text: "Setting up application..."
            }
            ProgressBar {
                id: progressBar

                anchors.horizontalCenter: parent.horizontalCenter
                objectName: "progressBar"
                to: 100
                value: 0
                width: progress.width * 0.8
            }
        }
    }
}
