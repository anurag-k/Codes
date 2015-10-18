import QtQuick 1.1
import IUiManager 1.1
import IQApp 1.1
import QtWebKit 1.0
Item
{
   UiManager
    {
        id: uiController

    }

    Connections
    {
        target:uiController
        onWindowStatusChanged:
        {
            startup.z=z1
            contactListPage.z=z2
            browserPage.z=z3

        }
        onContactsFromUi:
        {
            contactsList.model=a
        }

        /*onTimeUpdated:
        {
            date.text=dateString
            time.text=timeString
        }*/

    }



    Timer
    {
        id:timerMin
        interval:60000; running:true; repeat:true
        onTriggered:
        {
            //timeOfContactsPage.text=Qt.formatDateTime(new Date(), "hh:mm")
            timeOfContactsPage.text=uiController.getTime()
            dateOfContactsPage.text=uiController.getDate()
            timeOfBrowserPage.text=uiController.getTime()
            dateOfBrowserPage.text=uiController.getDate()
            //date.text=Qt.formatDateTime(new Date(), "dd/MM/yy")
        }
    }

    height:480
    width:640
    Rectangle
    {
        id:startup
        z:1
        height:480
        width:640
        color:"lightsteelblue"
        Row
        {
            anchors.centerIn: parent
            height:100
            spacing:50
            Button{buttonLabel:"Contacts"}
            Button{buttonLabel:"Internet"}
        }
        Image
        {
            source:"/home/kpit/anuragk/contactList/poweroff.png"
            anchors.right:parent.right
            anchors.rightMargin: 20
            height:45
            width:40
            MouseArea
            {
                id:mouseAreaOfQuit
                anchors.fill: parent
                onClicked:
                {
                    Qt.quit()
                }
            }
        }
    }

    Rectangle
    {
        id:contactListPage
        z:0
        height:480
        width:640
        color:"lightsteelblue"
        Rectangle
        {
            id:contactList
            color:"darkblue"
            height:380
            width:540
            anchors.centerIn: parent
            radius:20
            Image
            {
                id:scrollup
                source:"/home/kpit/anuragk/contactList/navigate.JPG"
                rotation:0
                height: 25
                width:25
                anchors.right: parent.right
                anchors.rightMargin: 10
                anchors.top: parent.top
                anchors.topMargin: 10
                MouseArea
                {
                    id:mouseAreaOfScrollup
                    anchors.fill: parent
                    onClicked:
                    {

                    }
                }
            }
            Image
            {
                id:scrolldown
                source:"/home/kpit/anuragk/contactList/navigate.JPG"
                rotation:180
                height: 25
                width:25
                anchors.right: parent.right
                anchors.rightMargin: 10
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 10
                MouseArea
                {
                    id:mouseAreaOfScrolldown
                    anchors.fill: parent
                    onClicked:
                    {

                    }
                }
            }
            QApp
            {
                id: qAppObj
            }

            Connections
            {
                target:qAppObj
                onContactListObtained:
                {
                    //contactsList.model=contactFieldList
                    console.log("hello")
                }
            }
            ListView
            {
                id:contactsList
                model:testModel
                spacing:20
                anchors.top:parent.top
                anchors.topMargin: 15
                height:100
                width: 480
                delegate:
                    Item
                    {

                       anchors.left: parent.left
                       anchors.leftMargin: 15

                       Rectangle
                       {
                       height:20
                       width:480

                       color:"darkblue"
                       //anchors.horizontalCenter: parent.horizontalCenter
                       TextEdit
                       {

                           anchors.fill: parent
                           Text
                           {
                               text:somecontact
                               color:"white"
                               font.bold: true
                           }
                       }}
                    }
            }
        }
        Text
        {
            text:"Contact List"
            color:"grey"
            font.pointSize: 10
            font.bold: true
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top:parent.top
            anchors.topMargin: 20
        }
        Image
        {
            source:"/home/kpit/anuragk/contactList/home.png"
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.top:parent.top
            anchors.topMargin: 10
            width:40
            height:40
            MouseArea
            {
                id:mouseAreaOfHome_Contacts
                anchors.fill:parent
                onClicked:
                {
                    startup.z=1
                    contactListPage.z=0
                    browserPage.z=0
                }
            }
        }

        Text
        {
            id:dateOfContactsPage
            text: uiController.getDate()
            font.bold: true
            anchors.top: parent.top
            anchors.topMargin: 8
            anchors.right: parent.right
            anchors.rightMargin: 120
            font.pointSize: 10
            color:"black"

        }
        Text
        {
            id:timeOfContactsPage
            text: uiController.getTime()
            font.bold: true
            anchors.top: parent.top
            anchors.topMargin: 8
            anchors.right: parent.right
            anchors.rightMargin: 55
            font.pointSize: 10
            color:"black"

        }

    }

    Rectangle
    {
        id:browserPage
        z:0
        height:480
        width:640
        color:"lightsteelblue"
        Rectangle
        {
            id:qWebView
            color:"darkblue"
            height:360
            width:580
            anchors.top: parent.top
            anchors.topMargin: 100
            anchors.left: parent.left
            anchors.leftMargin: 30
            WebView
            {
                id:browserView
                height:100
                width:100
                anchors.fill: parent
                url:""

            }
        }
        Rectangle
        {
            color:"white"
            width:580
            height:30
            border.width:1
            border.color:"darkblue"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top:parent.top
            anchors.topMargin: 67
            TextEdit
            {
                id:urlField
                height:28
                horizontalAlignment: Text.AlignHCenter
                width:578
                anchors.centerIn: parent
                font.pointSize: 15
                Rectangle
                {
                    id:go
                    height:29
                    width:30
                    anchors.right: parent.right
                    color:"grey"
                    Text
                    {
                        text:"Go"
                        anchors.centerIn: parent
                    }

                    MouseArea
                    {
                        anchors.fill: parent
                        hoverEnabled:true
                        onEntered:
                        {
                            go.color="lightgrey"
                        }
                        onExited: go.color="grey"

                        onClicked: browserView.url=urlField.text
                    }
                }

//                Keys
//                {
//                    onEnterPressed: browserView.url=urlField.text
//                }
            }
        }
        Text
        {
            height:20
            text:"Web Browser"
            color:"black"
            font.bold:true
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top:parent.top
            anchors.topMargin: 20
        }
        Image
        {
            source:"/home/kpit/anuragk/contactList/home.png"
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.top:parent.top
            anchors.topMargin: 10
            width:40
            height:40
            MouseArea
            {
                id:mouseAreaOfHome_Browser
                anchors.fill:parent
                onClicked:
                {
                    startup.z=1
                    contactListPage.z=0
                    browserPage.z=0
                }
            }
        }
        Text
        {
            id:dateOfBrowserPage
            text: uiController.getDate()
            font.bold: true
            anchors.top: parent.top
            anchors.topMargin: 8
            anchors.right: parent.right
            anchors.rightMargin: 95
            font.pointSize: 10
            color:"black"

        }
        Text
        {
            id:timeOfBrowserPage
            //text: Qt.formatDateTime(new Date(), "hh:mm")
            text:uiController.getTime()
            font.bold: true
            anchors.top: parent.top
            anchors.topMargin: 8
            anchors.right: parent.right
            anchors.rightMargin: 30
            font.pointSize: 10
            color:"black"

        }

    }
}

