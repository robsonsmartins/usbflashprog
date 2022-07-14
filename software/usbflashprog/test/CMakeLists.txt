# ---------------------------------------------------------------------------
# USB EPROM/Flash Programmer
#
# Copyright (2022) Robson Martins
#
# This work is licensed under a Creative Commons Attribution-NonCommercial-
# ShareAlike 4.0 International License.
# ---------------------------------------------------------------------------

include_directories(.. .)

set(QT_VERSION 5)
set(REQUIRED_LIBS Core Gui Widgets LinguistTools SerialPort Test)
set(REQUIRED_LIBS_QUALIFIED Qt5::Core Qt5::Gui Qt5::Widgets Qt5::SerialPort Qt5::Test)

find_package(Qt${QT_VERSION} COMPONENTS ${REQUIRED_LIBS} REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
enable_testing(true)

set(PROJ_NAME ufprog_test)

set(PROJECT_SOURCES
    ../config.hpp
    ../backend/opcodes.cpp
    ../backend/opcodes.hpp
    ../backend/runner.cpp
    ../backend/runner.hpp
#      ../main/mainwindow.cpp
#      ../main/mainwindow.hpp
#      ../main/mainwindow.ui
#      ../diag/diagwindow.cpp
#      ../diag/diagwindow.hpp
#      ../diag/diagwindow.ui
    backend/opcodes_test.hpp
    backend/opcodes_test.cpp
    main.cpp
)

set(PROJECT_RESOURCES
        ../resources/resources.qrc
)

add_executable(${PROJ_NAME} WIN32 ${PROJECT_SOURCES} ${PROJECT_RESOURCES})
add_test(NAME OpCodeTest COMMAND ${PROJ_NAME} OpCodeTest)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")

target_include_directories(${PROJ_NAME} PUBLIC . ..)
target_link_libraries(${PROJ_NAME} PRIVATE ${REQUIRED_LIBS_QUALIFIED})

set_target_properties(${PROJ_NAME} PROPERTIES
    WIN32_EXECUTABLE TRUE
)

