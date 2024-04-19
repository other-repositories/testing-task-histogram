import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Popup {
    property color background_color
    property color border_color
    property color text_color

    property alias title : label.text
    modal: true
    focus: true
    id: commonDialog
    x: (parent.width - width)/2
    y: (parent.height - height)/2
    padding: 0
    width: 250
    height:130
    background: Rectangle {
        color: 'white'
        border {
            width: 2
            color: border_color
        }
    }
    contentItem: ColumnLayout {
        Item { Layout.fillHeight: true;}
        Label {
            font.pixelSize: 16
            Layout.alignment: Qt.AlignHCenter
            //font.bold: true
            color:"black"
            width:200
            height:50
            text: qsTr('Предупреждение')
        }
        Item { Layout.fillHeight: true;}
        Label {
            font.pixelSize: 13
            Layout.leftMargin: 20
            Layout.alignment: Qt.AlignLeft
            id: label
            color:"black"
            width:200
            height:50
        }
        Item { Layout.fillHeight: true;}
        CustomButton {
            Layout.alignment: Qt.AlignRight
            Layout.rightMargin: 20
            text: qsTr('Закрыть')
            mainColor: background_color
            secondColor: border_color
            onClicked: {
                commonDialog.close()
            }
        }
        Item { Layout.fillHeight: true;}
    }
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
}
