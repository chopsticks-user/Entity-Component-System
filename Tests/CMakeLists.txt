

add_executable(TestEntityManager TestEntityManager.cpp)

target_include_directories(TestEntityManager 
    PRIVATE ${PROJECT_SOURCE_DIR}/Include
    # PRIVATE ${PROJECT_SOURCE_DIR}/Libs/Catch2/src
)

target_link_libraries(TestEntityManager 
    PRIVATE CompilerFlags
    PRIVATE Catch2::Catch2WithMain
    PRIVATE Catch2::Catch2
)

# set_property(TARGET TestBasic PROPERTY CXX_STANDARD 20)

# target_link_libraries(TestBasic PUBLIC CompilerFlags)

# include(CTest)
# include(Catch)
# catch_discover_tests(TestBasic)