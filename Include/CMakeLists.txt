


add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE
    ${PROJECT_SOURCE_DIR}/Include
    INTERFACE ${Container_INCLUDE_DIR}
    # $<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}>
    # $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
# target_link_libraries(${PROJECT_NAME}
#     # INTERFACE CompilerFlags
#     # INTERFACE Container
# )

install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}Targets
        PUBLIC_HEADER DESTINATION 
            ${PROJECT_SOURCE_DIR}/Include/${PROJECT_NAME}
)
include(CMakePackageConfigHelpers)
write_basic_package_version_file("${PROJECT_NAME}ConfigVersion.cmake"
                                 VERSION ${PROJECT_VERSION}
                                 COMPATIBILITY SameMajorVersion)
configure_package_config_file(
    "${PROJECT_SOURCE_DIR}/CMake/${PROJECT_NAME}Config.cmake.in"
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION CMake/${PROJECT_NAME}
)