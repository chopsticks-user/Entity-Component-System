set(gcc_like_cxx $<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>)
set(msvc_cxx $<COMPILE_LANG_AND_ID:CXX,MSVC>)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(ExceptionFlag)
if(ECS_ALLOW_EXCEPTIONS)
    set(ExceptionFlag -DECS_ALLOW_EXCEPTIONS)
endif()

set(gcc_like_cxx_flags ${ExceptionFlag};-Wall;-Wextra;-pedantic;-Wextra;
    -Wshadow;-Wconversion;-Wunreachable-code)
set(msvc_cxx_flags ${ExceptionFlag};-W3)

message(${gcc_like_cxx_flags})


add_library(CompilerFlags INTERFACE)
target_compile_features(CompilerFlags INTERFACE cxx_std_20)
target_compile_options(CompilerFlags INTERFACE
    $<${gcc_like_cxx}: $<BUILD_INTERFACE: ${gcc_like_cxx_flags}>>
    $<${msvc_cxx}: $<BUILD_INTERFACE: ${msvc_cxx_flags}>>
)