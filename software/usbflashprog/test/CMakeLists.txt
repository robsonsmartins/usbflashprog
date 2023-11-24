# ---------------------------------------------------------------------------
# USB EPROM/Flash Programmer
#
# Copyright (2022) Robson Martins
#
# This work is licensed under a Creative Commons Attribution-NonCommercial-
# ShareAlike 4.0 International License.
# ---------------------------------------------------------------------------

set(QT_VERSION 5)
set(REQUIRED_LIBS Core Gui Widgets LinguistTools SerialPort)
set(REQUIRED_LIBS_QUALIFIED Qt5::Core Qt5::Gui Qt5::Widgets Qt5::SerialPort)
add_compile_definitions(TEST_BUILD)

find_package(Qt${QT_VERSION} COMPONENTS ${REQUIRED_LIBS} REQUIRED)

include(FetchContent)
FetchContent_Declare(
  googletest
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(PROJ_NAME ufprog_test)

set(PROJECT_SOURCES
    ../config.hpp
    ../backend/opcodes.cpp
    ../backend/opcodes.hpp
    ../backend/runner.cpp
    ../backend/runner.hpp
    mock/qserialport.hpp
    backend/opcodes_test.hpp
    backend/opcodes_test.cpp
    backend/runner_test.hpp
    backend/runner_test.cpp
    main.cpp
)

include(GoogleTest)
find_package(Threads REQUIRED)

add_executable(${PROJ_NAME} WIN32 ${PROJECT_SOURCES})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")

target_include_directories(${PROJ_NAME} PUBLIC . ..)
target_link_libraries(${PROJ_NAME} gtest ${REQUIRED_LIBS_QUALIFIED})

set_target_properties(${PROJ_NAME} PROPERTIES
    WIN32_EXECUTABLE TRUE
)

gtest_discover_tests(${PROJ_NAME})
