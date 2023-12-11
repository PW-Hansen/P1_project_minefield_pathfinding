#include <stdio.h>
#include <stdlib.h>

#include "combiner.h"

// Temp placeholder variables till merged with settings-feature
int base_cost = 5;
int mine_cost = 10;
int size = 64;

void combine_arrays(int **terrain_layout, double **kde_layout) {
    // Dynamically allocate the cost map
    double **cost_map = (double **)malloc(size * sizeof(double *));
    for (int y = 0; y < size; y++) {
        cost_map[y] = (double *)malloc(size * sizeof(double));
    }

    // Fill the cost_map with calculated costs
    for (int x = 0; x < size; x++) {
        for (int y = 0; y < size; y++) { 
            int terrain_cost;
            switch (terrain_layout[x][y]) {
                // Temp values which will be changed once merged with settings-feature
                case HILL:   terrain_cost = 5; break;
                case ROAD:   terrain_cost = 1; break;
                case FIELD:  terrain_cost = 3; break;
                case RIVER:  terrain_cost = 8; break;
                case FOREST: terrain_cost = 7; break;
                default:     terrain_cost = 0;
            }

            cost_map[x][y] = terrain_cost + base_cost + (mine_cost * kde_layout[x][y]);
            printf("%0.1lf, ", cost_map[x][y]);
        }
        printf("\n");
    }

    free(cost_map);
}
