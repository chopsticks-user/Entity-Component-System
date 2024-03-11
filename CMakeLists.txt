


cmake_minimum_required(VERSION 3.15)
project(Tora VERSION 1.0.0 LANGUAGES CXX)

include(CMake/Options.cmake)
include(CMake/CompilerFlags.cmake)

enable_testing()

add_library(${Tora_TARGET_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${Tora_TARGET_NAME} 
    ALIAS ${Tora_TARGET_NAME}
)
target_include_directories(${Tora_TARGET_NAME} 
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/Include
)
target_link_libraries(${Tora_TARGET_NAME} 
    INTERFACE ${PROJECT_NAME}::CompilerFlags
)

if(${Tora_BUILD_EXAMPLES})
    add_subdirectory(Examples)
endif()

if(${Tora_BUILD_TESTS})
    add_subdirectory(Test)
endif()