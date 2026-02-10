add_library(modian_inkstone_infra_utils STATIC
    src/utils.cpp
)

target_include_directories(modian_inkstone_infra_utils PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

add_library(modian::inkstone::infra_utils ALIAS modian_inkstone_infra_utils)