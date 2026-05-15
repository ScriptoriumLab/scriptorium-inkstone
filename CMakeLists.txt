cmake_minimum_required(VERSION 3.25)
project(modian-inkstone)

set(CMAKE_CXX_STANDARD 23)

include(FetchContent)
FetchContent_Declare(
    modian_common
    GIT_REPOSITORY https://github.com/ModianIME/modian-common
    BRANCH main
)
FetchContent_MakeAvailable(modian_common)

include(CTest)

add_subdirectory(core)
add_subdirectory(manager)
add_subdirectory(inkstone)

if (BUILD_TESTING)
    add_subdirectory(tests)
endif ()
