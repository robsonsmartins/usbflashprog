# ---------------------------------------------------------------------------
# USB EPROM/Flash Programmer
#
# Copyright (2022) Robson Martins
#
# This work is licensed under a Creative Commons Attribution-NonCommercial-
# ShareAlike 4.0 International License.
# ---------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.20)

project(emuprog_proj LANGUAGES C CXX)

set(CMAKE_SYSTEM_NAME "Windows")
set(CMAKE_SYSTEM_PROCESSOR "i686")
set(CMAKE_SYSTEM_VERSION "7")
set(CMAKE_SHARED_LIBRARY_SUFFIX ".dll")

# Forces Compiler (32 bits)
if(WIN32)
    set(CXX_32BIT_COMPILER "g++.exe")
    set(RC_32BIT_COMPILER "windres.exe")
else()
    set(CXX_32BIT_COMPILER "i686-w64-mingw32-g++")
    set(RC_32BIT_COMPILER "i686-w64-mingw32-windres")
endif()

set(CXX_32BIT_FLAGS "-Wall -O2 -mwindows")
set(SHARED_LINKER_32BIT_FLAGS "-static-libgcc -shared -mwindows -Wl,--add-stdcall-alias -Wl,--no-undefined -s")

message(STATUS "CMAKE_C_COMPILER (32 bits): ${C_32BIT_COMPILER}")

add_subdirectory(./iowrapper)
add_subdirectory(./prog)
add_subdirectory(./chip)
add_subdirectory(./emuprog)
