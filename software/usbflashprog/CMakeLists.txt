# ---------------------------------------------------------------------------
# USB EPROM/Flash Programmer
#
# Copyright (2022) Robson Martins
#
# This work is licensed under a Creative Commons Attribution-NonCommercial-
# ShareAlike 4.0 International License.
# ---------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.20)

set(CMAKE_OSX_DEPLOYMENT_TARGET "10.13" CACHE STRING "Minimum OS X deployment version")

OPTION(NORMAL_BUILD "Build normal binary" ON)
OPTION(TEST_BUILD "Build the test binary" OFF)

if(TEST_BUILD)
  message("TEST BUILD")
  set(NORMAL_BUILD OFF)
  set(CMAKE_BUILD_TYPE Debug)
  project(ufprog_test LANGUAGES CXX)
  set(CMAKE_CXX_STANDARD 11)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_INCLUDE_CURRENT_DIR ON)
  set(CMAKE_AUTOUIC ON)
  set(CMAKE_AUTOMOC ON)
  set(CMAKE_AUTORCC ON)
  enable_language(CXX)
  enable_testing()
  add_subdirectory(./test)

elseif(NORMAL_BUILD)
  message("NORMAL BUILD")
  set(TEST_BUILD OFF)
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
          backend/opcodes.cpp
          backend/opcodes.hpp
          backend/runner.cpp
          backend/runner.hpp
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

  # MacOS application icon
  set(MACOSX_BUNDLE_ICON_FILE ufprog.icns)
  set(APP_ICON_MACOSX ${CMAKE_CURRENT_SOURCE_DIR}/resources/ufprog.icns)
  set_source_files_properties(${APP_ICON_MACOSX} PROPERTIES
      MACOSX_PACKAGE_LOCATION "Resources")

  add_executable(ufprog WIN32 MACOSX_BUNDLE
      ${PROJECT_SOURCES} ${PROJECT_RESOURCES} ${WINDOWS_RES_FILE} ${APP_ICON_MACOSX})

  target_link_libraries(ufprog ${REQUIRED_LIBS_QUALIFIED})

  set_target_properties(ufprog PROPERTIES
      MACOSX_BUNDLE TRUE
      MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/Info.plist
      WIN32_EXECUTABLE TRUE
  )

endif()