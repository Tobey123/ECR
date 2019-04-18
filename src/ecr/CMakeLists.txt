cmake_minimum_required(VERSION 3.9.1)
project(libecr)
set(LIBRARY_OUTPUT_PATH  ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

include_directories(base)
include_directories(cjson)
include_directories(compat)
include_directories(job)
include_directories(status)
include_directories(redis)
file(GLOB SOURCES "*.c")

find_package(hiredis REQUIRED)
include_directories( ${HIREDIS_INCLUDE_DIRS} )

add_library(ecr STATIC ${SOURCES})
target_link_libraries(ecr ${HIREDIS_LIBRARIES})
