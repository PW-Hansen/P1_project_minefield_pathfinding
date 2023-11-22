add_executable(a_star pathfinding/a_star.c)

add_executable(main main.c
        valuation/valuation_functions.c
        valuation/valuation_functions.h
        mapping/kernel_density_estimation.c
        mapping/kernel_density_estimation.h)