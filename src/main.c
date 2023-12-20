#include <stdio.h>
#include <stdlib.h>

#include <stdlib.h>
#include "combiner.h"
#include "kernel_density_estimation.h"
#include "terrain_map.h"
#include "settings.h"
#include "pathfinding.h"
#include "evaluation.h"


int main() {
    // Sanity test Settings:
    printf("Tile base cost: %d\n", SETTINGS_TILE_COST);
    printf("Field tile cost: %d\n", SETTINGS_TERRAIN_FIELD_COST);

    char file_path[1024];
    char file_hotspot_path[1024];
    printf("Enter map file path: ");
    (void) scanf(" %s", file_path);
    printf("Enter hotspot file path: ");
    (void) scanf(" %s", file_hotspot_path);

    map_t terrain_map = map_from_ppm(file_path);

    // TODO: MOVE OUT TO OWN FUNCTION
    for (int i = 0; i < terrain_map.width; i++) {
        for (int j = 0; j < terrain_map.height; j++) {
            printf("%1d", terrain_map.data[i][j]);
        }
        printf("\n");
    }

    // Initializing an array for the hotspots. Length is set at the max possible value.
    hotspot_tuple_t hotspot_pos[terrain_map.width * terrain_map.height];
    int hotspot_num = hotspot_pos_from_ppm(file_hotspot_path, hotspot_pos);

    kde_settings_t kde_settings = {terrain_map.width, terrain_map.height, SETTINGS_LANDMINE_BANDWIDTH, hotspot_num};

    // Creating a 2D array for the value of the KDE values.
    double **kde_map = malloc(sizeof(double *) * kde_settings.x_size);
    for (int y = 0; y < kde_settings.y_size; y++) {
        kde_map[y] = malloc(sizeof(double) * kde_settings.y_size);
    }
    kde_main(kde_map, kde_settings, hotspot_pos);


    // Dynamically allocate the cost map
    double **cost_map = malloc(sizeof(double *) * kde_settings.x_size);
    for (int y = 0; y < kde_settings.y_size; y++) {
        cost_map[y] = malloc(sizeof(double) * kde_settings.y_size);
    }

    combine_arrays(terrain_map, kde_map, cost_map);

    // Free memory used for the terrain.
    free_map(terrain_map);
    // Frees the memory used for the map of the KDE values.
    for (int y = 0; y < kde_settings.y_size; y++) {
        free(kde_map[y]);
    }
    free(kde_map);

    pathfinding_output_t pf_output = pathfinding_main(cost_map, kde_settings.y_size, kde_settings.y_size,
                                         TARGET_X, TARGET_Y, START_X, START_Y);

    printf("Total road-building cost: %lf\n", pf_output.cost);
    printf("Road-building length: %lf\n", pf_output.length);

    // Frees the memory used for the combined values map.
    for (int y = 0; y < kde_settings.y_size; y++) {
        free(cost_map[y]);
    }
    free(cost_map);
    printf("Thank you for using our software :)\n");

    economic_evaluation(pf_output);

    return EXIT_SUCCESS;
}
