import QtQuick 2.15
import QtQuick.Controls 2.12
import QtQuick.Dialogs
import Qt.labs.platform

Window {
    id: root
    signal qmlAuth(string nick, string pass, bool reg)
    signal qmlSend(int code, string mes)
    signal qmlSaveCurrentUser()
    property Workspace window: Workspace{}

    SystemTrayIcon {
        visible: true
        icon.source: "qrc:/appicon.ico"

        onActivated: {
            if(window.visible){
                window.hide()
            }else{
                window.show()
                window.raise()
                window.requestActivate()
            }
        }
    }

    Component.onCompleted: {
        window.hide()
    }
    function showMessageBox(title, text){
        let msgBoxComp = Qt.createComponent("MessageBox.qml");
        if(msgBoxComp.status === Component.Ready){
            let msgBox = msgBoxComp.createObject(root)
            msgBox.setMessageText(text)
            msgBox.setTitle(title)
        }
    }

    function addAccount(nick, login, time){
        window.addAccount(nick,login,time)
    }

    function userExit()
    {
        if(window.visible){
            window.hide()
            show()
            window.accModel.clear()
        }
    }

    function getAuthResult(res, err)
    {
        console.log("Auth result: " + res + " " + err)
        if(res === true){
            window.username = err
            window.show()
            root.hide()
            if(rememberCheck)
                qmlSaveCurrentUser()
        }else if(res === false){
            let msgBoxComp = Qt.createComponent("MessageBox.qml");
            if(msgBoxComp.status === Component.Ready){
                console.log(err)
                let msgBox = msgBoxComp.createObject(root)
                msgBox.setMessageText(err)
            }
        }
    }

    width: 340
    height: 320
    visible: true
    title: qsTr("Authorization in the workspace")

    Grid{
        id: loginGrid
        anchors.centerIn: parent
        rows: 5
        rowSpacing: 8
        Row{
            Rectangle{
                id: nameEditBorder
                width: 100
                height: 20
                border.width: 0.5
                border.color: "#000000"
                clip: true

                TextInput{
                    id: nameEdit
                    width: parent.width - 6
                    height: parent.height
                    verticalAlignment: TextEdit.AlignVCenter
                    horizontalAlignment: TextEdit.AlignLeft
                    x: 3
                    selectByMouse: true
                }
            }
        }
        Row{
            Rectangle{
                id: passwordEditBorder
                width: 100
                height: 20
                border.width: 0.5
                border.color: "#000000"
                clip: true

                TextInput{
                    id: passwordEdit
                    width: parent.width - 6
                    height: parent.height
                    verticalAlignment: TextEdit.AlignVCenter
                    horizontalAlignment: TextEdit.AlignLeft
                    x: 3
                    echoMode: TextInput.Password
                    selectByMouse: true
                }
            }
        }
        Row{

            Button{
                id: signinButton
                width: 100
                text: "Sign In"
                onClicked: {
                    qmlAuth(nameEdit.text, passwordEdit.text, false)
                }
            }
        }
        Row{

            Button{
                id: signupButton
                width: 100
                text: "Sign Up"
                onClicked: {
                    qmlAuth(nameEdit.text, passwordEdit.text, true)
                }
            }
        }
        Row{
              CheckBox{
                  id: rememberCheck
                  text: "Remember me"
              }

//            Button{
//                id: debugButton
//                width: 100
//                text: "Debug"
//                onClicked: getAuthResult(true, "")
//            }
        }
    }
}
