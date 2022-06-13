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

  # Pull in our pico_stdlib which aggregates commonly used features
  target_link_libraries(ufprog 
          pico_stdlib
          hardware_adc
          hardware_pwm
          pico_multicore
          hardware_flash
)

  # enable usb output, disable uart output
  pico_enable_stdio_usb(ufprog 1)
  pico_enable_stdio_uart(ufprog 0)

  # create map/bin/hex/uf2 file etc.
  pico_add_extra_outputs(ufprog)

endif()
