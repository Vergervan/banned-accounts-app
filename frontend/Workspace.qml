import QtQuick 2.15
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Window {
    id: window
    width: 300
    height: 200
    title: qsTr("Workspace")
    property AccountListModel accModel: AccountListModel {}
    property string username: ""
    Component.onCompleted: accModel.clear()

    function addAccount(nick, login, time){
        //accModel.append({"name": nick, "login": login, "days": time})
        if(time === "00 00:00"){
            accModel.append({"name": nick, "login": login, "days": time})
        }
        else{
            var numbers = time.split(',');
            var days = numbers[0]*1, hours = numbers[1]*1, minutes = numbers[2]*1
            time = (days < 10 ? ("0"+numbers[0]) : numbers[0]) + " " + (hours < 10 ? ("0"+numbers[1]) : numbers[1]) + ":" + (minutes < 10 ? ("0"+numbers[2]) : numbers[2])
            accModel.append({"name": nick, "login": login, "days": time, "daysNum": days, "hoursNum": hours, "minutesNum": minutes})
        }
    }

    Grid{
        id: grid
        columns: 2

        width: window.width - 10
        height: window.height - 10
        antialiasing: true
        columnSpacing: 5
        leftPadding: 5
        rightPadding: 5
        Column{
            width: grid.width / 3
            Row{
                width: parent.width
                height: plusButton.height
                Rectangle{
                    width: parent.width
                    height: plusButton.height
                    Button{
                        id: plusButton
                        text: "<b>+</b>"
                        background: Rectangle{
                            implicitWidth: 25
                            implicitHeight: 25
                            opacity: enabled ? 1 : 0.3
                            border.color: plusButton.down ? "#17a81a" : "#21be2b"
                            border.width: 1
                            radius: 4
                        }

                        onClicked: {
                            if(listview.count > 0){
                                var item = listview.getCurrentItem()
                                if(item.isEdit)
                                    item.onEdit()
                            }
                            accModel.append({"name": "", "login": "", "days":"000000"})
                            listview.currentIndex = listview.count - 1
                            listview.getCurrentItem().isNew = true
                            listview.getCurrentItem().onEdit()
                            listview.focus = true
                        }
                    }
                    Button{
                        id: minusButton
                        text: "<b>-</b>"
                        x:27
                        background: Rectangle{
                            implicitWidth: 25
                            implicitHeight: 25
                            opacity: enabled ? 1 : 0.3
                            border.color: minusButton.down ? "#a81e17" : "#eb391a"
                            border.width: 1
                            radius: 4
                        }

                        onClicked: {
                            if(listview.count > 0){
                                var str = "{\"nick\":\"%1\",\"login\":\"%2\",\"username\":\"%3\"}"
                                let item = accModel.get(listview.currentIndex)
                                qmlSend(13, str.arg(item.name).arg(item.login).arg(window.username))
                                accModel.remove(listview.currentIndex)
                                listview.focus = true
                            }
                        }
                    }
                }
            }
        }

        Column{
            width: grid.width / 3 * 2
            Rectangle{
                width: parent.width
                height: window.height
                border.color: "black"
                ListView{
                    id: listview
                    width: parent.width
                    height: window.height
                    anchors.centerIn: parent
                    leftMargin: 5; rightMargin: 5; topMargin: 5; bottomMargin: 5;

                    property bool inEdit: false
                    function getCurrentItem() { return listview.itemAtIndex(listview.currentIndex); }
                    function getLastItem() { return listview.itemAtIndex(listview.count - 1); }

                    model: accModel
                    delegate: Item{
                        id: listItem
                        width: listview.width - 25
                        height: 55
                        clip: true
                        property bool isEdit: false
                        property bool isNew: false
                        property string tempDays: ""
                        property int currentEdit: 0
                        //Component.onCompleted: timer.running = true
                        Timer {
                            id:timer
                            interval: 60000; running: window.visible; repeat: true
                            onTriggered: {
                                if(minutesNum === 0){
                                    if(hoursNum === 0){
                                        if(daysNum === 0) return
                                        daysNum--
                                        hoursNum = 23
                                        minutesNum = 59
                                    }else{
                                        hoursNum--
                                        minutesNum = 59
                                    }
                                }else{
                                    minutesNum--
                                }
                                listItem.tempDays = (daysNum < 10 ? ("0"+daysNum) : daysNum.toString()) + " " + (hoursNum < 10 ? ("0"+hoursNum) : hoursNum.toString()) + ":" + (minutesNum < 10 ? ("0"+minutesNum) : minutesNum.toString())
                                if(!listItem.isEdit){
                                    days = listItem.tempDays
                                }
                                console.log("Temp days: " + listItem.tempDays)
                            }
                        }

                        function onEdit(){
                            if(isEdit){
                                if(nickTextEdit.text === "" && loginTextEdit.text === ""){
                                    console.log(listview.currentIndex + ": " + name)
                                    accModel.remove(listview.currentIndex)
                                }else if(daysTextEdit.text === days && !isNew){
                                }else if(daysTextEdit.text.search("^[0-9]{2} [0-9]{2}:[0-9]{2}$") !== 0){
                                    daysTextEdit.text = "00 00:00"
                                    daysTextEdit.focus = true
                                    return
                                }else{
                                    name = nickTextEdit.text
                                    login = loginTextEdit.text
                                    var arr = daysTextEdit.text.split(/[: ]+/)
                                    console.log("Array: " + arr.length + " edit: " + daysTextEdit.text)
                                    daysNum = arr[0]*1
                                    hoursNum = arr[1]*1
                                    minutesNum = arr[2]*1
                                    days = daysTextEdit.text
                                    var str = "{\"nick\":\"%1\",\"login\":\"%2\",\"days\":\"%3\",\"username\":\"%4\"}"
                                    qmlSend(isNew ? 12 : 14, str.arg(name).arg(login).arg(days).arg(window.username))
                                    if(isNew) isNew = false
                                }
                            }
                            isEdit = !isEdit
                            listview.inEdit = isEdit
                            nickTextEdit.focus = true
                        }

                        Keys.onPressed: (event) => {
                            if(event.key === Qt.Key_Return){
                                    onEdit()
                                    listItem.focus = true
                                    event.accepted = true
                            }
                            else if(event.key === Qt.Key_Delete){
                                accModel.remove(listview.currentIndex)
                                var str = "{\"nick\":\"%1\",\"login\":\"%2\",\"username\":\"%3\"}"
                                qmlSend(13, str.arg(nickTextEdit.text).arg(loginTextEdit.text).arg(window.username))
                                event.accepted = true
                            }
                            else if(isEdit){
                                if(event.key === Qt.Key_Up){
                                   currentEdit = Math.max(0, currentEdit - 1)
                                }else if(event.key === Qt.Key_Down){
                                   currentEdit = Math.min(2, currentEdit + 1)
                                }
                                switch(currentEdit){
                                    case 0:
                                        nickTextEdit.focus = true
                                        break;
                                    case 1:
                                        loginTextEdit.focus = true
                                        break;
                                    case 2:
                                        daysTextEdit.focus = true
                                        daysTextEdit.cursorPosition = 0
                                        break;
                                }
                                event.accepted = true
                            }
                        }
                        MouseArea{
                            hoverEnabled: true
                            anchors.fill: parent
                            onClicked: {
                                if(!listview.inEdit)
                                    listview.currentIndex = index
                            }
                        }
                        Grid{
                            width: listItem.width
                            height: listItem.height
                            rows: 3
                            Row{
                                width: parent.width
                                height: 20
                                Text{
                                    id: nickTextLabel
                                    text: "<b>Nick</b>: " + name
                                    height: 18
                                    visible: !listItem.isEdit
                                    leftPadding: 4
                                    rightPadding: 4
                                    topPadding: 2
                                }
                                TextField{
                                    id: nickTextEdit
                                    width: listItem.width
                                    height: parent.height + 2
                                    visible: listItem.isEdit
                                    text: name
                                    placeholderText: "Nickname"
                                    leftPadding: 4
                                    rightPadding: 4
                                    background: null
                                    selectByMouse: true
                                }
                            }
                            Row{
                                width: parent.width
                                height: 20
                                y: 20
                                Text{
                                    id: loginTextLabel
                                    text: "<b>Login</b>: " + login
                                    height: 18
                                    visible: !listItem.isEdit
                                    leftPadding: 4
                                    rightPadding: 4
                                }
                                TextField{
                                    id: loginTextEdit
                                    width: listItem.width
                                    height: parent.height + 2
                                    y: -2.5
                                    visible: listItem.isEdit
                                    text: login
                                    placeholderText: "Login"
                                    leftPadding: 4
                                    rightPadding: 4
                                    background: null
                                    selectByMouse: true
                                }
                            }
                            Row{
                                width: parent.width
                                height: 15
                                Text{
                                    id: daysLabel
                                    text: {
                                        var color = "red", daysText = days.substring(0,2), timeText = days.substring(3)
                                        var finalText = daysText + "d " + timeText
                                        if(daysText === '00'){
                                            color = "#FFD580"
                                            if(timeText === '00:00'){
                                                color = "#90EE90"
                                                finalText = "Unbanned"
                                            }
                                        }
                                        return ("<font color=\"%1\">■ </font>" + finalText).arg(color)
                                    }
                                    font.pointSize: 7.5
                                    y: -3
                                    height: 15
                                    visible: !listItem.isEdit
                                    leftPadding: 4
                                    rightPadding: 4
                                }
                                TextField{
                                    id: daysTextEdit
                                    width: listItem.width
                                    height: parent.height + 2
                                    cursorPosition: 0
                                    inputMask: "00 00:00"
                                    y: -4.5
                                    font.pointSize: daysLabel.font.pointSize
                                    visible: listItem.isEdit
                                    text: days
                                    placeholderText: "Time"
                                    leftPadding: 4
                                    rightPadding: 4
                                    background: null
                                    selectByMouse: true
                                }
                            }
                        }
                    }
                    highlight: Rectangle { color: "orange"; radius: 4; width: listview.width-20 }
                    ScrollBar.vertical: ScrollBar{
                        visible: true
                        clip: false
                        width: 15; height: listview.height;
                    }
                    keyNavigationEnabled: !inEdit
                    focus: true
                    clip: true
                }
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:1.66}
}
##^##*/
