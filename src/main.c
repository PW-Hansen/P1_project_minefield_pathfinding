#include <stdio.h>
#include <stdlib.h>

#include <stdlib.h>
#include "kernel_density_estimation.h"
#include "terrain_map.h"
#include "settings.h"

int main() {
    // Sanity test Settings:
    printf("Tile base cost: %d\n", SETTINGS_TILE_COST);
    printf("Field tile cost: %d\n", SETTINGS_TERRAIN_FIELD_COST);

    char file_path[1024];
    char file_path_hotspots[1024];
    printf("Enter file path: ");
    (void) scanf(" %s", file_path);
    map_t terrain_map = map_from_ppm(file_path);

    for (int i = 0; i < terrain_map.width; i++) {
        for (int j = 0; j < terrain_map.height; j++) {
            printf("%1d", terrain_map.data[i][j]);
        }
        printf("\n");
    }

    free_map(terrain_map);
    printf("Thank you for using our software :)\n");

    // Initializing an array for the hotspots. Length is set at the max possible value.
    hotspot_tuple_t hotspot_pos[terrain_map.width * terrain_map.height];
    int hotspot_num;

    printf("Enter file path: ");
    (void) scanf(" %s", file_path_hotspots);
    hotspot_num = hotspot_pos_from_ppm(file_path_hotspots, hotspot_pos);

    kde_settings_t kde_settings = {terrain_map.width, terrain_map.height, 1, hotspot_num, 0};

    // Creating a 2D array for the value of the KDE values.
    double **kde_map = malloc(sizeof(double *) * kde_settings.x_size);
    for (int y = 0; y < kde_settings.y_size; y++) {
        kde_map[y] = malloc(sizeof(double) * kde_settings.y_size);
    }
    kde_main(kde_map, kde_settings, hotspot_pos);

    // Frees the memory used for the map of the KDE values.
    for (int y = 0; y < kde_settings.y_size; y++) {
        free(kde_map[y]);
    }
    free(kde_map);

    return 0;
}
