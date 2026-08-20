add_executable(scriptorium_inkstone_integration_tests
    core/engine/input_engine_test.cpp
    inkstone/server_test.cpp
)

target_link_libraries(scriptorium_inkstone_integration_tests
    scriptorium::felt::core::logger
    scriptorium::felt::infra::logger
    scriptorium::inkstone::core
    scriptorium::inkstone::server
    scriptorium::inkstone::test_utils
    GTest::gtest_main
    GTest::gmock
)

target_compile_definitions(scriptorium_inkstone_integration_tests PRIVATE
    PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}"
)

include(GoogleTest)

gtest_discover_tests(
    scriptorium_inkstone_integration_tests
    PROPERTIES
        LABELS "inkstone-integration"
)
