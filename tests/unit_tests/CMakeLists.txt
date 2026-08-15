add_executable(scriptorium_inkstone_unit_tests
    test_main.cpp
    core/tokenizer/tokenizer_test.cpp
    core/dictionary/dictionary_test.cpp
)

target_link_libraries(scriptorium_inkstone_unit_tests PRIVATE
    scriptorium::felt::infra::utils
    scriptorium::inkstone::core
    scriptorium::inkstone::manager
    gtest_main
    gmock_main
)

target_compile_definitions(scriptorium_inkstone_unit_tests PRIVATE PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}")
