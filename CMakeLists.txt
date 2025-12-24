cmake_minimum_required(VERSION 3.25)
project(modian-inkstone)

set(CMAKE_CXX_STANDARD 23)

include(CTest)

add_subdirectory(core)
add_subdirectory(manager)
add_subdirectory(infra)
add_subdirectory(inkstone)

if (BUILD_TESTING)
    add_subdirectory(tests)
endif ()