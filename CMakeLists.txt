cmake_minimum_required(VERSION 3.2)

project(time_measuremt)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_library(time_measurement ${CMAKE_CURRENT_SOURCE_DIR}/src/time_measurement.cpp)

# If not used as a sub directory, build example
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    add_executable(example example.cpp)
    target_link_libraries(example time_measurement)
endif()