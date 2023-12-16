#include <stdio.h>
#include <stdlib.h>

#include "combiner.h"
#include "../settings.h"

void combine_arrays(int **terrain_layout, double **kde_layout, double **cost_map) {
    int size_x = GRID_SIZE_X,
        size_y = GRID_SIZE_Y,
        base_cost = SETTINGS_TILE_COST,
        mine_cost = SETTINGS_LANDMINE_COST;
    
    int lookup_table[5] = {
        SETTINGS_TERRAIN_HILL_COST,
        SETTINGS_TERRAIN_ROAD_COST,
        SETTINGS_TERRAIN_FIELD_COST,
        SETTINGS_TERRAIN_RIVER_COST,
        SETTINGS_TERRAIN_FOREST_COST
    };

    printf("\n\nCombined map:\n");
    // Fill the cost_map with calculated costs
    for (int x = 0; x < size_x; x++) {
        for (int y = 0; y < size_y; y++) { 
            int terrain_cost = lookup_table[terrain_layout[x][y] - 1];  // Subtract one due to terrain_layout starting at 1
            cost_map[x][y] = terrain_cost + base_cost + (mine_cost * kde_layout[x][y]);
            printf("%0.1lf, ", cost_map[x][y]);
        }
        printf("\n");
    }
}
