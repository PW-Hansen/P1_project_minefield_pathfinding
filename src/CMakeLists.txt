cmake_minimum_required(VERSION 3.12)

project(P1_project)

add_subdirectory(combiner)
add_subdirectory(mapping)
add_subdirectory(pathfinding)
add_subdirectory(evaluation)

add_library(settings INTERFACE settings.h)

add_executable(main main.c)
target_link_libraries(main combiner terrain_map kernel_density_estimation pathfinding evaluation settings)

install(TARGETS main)
