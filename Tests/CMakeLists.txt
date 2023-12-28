


cmake_minimum_required(VERSION 3.15)
project(ECSTests LANGUAGES CXX)

find_package(ECS CONFIG REQUIRED PATHS ${CMAKE_BINARY_DIR})

file(GLOB TestPaths ${CMAKE_CURRENT_SOURCE_DIR}/*.test.cpp)
foreach (TestPath ${TestPaths})
    get_filename_component(TestFileName ${TestPath} NAME_WLE)
    add_executable(${TestFileName} ${TestPath})
    target_include_directories(${TestFileName}
        PRIVATE ${ECS_INCLUDE_DIR}
    )
    target_link_libraries(${TestFileName}
        PRIVATE ECSCompilerFlags
        PRIVATE ECS
    )
endforeach (TestPath ${TestPaths})