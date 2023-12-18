cmake_minimum_required(VERSION 3.5)

project(ECS VERSION 1.0.0 LANGUAGES CXX)

include(CompilerFlags.cmake)

# add_subdirectory(${PROJECT_SOURCE_DIR}/Source)
add_subdirectory(${PROJECT_SOURCE_DIR}/Tests)
# add_subdirectory(${PROJECT_SOURCE_DIR}/Examples)

add_subdirectory(${PROJECT_SOURCE_DIR}/Libs/Catch2)
