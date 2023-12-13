cmake_minimum_required(VERSION 3.5)

project(libGIS)

file(GLOB_RECURSE BASE_HEADERS "*.h")
file(GLOB_RECURSE BASE_SOURCES "*.c")

add_library(${PROJECT_NAME} INTERFACE)
target_sources(${PROJECT_NAME} INTERFACE ${BASE_HEADERS} ${BASE_SOURCES})
target_include_directories(${PROJECT_NAME} INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")
