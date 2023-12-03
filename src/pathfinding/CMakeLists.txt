add_library(pathfinding STATIC pathfinding.c)
target_link_libraries(pathfinding m)
target_include_directories(pathfinding PUBLIC .)
