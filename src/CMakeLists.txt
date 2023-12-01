add_subdirectory(combiner)
add_subdirectory(mapping)
add_subdirectory(pathfinding)

add_executable(main main.c)
target_link_libraries(main terrain_map kernel_density_estimation)

install(TARGETS main)
