add_executable(
    modian_inkstone
    main.cpp
)

target_link_libraries(modian_inkstone PRIVATE modian_inkstone_lib)

set_target_properties(modian_inkstone PROPERTIES
    OUTPUT_NAME "modian-inkstone"
)