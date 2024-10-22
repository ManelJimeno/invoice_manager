import QtQuick 2.15
import QtQuick.Controls 2.15


Item {
    property alias password: passwordInput.text
    property alias username: usernameInput.text

    Column {
        spacing: 10

        TextField {
            id: usernameInput

            placeholderText: "Username"
            width: 200
        }
        TextField {
            id: passwordInput

            echoMode: TextInput.Password
            placeholderText: "Password"
            width: 200
        }
        Button {
            text: "Login"

            onClicked: {
                // Handle login logic here
                console.log("Username: " + username);
                console.log("Password: " + password);
            }
        }
    }
}
