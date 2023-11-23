#include <stdio.h>
#include <stdlib.h>

typedef enum {
    HILL = 1,
    ROAD = 2,
    FIELD = 3,
    RIVER = 4,
    FOREST = 5,
} TileType;

// Temp placeholder variables till merged with settings-feature
int base_cost = 5;
int mine_cost = 69;
int size = 64;

void combine_arrays(int **terrain_layout, double **kde_layout) {
    // Dynamically allocate the cost map
    double **cost_map = (double **)malloc(size * sizeof(double *));
    for (int i = 0; i < size; i++) {
        cost_map[i] = (double *)malloc(size * sizeof(double));
    }

    // Fill the cost_map with calculated costs
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) { 
            int terrain_cost;
            switch (terrain_layout[i][j]) {
                // Temp values which will be changed once merged with settings-feature
                case HILL:   terrain_cost = 5; break;
                case ROAD:   terrain_cost = 1; break;
                case FIELD:  terrain_cost = 3; break;
                case RIVER:  terrain_cost = 8; break;
                case FOREST: terrain_cost = 7; break;
                default:     terrain_cost = 0;
            }

            cost_map[i][j] = terrain_cost + base_cost + (mine_cost * kde_layout[i][j]);
            printf("%0.3lf, ", cost_map[i][j]);
        }
        printf("\n");
    }

    free(cost_map);
}
