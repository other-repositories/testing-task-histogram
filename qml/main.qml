import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3
import QtQuick.Controls.Universal
import QtQuick.Dialogs

Window {
    id: root
    width: 600
    height: 480
    minimumHeight: 360
    minimumWidth: 600
    visible: true
    title: qsTr("Histogram")

    property color background_color: "#F0F8FF"
    property color background_color_bar: "#E7EFF7"
    property color line_color: "#1D1F20"
    property color progress_color: "#00A3FF"
    property color main_button_color: "#F0F8FF"
    property color second_button_color: "#0096EF"
    property color third_button_color: "#023e8a"

    property color chart_background_color: "#F0F8FF"
    property color chart_chart_color: "#00A3FF"
    property color chart_label_color: "#1D1F20"
    property color chart_lagend_area_color: "#F0F8FF"
    property color chart_line_color: "#1D1F20"
    property color chart_ayis_line_color: "#20000000"

    color: background_color

    Rectangle {
        z: 20
        anchors.fill: parent
        color: background_color
        id: initialScreen

        Item {
            id: openIcon
            anchors.centerIn: parent
            height: 150
            width: 150
            Image {
               anchors.fill: parent
               source: "../images/open_icon.png"
            }
        }
        Text {
            anchors.top: openIcon.bottom
            anchors.horizontalCenter: openIcon.horizontalCenter
            font.bold: true
            font.pixelSize: 25
            opacity: 0.3
            text: qsTr('Открыть файл')
        }
        Rectangle {
            color:"transparent"
            radius: 25
            border{width:1; color: chart_label_color}
            width: parent.width* 0.8
            height: parent.height* 0.8
            anchors.centerIn: parent
        }
        MouseArea {
            cursorShape: Qt.PointingHandCursor
            anchors.fill: parent
            onClicked: {
                fileDialog.open()
            }
        }
    }

    ColumnLayout {
        anchors.fill: parent

        Item {
            id: mainArea
            visible: false
            Layout.fillWidth: true
            Layout.fillHeight: true
            RowLayout {
                spacing: 30
                anchors.fill:parent
                Item {
                    width: 15
                    height:1
                }
                HistogramWidget {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    background_color: chart_background_color
                    chart_color: chart_chart_color
                    label_color: chart_label_color
                    lagend_area_color: chart_lagend_area_color
                    line_color: chart_line_color
                    ayis_line_color: chart_ayis_line_color
                }
                RightBar {
                    id: mainBar
                    Layout.fillHeight: true
                    backgroundColor: background_color_bar
                    mainButtonColor: main_button_color
                    secondButtonColor: second_button_color
                    thirdButtonColor: third_button_color
                    borderColor: second_button_color
                    width: 150
                    onSomeError: {
                        warningBox.title = what
                        warningBox.open()
                    }
                }
                Item {
                    width: 25
                    height:1
                }
            }
        }

        ProgressBar {
            id: progressBar
            Layout.leftMargin: 10
            Layout.rightMargin: 10
            Layout.topMargin: 5
            Layout.bottomMargin: 5
            Layout.fillWidth: true
            height: 50
            value: chartModel.proc
            Label {
                z:20
                visible: parent.value
                color: parent.value <  0.02 ? "#00A3FF" : "white"
                font.bold: true
                text: Math.floor(parent.value*100)+'%'
                anchors.top: parent.top
                anchors.topMargin: -4
                anchors.left: parent.left
                anchors.leftMargin: ( parent.value + ( parent.value < 0.02 ? 0.05 : 0) )* parent.width / 2 - 5
            }
        }
    }

    FileDialog {
        visible: false
        id: fileDialog
        title: "Выберите файл"
        onAccepted: {
            chartModel.setFile(selectedFile.toString().replace('file:///',''))
            mainArea.visible = true
            initialScreen.visible = false
        }
    }

    CustomWarningPopup {
        id: warningBox
        background_color: root.background_color
        border_color: second_button_color
        text_color: "black"
    }

    Rectangle {
        anchors.fill: parent
        id: pauseSplash
        color : "#09000000"
        visible: mainBar.isPaused
        Image {
            id: pauseImage
            anchors.centerIn: parent
            scale:0.2
            source: "../images/pause_icon.png"

            SequentialAnimation {
                id: opacityAnimation
                loops: Animation.Infinite
                running: true

                NumberAnimation {
                    target: pauseImage
                    property: "opacity"
                    to: 0.5
                    duration: 1000
                    easing.type: Easing.InOutQuad
                }

                NumberAnimation {
                    target: pauseImage
                    property: "opacity"
                    to: 1.0
                    duration: 1000
                    easing.type: Easing.InOutQuad
                }
            }
        }
    }
}
