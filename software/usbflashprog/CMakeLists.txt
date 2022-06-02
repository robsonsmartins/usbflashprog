cmake_minimum_required(VERSION 3.5)

project(ufprog VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Widgets LinguistTools SerialPort REQUIRED)

set(TS_FILES
    i18n/ufprog_en_US.ts
    i18n/ufprog_pt_BR.ts
)

set(PROJECT_SOURCES
        config.hpp
        backend/serial.cpp
        backend/serial.hpp
        main/mainwindow.cpp
        main/mainwindow.hpp
        main/mainwindow.ui
        diag/diagwindow.cpp
        diag/diagwindow.hpp
        diag/diagwindow.ui
        main.cpp
)

add_executable(ufprog ${PROJECT_SOURCES})

qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})

target_link_libraries(ufprog PRIVATE Qt5::Widgets Qt5::SerialPort)

set_target_properties(ufprog PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER usbflashprog.robsonmartins.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)
