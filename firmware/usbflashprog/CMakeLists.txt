cmake_minimum_required(VERSION 3.16)
include(pico_sdk_import.cmake)
project(usbflashprog C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(usbflashprog 
        hal/gpio.cpp
        circuits/74hc595.cpp
#        base/pwm.cpp
#        base/adc.cpp
#        base/multicore.cpp
#        base/flash.cpp
#        modules/dc2dc.cpp
#        modules/vgenerator.cpp
        main.cpp
)

target_include_directories(usbflashprog PUBLIC .)

# Pull in our pico_stdlib which aggregates commonly used features
target_link_libraries(usbflashprog 
        pico_stdlib hardware_adc hardware_pwm pico_multicore hardware_flash)

# enable usb output, disable uart output
pico_enable_stdio_usb(usbflashprog 1)
pico_enable_stdio_uart(usbflashprog 0)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(usbflashprog)
