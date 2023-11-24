add_executable(a_star pathfinding/a_star.c)

add_executable(main main.c
        valuation/valuation_functions.c
        valuation/valuation_functions.h
        mapping/mapping_functions.c
        mapping/mapping_functions.h
        mapping/read_print_terrain_map.c
        mapping/read_print_terrain_map.h)