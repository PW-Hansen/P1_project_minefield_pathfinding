#include <stdio.h>
#include <stdlib.h>

int size = 64;

void combine_arrays(/*int **terrain_layout, double **kde_layout*/) {
    double **cost_map = (double **)malloc(size * sizeof(double *));

    for (int i = 0; i < size; i++) {
        cost_map[i] = (double *)malloc(size * sizeof(double));
    }

    free(cost_map);
}
