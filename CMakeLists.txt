cmake_minimum_required(VERSION 3.9.1)
project (ECR)
set(CMAKE_BUILD_TYPE Debug)

if(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zi")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3")
endif()

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH  ${CMAKE_BINARY_DIR}/lib)
include_directories(${ZMQ_INCLUDE_DIRS})
include_directories(${CZMQ_INCLUDE_DIRS})

message(${CMAKE_BINARY_DIR})

add_subdirectory(src/ecr)

add_executable(ecrprog ecrprog.c)
target_link_libraries(ecrprog ecr)