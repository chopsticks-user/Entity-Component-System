


set(gcc_like_cxx $<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>)
set(msvc_cxx $<COMPILE_LANG_AND_ID:CXX,MSVC>)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(gcc_like_cxx_flags -pedantic;-Wall;-Wextra;
    -Wshadow;-Wconversion;-Wunreachable-code)
set(msvc_cxx_flags -W3)

add_library(${PROJECT_NAME}_CompilerFlags INTERFACE)
add_library(${PROJECT_NAME}::CompilerFlags
    ALIAS ${PROJECT_NAME}_CompilerFlags
)
target_compile_options(${PROJECT_NAME}_CompilerFlags INTERFACE
    $<${gcc_like_cxx}: $<BUILD_INTERFACE: ${gcc_like_cxx_flags}>>
    $<${msvc_cxx}: $<BUILD_INTERFACE: ${msvc_cxx_flags}>>
)