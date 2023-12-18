#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "kernel_density_estimation.h"

/**
 * Function to populate a supplied map with kernel density estimates when supplied
 * with a list of hotspots and settings.
 * @param kde_map the destination where the kernel density estimated are saved to.
 * @param settings settings, containing information such as the size, the bandwidth
 * used for calculating the kernel density, and the number of hotspots.
 * @param hotspot_pos positions in the map where there's a mine hotspot.
 * @return 0, to verify that the function ran correctly.
 */
int kde_main(double **kde_map, kde_settings_t settings, hotspot_tuple_t *hotspot_pos) {
    for (int x = 0; x < settings.x_size; x++) {
        for (int y = 0; y < settings.y_size; y++) {
            kde_map[x][y] = kde_density(x, y, settings, hotspot_pos);
        }
    }

    kde_map_normalizer(settings, kde_map, hotspot_pos);

    print_kde_map(settings, kde_map);

    return (0);
}

/**
 * Calculates the kernel density estimate for a tile in the map, based on the positions of all
 * suspected hotspots.
 * @param x_tile
 * @param y_tile
 * @param settings
 * @param hotspot_pos
 * @return
 */
double kde_density(int x_tile, int y_tile, kde_settings_t settings, hotspot_tuple_t *hotspot_pos) {
    double val = 0;
    int x_hotspot, y_hotspot;

    for (int i = 0; i < settings.n; i++) {
        x_hotspot = hotspot_pos[i].x_pos;
        y_hotspot = hotspot_pos[i].y_pos;

        val += kde_k_function(x_tile, y_tile, x_hotspot, y_hotspot, settings);
    }

    // Part of the formula we use to compute KDE.
    val /= (settings.n * settings.bandwidth);

    return val;
}

/**
 * Calculates the KDE contribution from a specified mine hotspot to a specified map tile.
 * @param x_tile
 * @param y_tile
 * @param x_hotspot
 * @param y_hotspot
 * @param settings
 * @return
 */
double kde_k_function(int x_tile, int y_tile, int x_hotspot, int y_hotspot, kde_settings_t settings) {
    double distance = sqrt(pow(x_tile - x_hotspot, 2) + pow(y_tile - y_hotspot, 2));
    double geometric_factor = 1.0 / (sqrt(2.0 * M_PI));
    double exp_value = exp(-(pow((distance / settings.bandwidth), 2) / 2.0));
    double result = geometric_factor * exp_value;
    return result;
}

/**
 * Function to normalize the estimates so that all hotspots will have a value of 1. The function
 * finds the hotspot position with the lowest value in the KDE map, then divides all entries in
 * the map by this value, and finally ensures that no value in the KDE map is greater than 1.
 * @param settings
 * @param kde_map
 * @param hotspot_pos
 */
void kde_map_normalizer(kde_settings_t settings, double **kde_map, hotspot_tuple_t *hotspot_pos) {
    double min_hotspot_val = 1e3;
    double kde_hotspot_val;

    // Figure out the highest kde value.
    for (int i = 0; i < settings.n; ++i) {
        kde_hotspot_val = kde_map[hotspot_pos[i].x_pos][hotspot_pos[i].y_pos];
        if (kde_hotspot_val < min_hotspot_val) {
            min_hotspot_val = kde_hotspot_val;
        }
    }

    /* Divide all map tiles by the minimum hotspot value to normalize it, then
     * make sure that no tiles have a value above 1, as these values are supposed
     * to represent the probability of a specific tile in the map containing hotspots.
     * */
    for (int x = 0; x < settings.x_size; x++) {
        for (int y = 0; y < settings.y_size; y++) {
            kde_map[x][y] /= min_hotspot_val;
            if (kde_map[x][y] > 1.) {
                kde_map[x][y] = 1.;
            }
        }
    }
}

void print_kde_map(kde_settings_t settings, double **kde_map) {
    printf("The map is %d by %d and contains %d hotspots.\n", settings.x_size, settings.y_size, settings.n);
    printf("KDE map:\n");
    for (int x = 0; x < settings.x_size; x++) {
        for (int y = 0; y < settings.y_size; y++) {
            printf("%.1lf ", kde_map[x][y]);
        }
        printf("\n");
    }
}

void print_hotspots_map(kde_settings_t settings, hotspot_tuple_t *hotspot_pos) {
    int map[settings.x_size][settings.y_size];
    for (int x = 0; x < settings.x_size; x++) {
        for (int y = 0; y < settings.y_size; y++) {
            map[x][y] = 0;
        }
    }

    int hotspot_x, hotspot_y;
    for (int i = 0; i < settings.n; i++) {
        hotspot_x = hotspot_pos[i].x_pos;
        hotspot_y = hotspot_pos[i].y_pos;
        map[hotspot_x][hotspot_y] += 1;
    }

    printf("Hotspot map:\n");
    for (int x = 0; x < settings.x_size; x++) {
        for (int y = 0; y < settings.y_size; y++) {
            printf("%d ", map[x][y]);
        }
        printf("\n");
    }
}

/**
 * Prints the KDE map to a specified file path.
 * @param fp
 * @param settings
 * @param kde_map
 */
void kde_output(FILE *fp, kde_settings_t settings, double **kde_map) {
    for (int x = 0; x < settings.x_size; x++) {
        for (int y = 0; y < settings.y_size; y++) {
            fprintf(fp, "%.0lf ", kde_map[x][y]);
        }
        fprintf(fp, "\n");
    }
}
