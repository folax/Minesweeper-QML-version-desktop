import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3

ButtonStyle {
    background: Rectangle {
        anchors.fill: parent
        anchors.margins: 4
        border.width: 2
        border.color: "#FFED00"
        radius: height / 4
        color: control.pressed ? "#3F6CE8" : "#0044FF"
    }
    label: Component{
           id:labelComponent
               Label{
                   id:text
                   height: 30
                   width:100
                   horizontalAlignment:Text.AlignHCenter
                   verticalAlignment: Text.AlignVCenter

                   color: "yellow"
                   text: control.text
               }
           }
        }
