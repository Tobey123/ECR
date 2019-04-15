cmake_minimum_required(VERSION 3.9.1)
project (ecrprog)
set(CMAKE_BUILD_TYPE Debug)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH  ${CMAKE_BINARY_DIR}/lib)

message(${CMAKE_BINARY_DIR})

add_subdirectory(src/cJSON)
add_subdirectory(src/ecr)
add_subdirectory(src/redis)

add_executable(ecrprog ecrprog.c)
target_link_libraries(ecrprog cjson redisclient ecr)