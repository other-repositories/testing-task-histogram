import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3
import QtQuick.Dialogs

Rectangle {
    id: root

    property color backgroundColor
    property color mainButtonColor
    property color secondButtonColor
    property color thirdButtonColor
    property color borderColor

    signal someError(var what)

    property bool isPaused: false

    border {width:1; color: borderColor}
    radius: 3

    color: backgroundColor

    function handleError(error) {
        if(error !== "") {
            someError(error)
            return true;
        }
        return false;
    }

    Item {
        anchors.centerIn: parent
        width: parent.width - 20
        height: parent.height - 20
        ColumnLayout {
            anchors.fill:parent
            spacing: 2
            CustomButton {
                text: qsTr("Старт")
                implicitWidth: parent.width
                mainColor: mainButtonColor
                secondColor: secondButtonColor
                onClicked: {
                    handleError(chartModel.start())
                }
            }
            CustomButton {
                text: qsTr("Пауза/Пуск")
                implicitWidth: parent.width
                mainColor: mainButtonColor
                secondColor: secondButtonColor
                onClicked: {
                    if(!handleError(chartModel.pause())) {
                        isPaused = !isPaused
                    }
                }
            }
            CustomButton {
                text: qsTr("Отмена")
                implicitWidth: parent.width
                mainColor: mainButtonColor
                secondColor: secondButtonColor
                onClicked: {
                    if(!handleError(chartModel.cancel())) {
                        isPaused = false
                    }
                }
            }
            CustomButton {
                text: qsTr("Выбрать файл")
                implicitWidth: parent.width
                mainColor: mainButtonColor
                secondColor: thirdButtonColor
                onClicked: {
                    onClicked: fileDialog.open()
                }
            }
            Item {
                Layout.fillHeight: true
            }
        }
    }

    FileDialog {
        visible: false
        id: fileDialog
        title: qsTr("Выберите файл")
        onAccepted: {
            chartModel.setFile(selectedFile.toString().replace('file:///',''))
            mainArea.visible = true
        }
    }
}
