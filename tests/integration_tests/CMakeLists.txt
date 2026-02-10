add_executable(modian_inkstone_integration_tests
    test_main.cpp
    inkstone/server_test.cpp
)

target_link_libraries(modian_inkstone_integration_tests
    modian::inkstone::core
    modian::inkstone::server
    modian::inkstone::logger
    modian_inkstone_test_utils
    gtest_main
    gmock_main
)

target_compile_definitions(modian_inkstone_integration_tests PRIVATE PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}")
