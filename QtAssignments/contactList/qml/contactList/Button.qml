import QtQuick 1.1
Item
{
    id:buttonContainer
    property alias buttonLabel:label.text
    width:120
    height:100
    Rectangle
    {
        id:button
        width: 120
        height: 100
        radius:20
        color:"darkblue"
        Text
        {
            id:label
            text:""
            font.pointSize: 10
            color:"white"
            font.bold:true
            anchors.centerIn: parent
        }
        MouseArea
        {
            id:mouseAreaOfButton
            anchors.fill: parent
            hoverEnabled: true
            onHoveredChanged:
            {
                button.color="blue"
            }
            onExited:
            {
                button.color="darkblue"
            }

            onClicked:
            {
                button.color="darkblue"
                if(startup.z==1)
                {
                if(label.text=="Contacts")
                {
                    uiController.route()

                }
                else if(label.text=="Internet")
                {
                    uiController.handleBrowse()

                }

                }
            }
        }
        states:
            State
            {
                when:mouseAreaOfButton.pressed
                PropertyChanges {target:button;y:y+2}
                PropertyChanges {target:button;color:"#9696FF"}
            }
        transitions:
            Transition
            {
                PropertyAnimation{property:"y";duration:20}
                PropertyAnimation{property:"color";duration:200}
            }

    }
}
