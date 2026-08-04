add_executable(scriptorium_inkstone_integration_tests
    test_main.cpp
    core/engine/input_engine_test.cpp
    inkstone/server_test.cpp
)

target_link_libraries(scriptorium_inkstone_integration_tests
    scriptorium::felt::core::logger
    scriptorium::felt::infra::logger
    scriptorium::inkstone::core
    scriptorium::inkstone::server
    scriptorium::inkstone::test_utils
    gtest_main
    gmock_main
)

target_compile_definitions(scriptorium_inkstone_integration_tests PRIVATE PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}")
