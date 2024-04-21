


cmake_minimum_required(VERSION 3.15)
project(Ushi VERSION 1.0.0 LANGUAGES CXX)

include(CMake/Options.cmake)
include(CMake/CompilerFlags.cmake)

enable_testing()

add_library(${Ushi_TARGET_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${Ushi_TARGET_NAME} 
    ALIAS ${Ushi_TARGET_NAME}
)
target_include_directories(${Ushi_TARGET_NAME} 
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/Include
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/Include/Ushi # PRVIATE
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/Include/Ushi/Impl # PRVIATE
)
target_link_libraries(${Ushi_TARGET_NAME} 
    INTERFACE ${PROJECT_NAME}::CompilerFlags
)

# if(${Ushi_BUILD_EXAMPLES})
#     add_subdirectory(Examples)
# endif()

if(${Ushi_BUILD_TESTS})
    add_subdirectory(Test)
endif()