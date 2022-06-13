# ---------------------------------------------------------------------------
# USB EPROM/Flash Programmer
#
# Copyright (2022) Robson Martins
#
# This work is licensed under a Creative Commons Attribution-NonCommercial-
# ShareAlike 4.0 International License.
# ---------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.16)

OPTION(NORMAL_BUILD "Build normal binary" ON)
OPTION(TEST_BUILD "Build the test binary" OFF)

if(TEST_BUILD)
  message("TEST BUILD")
  set(NORMAL_BUILD OFF)
  set(CMAKE_BUILD_TYPE Debug)
  project(ufprog_test C CXX)
  set(CMAKE_C_STANDARD 11)
  set(CMAKE_CXX_STANDARD 17)
  enable_language(C CXX)
  enable_testing()
  add_subdirectory(./test)

elseif(NORMAL_BUILD)
  message("NORMAL BUILD")
  set(TEST_BUILD OFF)
  include(pico_sdk_import.cmake)
  project(ufprog C CXX ASM)
  set(CMAKE_C_STANDARD 11)
  set(CMAKE_CXX_STANDARD 17)
  pico_sdk_init()

  add_executable(ufprog 
        hal/gpio.cpp
        hal/adc.cpp
        hal/pwm.cpp
        hal/multicore.cpp
        hal/flash.cpp
        hal/serial.cpp
        hal/string.cpp
        circuits/74hc595.cpp
        circuits/dc2dc.cpp
        modules/vgenerator.cpp
        modules/opcodes.cpp
        modules/runner.cpp
        main.cpp
  )

  target_include_directories(ufprog PUBLIC .)

  target_compile_options(ufprog PRIVATE -DPICO_STDIO_USB_ENABLE_RESET_VIA_VENDOR_INTERFACE=0)
  target_compile_options(ufprog PRIVATE -DPICO_STDIO_USB_CONNECT_WAIT_TIMEOUT_MS=0)

  target_link_libraries(ufprog 
          pico_stdlib
          hardware_adc
          hardware_pwm
          pico_multicore
          hardware_flash
)

  pico_enable_stdio_usb(ufprog 1)
  pico_enable_stdio_uart(ufprog 0)
  pico_add_extra_outputs(ufprog)

endif()
