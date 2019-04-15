cmake_minimum_required(VERSION 3.9.1)
project(libredisclient)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set(LIBRARY_OUTPUT_PATH  ${CMAKE_BINARY_DIR}/lib)

include_directories(src/ecr/base)
include_directories(src/ecr/job)
include_directories(src/ecr/status)
include_directories(src/redis)
file(GLOB SOURCES "*.c")

find_package(hiredis REQUIRED)
include_directories( ${HIREDIS_INCLUDE_DIRS} )

add_library(redisclient STATIC ${SOURCES})
target_link_libraries(redisclient ${HIREDIS_LIBRARIES})


