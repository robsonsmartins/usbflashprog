# ---------------------------------------------------------------------------
# USB EPROM/Flash Programmer
#
# Copyright (2022) Robson Martins
#
# This work is licensed under a Creative Commons Attribution-NonCommercial-
# ShareAlike 4.0 International License.
# ---------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.20)
set(PROJECT_NAME willem)

project(${PROJECT_NAME} VERSION 1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O2 -mwindows")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -static-libgcc -shared -mwindows -Wl,--add-stdcall-alias -s")

set(PROJECT_SOURCES
        ../baseprog.hpp
        ../baseprog.cpp
        willem.hpp
        willem.cpp
)

# Windows application icon
set(WINDOWS_RES_FILE ${CMAKE_BINARY_DIR}/willem.obj)
set(WINDOWS_RC_FILE prog/willem/willem.rc)
set(RC_FLAGS -DMINGW -Ocoff --target=pe-i386)
cmake_path(GET CMAKE_CXX_COMPILER PARENT_PATH CMAKE_CXX_COMPILER_PATH)
set(CMAKE_RC_COMPILER ${CMAKE_CXX_COMPILER_PATH}/windres.exe)
add_custom_command(OUTPUT ${WINDOWS_RES_FILE}
COMMAND ${CMAKE_RC_COMPILER} ${RC_FLAGS} ${WINDOWS_RC_FILE} ${WINDOWS_RES_FILE}
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

add_library(${PROJECT_NAME} SHARED ${PROJECT_SOURCES} ${WINDOWS_RES_FILE})
target_include_directories(${PROJECT_NAME} PUBLIC . ..)
set_target_properties(${PROJECT_NAME} PROPERTIES
                      PREFIX ""
                      OUTPUT_NAME "willem"
                      RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}
                      RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}
)
