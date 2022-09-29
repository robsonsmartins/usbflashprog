# ---------------------------------------------------------------------------
# USB EPROM/Flash Programmer
#
# Copyright (2022) Robson Martins
#
# This work is licensed under a Creative Commons Attribution-NonCommercial-
# ShareAlike 4.0 International License.
# ---------------------------------------------------------------------------

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(name ufprog_test)
set(sources 
    ../hal/gpio.cpp
    ../hal/adc.cpp
    ../hal/pwm.cpp
    ../hal/multicore.cpp
    ../hal/flash.cpp
    ../hal/string.cpp
    ../hal/serial.cpp
    ../circuits/74hc595.cpp
    ../circuits/74hc165.cpp
    ../circuits/dc2dc.cpp
    ../modules/vgenerator.cpp
    ../modules/opcodes.cpp
    hal/gpio_test.cpp 
    hal/adc_test.cpp 
    hal/pwm_test.cpp 
    hal/multicore_test.cpp 
    hal/flash_test.cpp 
    hal/string_test.cpp 
    hal/serial_test.cpp 
    circuits/74hc595_test.cpp
    circuits/74hc165_test.cpp
    circuits/dc2dc_test.cpp
    modules/vgenerator_test.cpp
    modules/opcodes_test.cpp
    main.cpp
)

include(GoogleTest)

find_package(Threads REQUIRED)

add_executable(${name} ${sources})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")

target_include_directories(${name} PUBLIC . .. mock)
target_link_libraries(${name} ${CMAKE_THREAD_LIBS_INIT} gtest)
gtest_discover_tests(${name})
