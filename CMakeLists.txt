cmake_minimum_required(VERSION 3.0...3.30)

project(renderer_3d
    VERSION 0.1.0
    LANGUAGES CXX C
    )

set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-static HINTS ${CMAKE_CURRENT_LIST_DIR}/../../lib)
file(GLOB SRC_FILES ${CMAKE_CURRENT_LIST_DIR}/*.h ${CMAKE_CURRENT_LIST_DIR}/*.cpp)


add_executable(${PROJECT_NAME} main.cpp ${SRC_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3-static)
target_include_directories(${PROJECT_NAME} PRIVATE include)