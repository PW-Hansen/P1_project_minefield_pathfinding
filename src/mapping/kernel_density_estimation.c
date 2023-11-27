#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "kernel_density_estimation.h"

// Placeholder function, the contents of this should be in the mapping main function.
int kde_main(double **kde_map, kde_settings_t settings, hotspot_tuple_t *hotspot_pos) {

    for (int x = 0; x < settings.x_size; x++) {
        for (int y = 0; y < settings.y_size; y++) {
            kde_map[x][y] = kde_density(x, y, settings, hotspot_pos);
        }
    }

    kde_map_normalizer(settings, kde_map, hotspot_pos);

    // print_kde_map(settings, kde_map);

    // If testing is enabled, write out the KDE map to a file.
    if (settings.testing == 1) {
        FILE *fp = fopen("test/test_files/output.txt", "w");
        if (fp == NULL) {
            printf("Could not write to file.");
            exit(EXIT_FAILURE);
        }
        kde_output(fp, settings, kde_map);
        fclose(fp);

        FILE *fp_test = fopen("test/test_files/kde_test_1.txt", "r");
        FILE *fp_comp = fopen("test/test_files/output.txt", "r");

        kde_test(fp_test, fp_comp);

        fclose(fp_test);
        fclose(fp_comp);
    }

    return (0);
}

double kde_density(int x_tile, int y_tile, kde_settings_t settings, hotspot_tuple_t *hotspot_pos) {
    double val = 0;
    int x_hotspot, y_hotspot;

    for (int i = 0; i < settings.n; i++) {
        x_hotspot = hotspot_pos[i].x_pos;
        y_hotspot = hotspot_pos[i].y_pos;

        val += kde_k_function(x_tile, y_tile, x_hotspot, y_hotspot, settings);
    }

    val /= (settings.n * settings.bandwidth);

    return val;
}

double kde_k_function(int x_tile, int y_tile, int x_hotspot, int y_hotspot, kde_settings_t settings) {
    double distance = sqrt(pow(x_tile - x_hotspot, 2) + pow(y_tile - y_hotspot, 2));
    double geometric_factor = 1.0 / (sqrt(2.0 * M_PI));
    double exp_value = exp(-(pow((distance / settings.bandwidth), 2) / 2.0));
    double result = geometric_factor * exp_value;
    return result;
}

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
            printf("%3.3lf ", kde_map[x][y]);
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

void kde_output(FILE *fp, kde_settings_t settings, double **kde_map) {
    for (int x = 0; x < settings.x_size; x++) {
        for (int y = 0; y < settings.y_size; y++) {
            fprintf(fp, "%lf ", kde_map[x][y]);
        }
        fprintf(fp, "\n");
    }
}

void kde_test(FILE *test_file, FILE *compare_file) {
    int test_c, comp_c;

    do {
        test_c = fgetc(test_file);
        comp_c = fgetc(compare_file);

        if (test_c != comp_c) {
            printf("Files are not identical.\n");
            exit(EXIT_FAILURE);
        }

        if (feof(test_file)) {
            break;
        }

    } while (test_c == comp_c);
    printf("The two files are identical.");
}
