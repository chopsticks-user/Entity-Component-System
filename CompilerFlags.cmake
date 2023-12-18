set(gcc_like_cxx $<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>)
set(msvc_cxx $<COMPILE_LANG_AND_ID:CXX,MSVC>)

set(gcc_like_cxx_flags -Wall;-Wextra;-pedantic;-Wextra;
    -Wshadow;-Wconversion;-Wunreachable-code)
set(msvc_cxx_flags -W3)

option(CMAKE_BUILD_TYPE Debug)
option(BUILD_SHARED_LIBS OFF)

add_library(CompilerFlags INTERFACE)
target_compile_features(CompilerFlags INTERFACE cxx_std_20)
target_compile_options(CompilerFlags INTERFACE
    $<${gcc_like_cxx}: $<BUILD_INTERFACE: ${gcc_like_cxx_flags}>>
    $<${msvc_cxx}: $<BUILD_INTERFACE: ${msvc_cxx_flags}>>
)