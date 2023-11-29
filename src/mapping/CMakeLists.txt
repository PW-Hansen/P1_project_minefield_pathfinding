add_library(terrain_map STATIC terrain_map.c)
target_include_directories(terrain_map PUBLIC .)

add_library(kernel_density_estimation STATIC kernel_density_estimation.c)
target_link_libraries(kernel_density_estimation m)
target_include_directories(kernel_density_estimation PUBLIC .)
