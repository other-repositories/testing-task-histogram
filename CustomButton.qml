import QtQuick 2.12
import QtQuick.Controls 2.15

Button {
    property color mainColor: "#000"
    property color secondColor: "#000"
    flat:true
    hoverEnabled: true
    id: button
    contentItem: Text {
        horizontalAlignment: Text.AlignHCenter
        text: button.text
        color: button.hovered ? mainColor : button.pressed ? mainColor : secondColor.darker(2)
    }
    background: Rectangle {
        color: button.hovered ? secondColor : button.pressed ? secondColor : mainColor
        border.color:  button.hovered ? secondColor : button.pressed ? mainColor :  secondColor
        border.width: 2
        radius: 5
    }
}
