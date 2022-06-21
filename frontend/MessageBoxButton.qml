import QtQuick 2.0
import QtQuick.Controls 2.12

Column {
    property string buttonText
    property int buttonType
    property Button button: buttonReal
    signal msgBoxClick(int btn)
    Button{
        id: buttonReal
        text: buttonText
        onClicked: {
            msgBoxClick(buttonType)
        }
    }
}
