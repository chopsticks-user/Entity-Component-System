


cmake_minimum_required(VERSION 3.15)
project(Ushi_Examples LANGUAGES CXX)

if(${CMAKE_CXX_STANDARD} LESS 20)
    return()
endif()

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

file(GLOB ExamplePaths ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
foreach (ExamplePath ${ExamplePaths})
    get_filename_component(ExampleFileName ${ExamplePath} NAME_WLE)
    add_executable(${ExampleFileName} ${ExamplePath})
    target_link_libraries(${ExampleFileName}
        PRIVATE Ushi
        PRIVATE sfml-graphics
        PRIVATE sfml-window
        PRIVATE sfml-system
    )
endforeach (ExamplePath ${ExamplePaths})