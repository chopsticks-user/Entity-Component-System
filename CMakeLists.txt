


cmake_minimum_required(VERSION 3.15)
project(ECS VERSION 1.0.0 LANGUAGES CXX)

# include(GNUInstallDirs)
include(CMake/BuildOptions.cmake)
include(CMake/CompilerFlags.cmake)
include(CMake/Packages.cmake)


add_subdirectory(Include)
add_subdirectory(Tests)
add_subdirectory(Examples)


