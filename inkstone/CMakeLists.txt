add_executable(
    modian_inkstone
    main.cpp
)

target_link_libraries(modian_inkstone PRIVATE
    modian_inkstone_manager
    modian_inkstone_core
)

set_target_properties(modian_inkstone PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/ModianIME"
    OUTPUT_NAME "modian-inkstone"
)