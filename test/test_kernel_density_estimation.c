#include "mtest.h"
#include "kernel_density_estimation.h"

#include <stdlib.h>

TEST_CASE(kernel_density_estimation, {
    printf("Kernel Density Estimate function testing.\n");
    printf("Setting up a map with known settings and mine positions.\n");
    hotspot_tuple_t hotspot_pos[3];
    hotspot_pos[0].x_pos = 4; hotspot_pos[0].y_pos = 0;
    hotspot_pos[1].x_pos = 4; hotspot_pos[1].y_pos = 3;
    hotspot_pos[2].x_pos = 1; hotspot_pos[2].y_pos = 3;


    kde_settings_t kde_settings = {5, 5, 1, 3};

    // Creating a 2D array for the value of the KDE values.
    double **kde_map = malloc(sizeof(double *) * kde_settings.x_size);
    for (int y = 0; y < kde_settings.y_size; y++) {
        kde_map[y] = malloc(sizeof(double) * kde_settings.y_size);
    }
    kde_main(kde_map, kde_settings, hotspot_pos);

    // Test cases, verified by hand.
    printf("Checking hand-verified equalities.\n");
    // All hotspots should have the exact same value, which should be 1.0.
    CHECK_EQ_DOUBLE(kde_map[4][0], kde_map[4][3], 1e-6);
    CHECK_EQ_DOUBLE(kde_map[4][0], kde_map[1][3], 1e-6);
    CHECK_EQ_DOUBLE(1.0, kde_map[4][0], 1e-6);

    // Checking a few other tiles which should be pairwise identical.
    CHECK_EQ_DOUBLE(kde_map[4][1], kde_map[2][3], 1e-6);
    CHECK_EQ_DOUBLE(kde_map[4][2], kde_map[3][3], 1e-6);

    // Checking column 1.
    CHECK_EQ_DOUBLE(0.006999, kde_map[0][0], 1e-6);
    CHECK_EQ_DOUBLE(0.022093, kde_map[1][0], 1e-6);
    CHECK_EQ_DOUBLE(0.141982, kde_map[2][0], 1e-6);
    CHECK_EQ_DOUBLE(0.607943, kde_map[3][0], 1e-6);

    printf("Freeing memory used.\n");
    // Frees the memory used for the map of the KDE values.
    for (int y = 0; y < kde_settings.y_size; y++) {
        free(kde_map[y]);
    }
    free(kde_map);
    printf("\n");
})

MAIN_RUN_TESTS(kernel_density_estimation)
