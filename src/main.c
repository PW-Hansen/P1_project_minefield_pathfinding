#include <stdio.h>
#include <stdlib.h>
#include "mapping/kernel_density_estimation.h"

int main() {

    kde_settings_t kde_settings = {5, 5, 1, 3};

    int **map = malloc(sizeof(int *) * kde_settings.x_size);
    for (int i = 0; i < kde_settings.y_size; i++) {
        map[i] = malloc(sizeof(int) * kde_settings.y_size);
    }

    for (int x = 0; x < kde_settings.x_size; x++) {
        for (int y = 0; y < kde_settings.y_size; y++) {
            map[x][y] = 0;
        }
    }

    map[1][3] = 1;
    map[4][3] = 1;
    map[4][0] = 1;

    kernel_main(map, kde_settings);

    free(map);

    return (0);
}