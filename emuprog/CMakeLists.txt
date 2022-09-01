# ---------------------------------------------------------------------------
# USB EPROM/Flash Programmer
#
# Copyright (2022) Robson Martins
#
# This work is licensed under a Creative Commons Attribution-NonCommercial-
# ShareAlike 4.0 International License.
# ---------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.20)

set(PROJECT_NAME emuprog)
project(${PROJECT_NAME} VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(QT_VERSION 5)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(REQUIRED_LIBS Core Gui Widgets)
set(REQUIRED_LIBS_QUALIFIED Qt5::Core Qt5::Gui Qt5::Widgets)

find_package(Qt${QT_VERSION} COMPONENTS ${REQUIRED_LIBS} REQUIRED)

set(PROJECT_SOURCES
        main/mainwindow.cpp
        main/mainwindow.hpp
        main/mainwindow.ui
        main.cpp
)

set(PROJECT_RESOURCES
  ../resources/resources.qrc
)

# Forces Compiler and Qt5 (64 bits)
set(CMAKE_CXX_COMPILER "D:\\MinGW\\Qt\\5.12.12\\Tools\\mingw730_64\\bin\\g++.exe")
set(Qt5_DIR "D:\\MinGW\\Qt\\5.12.12\\5.12.12\\mingw73_64\\lib\\cmake\\Qt5")

# Windows application icon
if (WIN32)
  set(WINDOWS_RES_FILE ${CMAKE_BINARY_DIR}/resources.obj)
  set(WINDOWS_RC_FILE ../resources/resources.rc)
  set(RC_FLAGS -DMINGW -Ocoff --target=pe-x86-64)
  cmake_path(GET CMAKE_CXX_COMPILER PARENT_PATH CMAKE_CXX_COMPILER_PATH)
  set(CMAKE_RC_COMPILER ${CMAKE_CXX_COMPILER_PATH}/windres.exe)
  add_custom_command(OUTPUT ${WINDOWS_RES_FILE}
  COMMAND ${CMAKE_RC_COMPILER} ${RC_FLAGS} ${WINDOWS_RC_FILE} ${WINDOWS_RES_FILE}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/resources
  )
endif()

add_executable(${PROJECT_NAME} WIN32
    ${PROJECT_SOURCES} ${PROJECT_RESOURCES} ${WINDOWS_RES_FILE})

target_include_directories(${PROJECT_NAME} PUBLIC . ..)

target_link_libraries(${PROJECT_NAME} ${REQUIRED_LIBS_QUALIFIED})

set_target_properties(${PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE TRUE
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}
)

