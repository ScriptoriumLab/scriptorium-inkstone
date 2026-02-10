set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
set(BENCHMARK_INSTALL OFF CACHE BOOL "" FORCE)
set(BENCHMARK_DOWNLOAD_DEPENDENCIES OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    googlebenchmark
    URL https://github.com/google/benchmark/archive/refs/tags/v1.9.5.zip
)

FetchContent_MakeAvailable(googlebenchmark)

add_executable(
    modian_inkstone_performance_tests
    server_benchmark.cpp
)

target_link_libraries(
    modian_inkstone_performance_tests PRIVATE
    benchmark::benchmark
    modian_inkstone_server
    modian::inkstone::core
    modian::inkstone::logger
    modian_inkstone_test_utils
)

target_compile_definitions(modian_inkstone_performance_tests PRIVATE PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}")
