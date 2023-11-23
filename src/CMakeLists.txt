add_executable(a_star pathfinding/a_star.c)

add_executable(main main.c
        valuation/valuation_functions.c
        valuation/valuation_functions.h
        mapping/mapping_functions.c
        mapping/mapping_functions.h

)

add_subdirectory(combiner)
target_link_libraries(main combiner)

install(TARGETS a_star)
