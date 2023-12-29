


cmake_minimum_required(VERSION 3.15)
project(Tora_Examples LANGUAGES CXX)

if(${CMAKE_CXX_STANDARD} LESS 20)
    return()
endif()

find_library(SFMLGraphicsLib NAMES sfml-graphics)
find_library(SFMLWindowLib NAMES sfml-window)
find_library(SFMLSystemLib NAMES sfml-system)

if(NOT SFMLGraphicsLib OR NOT SFMLWindowLib OR NOT SFMLSystemLib)
    message(WARNING "Since SFML is not fully installed in your system, \
        examples will not be built.")
    return()
endif()

file(GLOB ExamplePaths ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
foreach (ExamplePath ${ExamplePaths})
    get_filename_component(ExampleFileName ${ExamplePath} NAME_WLE)
    add_executable(${ExampleFileName} ${ExamplePath})
    target_link_libraries(${ExampleFileName}
        PRIVATE Tora
        PRIVATE sfml-graphics
        PRIVATE sfml-window
        PRIVATE sfml-system
    )
endforeach (ExamplePath ${ExamplePaths})