add_subdirectory(mapping)
add_subdirectory(pathfinding)

add_executable(main main.c)
target_link_libraries(main terrain_map)
