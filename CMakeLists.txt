

cmake_minimum_required(VERSION 3.15)
project(ECS VERSION 1.0.0 LANGUAGES CXX)

include(CompilerFlags.cmake)

add_library(${PROJECT_NAME} INTERFACE)
include_directories(${PROJECT_SOURCE_DIR}/Source)

add_subdirectory(${PROJECT_SOURCE_DIR}/Examples)


