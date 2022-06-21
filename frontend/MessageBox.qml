import QtQuick 2.0
import QtQuick.Controls 2.12

Window {
    id: root
    flags: Qt.Dialog
    width: mainGrid.width+10
    height: mainGrid.height+10

    property Component btnComp: Qt.createComponent("MessageBoxButton.qml")

    function messageBoxButtonClicked(btn){
        console.log("Button clicked " + btn)
        root.close()
    }

    function addMessageBoxButton(type, text){
        buttonsCount++;
        let btn = btnComp.createObject(buttonGrid);
        btn.buttonType = type
        btn.buttonText = text
        btn.msgBoxClick.connect(messageBoxButtonClicked)
    }

    enum MessageBoxButton{
        Ok = 1,
        Cancel = 2
    }

    property int buttons: MessageBox.MessageBoxButton.Ok
    property int buttonsCount: 0

    Component.onCompleted: {
        messageText.text = "Error"
        let btnComp = Qt.createComponent("MessageBoxButton.qml")
        if((buttons & MessageBox.MessageBoxButton.Ok) === MessageBox.MessageBoxButton.Ok)
        {
            addMessageBoxButton(MessageBox.MessageBoxButton.Ok, "Ok")
        }
        if((buttons & MessageBox.MessageBoxButton.Cancel) === MessageBox.MessageBoxButton.Cancel)
        {
            addMessageBoxButton(MessageBox.MessageBoxButton.Cancel, "Cancel")
        }


        this.show()
    }

    Grid{
        id: mainGrid
        rows: 2
        rowSpacing: 10
        anchors.centerIn: parent

        Row{
            Text {
                id: messageText
                horizontalAlignment: Text.AlignHCenter
            }
        }
        Row{
            Grid{
                id: buttonGrid
                columns: buttonsCount
                columnSpacing: 10
            }
        }
    }
}
