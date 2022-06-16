import QtQuick 2.15
import QtQuick.Controls 2.12

Window {
    width: 300
    height: 200
    title: qsTr("Workspace")

    Label{
        anchors.centerIn: parent
        text: qsTr("You're logged in")
    }
}
