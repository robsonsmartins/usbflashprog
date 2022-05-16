include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(name usbflashprog_test)
set(sources 
    ../hal/gpio.cpp
    ../hal/adc.cpp
    ../hal/pwm.cpp
    ../hal/multicore.cpp
    ../circuits/74hc595.cpp
    ../circuits/dc2dc.cpp
    ../modules/vgenerator.cpp
    hal/gpio_test.cpp 
    hal/adc_test.cpp 
    hal/pwm_test.cpp 
    hal/multicore_test.cpp 
    circuits/74hc595_test.cpp
    circuits/dc2dc_test.cpp
    modules/vgenerator_test.cpp
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
