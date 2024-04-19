cmake_minimum_required(VERSION 3.16)

project(histogram VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.2 COMPONENTS Quick REQUIRED)

qt_add_executable(apphistogram
    source/main.cpp
    source/histogram.cpp
    source/model.cpp
    source/use_case.cpp
    source/use_case_histogram_impl.cpp
    source/use_case_reader_impl.cpp
    source/data_reader.cpp
)

qt_add_qml_module(apphistogram
    URI histogram
    VERSION 1.0
    QML_FILES
    qml/main.qml
    qml/HistogramWidget.qml
    qml/RightBar.qml
    qml/CustomWarningPopup.qml
    qml/CustomButton.qml
    images/open_icon.png
    images/pause_icon.png
    images/histogram_logo.png
)

set_target_properties(apphistogram PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(apphistogram
    PRIVATE Qt6::Quick)

install(TARGETS apphistogram
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
