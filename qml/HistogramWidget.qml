import QtQuick 2.15
import QtQml.Models 2.15
import QtQuick.Layouts 1.3

Rectangle {
    id: root
//PUBLIC
    property color background_color: "white"
    property color chart_color: "black"
    property color label_color: "black"
    property color lagend_area_color: "white"
    property color line_color: "black"
    property color ayis_line_color: "#20000000"
    property int width_legend_area: 70

    color: background_color

//PRIVATE
    readonly property int column_width : width / 50
    readonly property int extra_space_top: 20
    readonly property int widget_height : height - extra_space_top
    readonly property int widget_width: width
    readonly property int max_ayis_count: height / 60
    readonly property int all_words_visible_size : 3000
    property int font_size: 15
    property var delta: []
    property var cache_ayis
    property int max_element_column: 1

    Component.onCompleted: {
        let max_count = 20
        for(let i=0;i<max_count;i++) {
            delta.push(1*(10**i))
            delta.push(2*(10**i))
            delta.push(5*(10**i))
        }
    }

    function getSpaceStepAndCount(max, area_height) {
        let ayis_count = 0
        let sum = 0
        let step = 0
        for(let value of delta) {
            ayis_count = 0
            sum = 0
            while(ayis_count <= max_ayis_count && sum <= max) {
                sum += value
                ayis_count++
                step = value
            }
            if(ayis_count <= max_ayis_count)
                break;
        }
        return [Math.floor(area_height*(sum/max)/(ayis_count)-1), step, ayis_count]
    }

    Connections {
        target: chartModel
        function onMaxChanged() { ayisResize() }
    }

    function ayisResize() {
        let prop = getSpaceStepAndCount(chartModel.max, histogramArea.height)
        if(prop == cache_ayis)
           return
        max_element_column = chartModel.max
        ayisColumn.spacing = prop[0]
        ayisColumn.step = prop[1]
        ayisRepeater.model = prop[2]
        cache_ayis = prop
    }

    onHeightChanged: {
        ayisResize()
    }

    Column {
        Item {
            width: widget_width
            height: extra_space_top
        }
        Grid {
            columns: 2
            rows: 2

            rowSpacing: 0
            columnSpacing: 0

            Rectangle {
                z:5
                color: lagend_area_color
                width: width_legend_area
                height: widget_height - width_legend_area
                Column {
                    anchors.bottom: parent.bottom
                    id: ayisColumn
                    spacing: 0
                    z:5
                    property int step: 0
                    Repeater {
                        id: ayisRepeater
                        Rectangle {
                            z:5
                            width: widget_width
                            color: ayis_line_color
                            height:1
                            Text {
                                anchors.bottom: parent.top
                                font.pixelSize: font_size
                                text: (ayisRepeater.count-index-1)*ayisColumn.step
                            }
                        }
                    }
                }
            }

            Rectangle {
                id: histogramArea
                color: background_color
                border {width:1; color: line_color}
                Rectangle {
                        height: 1
                        color: parent.color
                        anchors.fill:parent
                        anchors.leftMargin:1
                        anchors.bottomMargin:1
                }

                property int space_beetween_column: width / (chartModel.size+1) - column_width

                property int free_space_top: 10

                width: widget_width - width_legend_area
                height: widget_height - width_legend_area
                RowLayout {
                    anchors.fill: parent
                    anchors.leftMargin: parent.space_beetween_column / 2
                    spacing: parent.space_beetween_column
                    Repeater {
                        id: histogramRepeater
                        model: chartModel
                        Rectangle {
                            z:6
                            Layout.alignment : Qt.AlignBottom
                            id: columnUnit
                            implicitHeight: parent.height * (value / max_element_column)
                            implicitWidth: column_width
                            color: chart_color.darker(1 + index/10)
                            Text {
                                font.pixelSize: font_size
                                anchors.bottom: parent.top
                                color: label_color
                                anchors.horizontalCenter: parent.horizontalCenter
                                text: value
                            }
                            Text {
                                anchors.topMargin: {
                                    for(let i = 5; i > 0; i--) {
                                        if(root.width < all_words_visible_size/i)
                                            return (index % i) * 13
                                    }
                                    return index
                                }
                                font.pixelSize: font_size
                                anchors.top: parent.bottom
                                color: label_color
                                anchors.horizontalCenter: parent.horizontalCenter
                                text: name
                            }
                        }
                    }
                }
            }

            Item {width: width_legend_area; height:width_legend_area}

            Item {
                width: widget_width - width_legend_area
                height: width_legend_area
            }
        }
    }
}
