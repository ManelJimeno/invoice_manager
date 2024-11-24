import QtQuick 2.15
import QtQuick.Controls 2.15
import Themes 1.0

Window {
    id: root

    color: Theme.backgroundColor
    flags: Qt.FramelessWindowHint
    height: 500
    title: qsTr("Invoice manager")
    visible: false
    width: 420

    Component.onCompleted: {
        stackView.push("qrc:/progress_page.qml");
    }

    Column {
        anchors.fill: parent
        spacing: 5

        Rectangle {
            id: header

            color: "transparent"
            height: 250
            width: parent.width

            Column {
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 20

                Text {
                    id: title

                    anchors.horizontalCenter: parent.horizontalCenter
                    color: Theme.mainTextColor
                    font.family: Theme.font_family
                    font.pointSize: 36
                    text: qsTr("Invoice manager")
                }
                Text {
                    id: iconText

                    anchors.horizontalCenter: parent.horizontalCenter
                    color: Theme.mainAppColor
                    font.family: Theme.awesome
                    font.pointSize: 112
                    text: qsTr("\uf571")
                }
            }
        }
        StackView {
            id: stackView

            focus: true
            height: 250
            objectName: "stackView"
            width: parent.width
        }
    }
    Popup {
        id: popup

        property alias popMessage: message.text

        closePolicy: Popup.CloseOnPressOutside
        focus: true
        modal: true
        y: (root.height - 60)

        background: Rectangle {
            color: Theme.popupBackgroundColor
            implicitHeight: 60
            implicitWidth: root.width
        }

        onOpened: popupClose.start()

        Text {
            id: message

            anchors.centerIn: parent
            color: Theme.popupTextColor
            font.pointSize: 12
        }
    }
    Timer {
        id: popupClose

        interval: 2000

        onTriggered: popup.close()
    }
}
