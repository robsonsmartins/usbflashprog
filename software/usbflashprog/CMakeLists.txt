cmake_minimum_required(VERSION 3.20)

project(ufprog VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(QT_VERSION 5)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(REQUIRED_LIBS Core Gui Widgets LinguistTools SerialPort)
set(REQUIRED_LIBS_QUALIFIED Qt5::Core Qt5::Gui Qt5::Widgets Qt5::SerialPort)

find_package(Qt${QT_VERSION} COMPONENTS ${REQUIRED_LIBS} REQUIRED)

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

set(PROJECT_RESOURCES
        ${CMAKE_BINARY_DIR}/i18n.qrc
        resources/resources.qrc
)

qt5_add_translation(QM_FILES ${TS_FILES})
configure_file(resources/i18n.qrc ${CMAKE_BINARY_DIR} COPYONLY)

# Windows application icon
if (WIN32)
  set(WINDOWS_RES_FILE ${CMAKE_BINARY_DIR}/resources.obj)
  set(WINDOWS_RC_FILE resources.rc)
  if ("${CMAKE_SIZEOF_VOID_P}" STREQUAL "4")
    message("-- 32-bit build")
    set(RC_FLAGS -DMINGW -Ocoff --target=pe-i386)
  else()
    message("-- 64-bit build")
    set(RC_FLAGS -DMINGW -Ocoff --target=pe-x86-64)
  endif()
  cmake_path(GET CMAKE_CXX_COMPILER PARENT_PATH CMAKE_CXX_COMPILER_PATH)
  set(CMAKE_RC_COMPILER ${CMAKE_CXX_COMPILER_PATH}/windres.exe)
  add_custom_command(OUTPUT ${WINDOWS_RES_FILE}
  COMMAND ${CMAKE_RC_COMPILER} ${RC_FLAGS} ${WINDOWS_RC_FILE} ${WINDOWS_RES_FILE}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/resources
  )
endif()

add_executable(ufprog WIN32 ${PROJECT_SOURCES} ${PROJECT_RESOURCES} ${WINDOWS_RES_FILE})

target_link_libraries(ufprog ${REQUIRED_LIBS_QUALIFIED})

set_target_properties(ufprog PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER usbflashprog.robsonmartins.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)
