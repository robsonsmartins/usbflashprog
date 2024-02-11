# ---------------------------------------------------------------------------
# USB EPROM/Flash Programmer
#
# Copyright (2022) Robson Martins
#
# This work is licensed under a Creative Commons Attribution-NonCommercial-
# ShareAlike 4.0 International License.
# ---------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.20)

project(simulator LANGUAGES C CXX)
add_subdirectory(./sram)
add_subdirectory(./eprom)
add_subdirectory(./eeprom28c)
add_subdirectory(./flash28f)
add_subdirectory(./flash28sst)
add_subdirectory(./flash28i)
add_subdirectory(./flash28i16)
