


cmake_minimum_required(VERSION 3.15)
project(Tora_Test VERSION 1.0.0 LANGUAGES CXX)

if(${CMAKE_CXX_STANDARD} LESS 20)
    return()
endif()

Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.4.0 # or a later release
)
FetchContent_MakeAvailable(Catch2)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)

file(GLOB TestPaths ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
foreach (TestPath ${TestPaths})
    get_filename_component(TestFileName ${TestPath} NAME_WLE)
    add_executable(${TestFileName} ${TestPath})
    target_link_options(${TestFileName}
      PRIVATE -fsanitize=address
      PRIVATE -fsanitize=leak
      PRIVATE -fsanitize=undefined
    )
    target_link_libraries(${TestFileName}
        PRIVATE Tora
        PRIVATE Catch2::Catch2WithMain
    )
    add_test(${TestFileName} ${TestFileName})
    # catch_discover_tests(${TestFileName})
endforeach (TestPath ${TestPaths})

    
