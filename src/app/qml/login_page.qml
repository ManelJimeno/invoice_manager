import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.1
import Themes 1.0

Page {
    id: loginPage

    signal registerClicked

    function login() {
        let loginResult;
        if (loginPassword.text === "") {
            loginResult = initDialog.checkUser(loginUsername.text);
        } else {
            loginResult = initDialog.login(loginUsername.text, loginPassword.text);
        }
        switch (loginResult) {
        case 3:
            return true;
        case 0:
            popup.popMessage = "You're not login";
            break;
        case 1:
            popup.popMessage = "The user doesn't exists";
            break;
        case 2:
            popup.popMessage = "Incorrect password";
            break;
        }
        popup.open();
        return false;
    }

    background: Rectangle {
        color: Theme.backgroundColor
    }

    ColumnLayout {
        spacing: 15
        width: parent.width

        TextField {
            id: loginUsername

            Layout.alignment: Qt.AlignHCenter
            Layout.preferredWidth: parent.width - 20
            color: Theme.mainTextColor
            focus: true
            font.family: Theme.font_family
            font.pointSize: 14
            leftPadding: 30
            placeholderText: qsTr("User name")

            background: Rectangle {
                color: "transparent"
                implicitHeight: 50
                implicitWidth: 200
                radius: implicitHeight / 2

                Text {
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    color: Theme.mainAppColor
                    font.family: Theme.awesome
                    font.pointSize: 14
                    leftPadding: 10
                    text: "\uf007"
                }
                Rectangle {
                    anchors.bottom: parent.bottom
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: Theme.mainAppColor
                    height: 1
                    width: parent.width - 10
                }
            }

            onEditingFinished: {
                if (!loginPage.login()) {
                    loginUsername.clear();
                    forceActiveFocus();
                }
            }
        }
        TextField {
            id: loginPassword

            Layout.alignment: Qt.AlignHCenter
            Layout.preferredWidth: parent.width - 20
            color: Theme.mainTextColor
            echoMode: TextField.PasswordEchoOnEdit
            font.family: Theme.font_family
            font.pointSize: 14
            leftPadding: 30
            placeholderText: qsTr("Password")

            background: Rectangle {
                color: "transparent"
                implicitHeight: 50
                implicitWidth: 200
                radius: implicitHeight / 2

                Text {
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    color: Theme.mainAppColor
                    font.family: Theme.awesome
                    font.pointSize: 14
                    leftPadding: 10
                    text: "\uf023"
                }
                Rectangle {
                    anchors.bottom: parent.bottom
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: Theme.mainAppColor
                    height: 1
                    width: parent.width - 10
                }
            }

            onEditingFinished: {
                if (!loginPage.login()) {
                    loginPassword.clear();
                    forceActiveFocus();
                }
            }
        }
        Item {
            Layout.preferredHeight: 20
        }
        Row {
            Layout.alignment: Qt.AlignHCenter
            Layout.preferredWidth: loginPage.width - 20
            spacing: 10

            CustomButton {
                id: login_button

                baseColor: Theme.mainAppColor
                borderColor: Theme.mainAppColor
                height: 50
                name: "Log In"

                onClicked: {
                    if (loginPage.login()) {
                        initDialog.close();
                    }
                }
            }
            CustomButton {
                id: cancel_button

                baseColor: Theme.mainAppColor
                borderColor: Theme.mainAppColor
                height: 50
                name: "Cancel"

                onClicked: {
                    cancel_button.enabled = false;
                    login_button.enabled = false;
                    initDialog.exit(-2);
                }
            }
        }
    }
}
