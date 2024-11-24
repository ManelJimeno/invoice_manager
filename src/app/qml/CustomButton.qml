import QtQuick 2.6
import QtQuick.Controls 2.0

Button {
    id: control

    property color baseColor
    property color borderColor
    property alias name: control.text

    font.pointSize: 16
    text: qsTr("Log In")

    background: Rectangle {
        id: bgrect

        border.color: control.borderColor
        color: control.baseColor //"#6fda9c"
        implicitHeight: 50
        implicitWidth: 100
        opacity: control.down ? 0.7 : 1
        radius: height / 2
    }
    contentItem: Text {
        color: control.down ? "#ffffff" : "#ffffff"
        elide: Text.ElideRight
        font: control.font
        horizontalAlignment: Text.AlignHCenter
        opacity: enabled ? 1.0 : 0.3
        text: control.text
        verticalAlignment: Text.AlignVCenter
    }
}
