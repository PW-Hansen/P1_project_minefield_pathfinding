add_subdirectory(combiner)
add_subdirectory(mapping)
add_subdirectory(pathfinding)

add_library(settings INTERFACE settings.h)

add_executable(main main.c)
target_link_libraries(main combiner terrain_map kernel_density_estimation settings)

install(TARGETS main)
