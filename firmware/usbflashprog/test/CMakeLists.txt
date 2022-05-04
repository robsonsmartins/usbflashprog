cmake_minimum_required(VERSION 3.16)

set(name usbflashprog_test)
set(sources 
    ../hal/gpio.cpp
    ../circuits/74hc595.cpp
    hal/gpio_test.cpp 
    circuits/74hc595_test.cpp
    main.cpp
)

find_package(GTest CONFIG REQUIRED)
include(GoogleTest)

add_executable(${name} ${sources})

target_include_directories(${name} PUBLIC . .. mock)
target_link_libraries(${name} gtest)
gtest_discover_tests(${name})
