


# Package Container
message("Building Container library...")
execute_process (
    COMMAND bash -c "cd ${PROJECT_SOURCE_DIR}/Libs/Container && 
        mkdir -p Build && cd Build && cmake .. && cmake --build ."
)
# add_subdirectory(Libs/Container)
find_package(Container CONFIG REQUIRED 
    PATHS ${PROJECT_SOURCE_DIR}/Libs/Container/Build
)
message("Finished\n")

# Package TPL
message("Building TPL library")
execute_process (
    COMMAND bash -c "cd ${PROJECT_SOURCE_DIR}/Libs/TPL && 
        mkdir -p Build && cd Build && cmake .. && cmake --build ."
)
# add_subdirectory(Libs/TPL)
find_package(TPL CONFIG REQUIRED 
    PATHS ${PROJECT_SOURCE_DIR}/Libs/TPL/Build
)
message("Finished\n")


