


cmake_minimum_required(VERSION 3.15)

project(Tora VERSION 1.0.0 LANGUAGES CXX)

include(CMake/Options.cmake)

# Library
include(CMake/CompilerFlags.cmake)

add_subdirectory(Libs/Nezumi)

add_library(${Tora_TARGET_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${Tora_TARGET_NAME} 
    ALIAS ${Tora_TARGET_NAME}
)
target_include_directories(${Tora_TARGET_NAME} INTERFACE
    $<BUILD_INTERFACE:${Tora_INCLUDE_BUILD_DIR}>
    $<INSTALL_INTERFACE:${Tora_INCLUDE_INSTALL_DIR}>
)
target_link_libraries(${Tora_TARGET_NAME} 
    INTERFACE ${PROJECT_NAME}::CompilerFlags
    INTERFACE Nezumi
)

# Install
include(CMakePackageConfigHelpers)

install(TARGETS ${Tora_TARGET_NAME}
        EXPORT ${Tora_TARGET_NAME}Targets
        PUBLIC_HEADER DESTINATION 
            ${Tora_INCLUDE_BUILD_DIR}/${Tora_TARGET_NAME}
)

write_basic_package_version_file("${Tora_TARGET_NAME}ConfigVersion.cmake"
                                 VERSION ${PROJECT_VERSION}
                                 COMPATIBILITY SameMajorVersion)

configure_package_config_file(
    "${Tora_CMAKE_MODULE_PATH}/${Tora_TARGET_NAME}Config.cmake.in"
    "${Tora_BINARY_DIR}/${Tora_TARGET_NAME}Config.cmake"
    INSTALL_DESTINATION CMake/${Tora_TARGET_NAME}
)

# install(DIRECTORY ${PROJECT_SOURCE_DIR}/Source/ 
#     DESTINATION ${PROJECT_SOURCE_DIR}/Include/${PROJECT_NAME}
#     # PATTERN "CMakeLists.txt" EXCLUDE
# )


# add_subdirectory(Tests)
add_subdirectory(Examples)