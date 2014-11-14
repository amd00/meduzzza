
cmake_minimum_required(VERSION 2.8)

project(meduzzza)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMakeModules)
#find_package(ClamAV REQUIRED)

execute_process(COMMAND clamav-config --cflags
WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
OUTPUT_VARIABLE CLAMAV_CFLAGS)

message("CFLAGS: " ${CLAMAV_CFLAGS})

execute_process(COMMAND clamav-config --libs
WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
OUTPUT_VARIABLE CLAMAV_LIBS)

message("LFLAGS: " ${CLAMAV_LIBS})

add_subdirectory(sources)