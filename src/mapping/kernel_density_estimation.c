#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "kernel_density_estimation.h"

/* Needs to be renamed to something else, as the only main function in the code should be the overall main,
 * in ./main.c. */

int kerneldensity() {
    // Setting setup.
    kde_settings_t settings = {10, 10, 1, 3};

    // TODO, proper initialization. Right now we just randomly generate some mines.
    mine_tuple_t* mine_pos = malloc(sizeof(mine_tuple_t) * settings.n);
    for (int i = 0; i < settings.n; i++) {
        mine_pos[i].x_pos = rand() % settings.x_size; // NOLINT
        mine_pos[i].y_pos = rand() % settings.y_size; // NOLINT
    }

    kde(settings, mine_pos);

    // Freeing memory initialized earlier.
    free(mine_pos);

    return (0);
}

int kde(kde_settings_t settings, mine_tuple_t* mine_pos) {


    // Creating a 2D array for the value of the KDE kde_map.
    double** kde_map = malloc(sizeof(double*) * settings.x_size);
    for (int i = 0; i < settings.y_size; i++) {
        kde_map[i] = malloc(sizeof(double) * settings.y_size);
    }

    // Looping over the tiles in the kde_map to determine their kde values.
    kde_map_loop(settings, kde_map, mine_pos);

    // Printing out the map.
    print_kde_map(settings, kde_map, mine_pos);

    // Freeing memory initialized earlier.
    for (int i = 0; i < settings.y_size; i++) {
        free(kde_map[i]);
    }
    free(kde_map);







    return (0);
}

void kde_map_loop(kde_settings_t settings, double** kde_map, mine_tuple_t* mine_pos) {
    for (int x = 0; x < settings.x_size; x++) {
        for (int y = 0; y < settings.y_size; y++) {
             kde_map[x][y] = kde_density(x, y, settings, mine_pos);
        }
    }
}

double kde_density (int x_tile, int y_tile, kde_settings_t settings, mine_tuple_t* mine_pos) {
    double val = 0;
    int x_mine, y_mine;

    for (int i = 0; i < settings.n; i++) {
        x_mine = mine_pos[i].x_pos;
        y_mine = mine_pos[i].y_pos;

        val += kde_k_function(x_tile, y_tile, x_mine, y_mine, settings);
    }

    val /= (settings.n * settings.bandwidth);

    return val;
}

double kde_k_function(int x_tile, int y_tile, int x_mine, int y_mine, kde_settings_t settings) {
    double distance = sqrt(pow(x_tile - x_mine,2) + pow(y_tile - y_mine,2));
    double geometric_factor = 1.0 / (sqrt(2.0 * M_PI));
    double exp_value = exp(- (pow((distance / settings.bandwidth),2)/2.0));
    double result = geometric_factor * exp_value;
    return result;
}

void print_kde_map(kde_settings_t settings, double** kde_map, mine_tuple_t* mine_pos) {
    int map[settings.x_size][settings.y_size];
    for (int i = 0; i < settings.x_size; i++) {
        for (int j = 0; j < settings.y_size; j++) {
            map[i][j] = 0;
        }
    }

    int mine_x, mine_y;
    for (int i = 0; i < settings.n; i++) {
        mine_x = mine_pos[i].x_pos;
        mine_y = mine_pos[i].y_pos;
        map[mine_x][mine_y] += 1;
    }


    double val;
    printf("The map is %d by %d and contains %d mines.\n", settings.x_size, settings.y_size, settings.n);
    printf("KDE map:\n");
    for (int i = 0; i < settings.x_size; i++) {
        for (int j = 0; j < settings.y_size; j++) {
            printf("%3.3lf ", kde_map[i][j]);
            val += kde_map[i][j];
        }
        printf("\n");
    }
    printf("\nTotal value is: %lf\n", val);

    printf("Mine map:\n");
    for (int i = 0; i < settings.x_size; i++) {
        for (int j = 0; j < settings.y_size; j++) {
            printf("%d ", map[i][j]);
        }
        printf("\n");
    }
}