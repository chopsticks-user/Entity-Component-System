

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
    get_filename_component(exampleName ${ExamplePath} NAME_WLE)
    add_executable(${exampleName} ${ExamplePath})
    target_include_directories(${exampleName}
        PUBLIC ${PROJECT_SOURCE_DIR}/Include
    )
    target_link_libraries(${exampleName}
        PRIVATE CompilerFlags
        PRIVATE sfml-graphics
        PRIVATE sfml-window
        PRIVATE sfml-system
    )
endforeach (ExamplePath ${ExamplePaths})