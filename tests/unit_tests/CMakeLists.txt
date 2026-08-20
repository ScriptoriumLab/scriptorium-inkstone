add_executable(scriptorium_inkstone_unit_tests
    core/tokenizer/tokenizer_test.cpp
    core/dictionary/dictionary_test.cpp
)

target_link_libraries(scriptorium_inkstone_unit_tests PRIVATE
    scriptorium::felt::infra::utils
    scriptorium::inkstone::core
    scriptorium::inkstone::manager
    GTest::gtest_main
    GTest::gmock
)

target_compile_definitions(scriptorium_inkstone_unit_tests PRIVATE
    PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}"
)

include(GoogleTest)

gtest_discover_tests(
    scriptorium_inkstone_unit_tests
    PROPERTIES
        LABELS "inkstone-unit"
)
