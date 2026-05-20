add_executable(modian_inkstone_integration_tests
    test_main.cpp
    core/engine/input_engine_test.cpp
    inkstone/server_test.cpp
)

target_link_libraries(modian_inkstone_integration_tests
    modian::common::core::logger
    modian::common::infra::logger
    modian::inkstone::core
    modian::inkstone::server
    modian::inkstone::test_utils
    gtest_main
    gmock_main
)

target_compile_definitions(modian_inkstone_integration_tests PRIVATE PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}")
