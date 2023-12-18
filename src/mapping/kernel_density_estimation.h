//
// Created by Peter on 20/11/2023.
//

#ifndef P1_KERNEL_DENSITY_ESTIMATION_H
#define P1_KERNEL_DENSITY_ESTIMATION_H

typedef struct {
    int x_size;
    int y_size;
    int bandwidth;
    int n;
} kde_settings_t;

typedef struct {
    int x_pos;
    int y_pos;
} hotspot_tuple_t;

int kde_main(double **kde_map, kde_settings_t settings, hotspot_tuple_t *hotspot_pos);

double kde_density(int x_tile, int y_tile, kde_settings_t settings, hotspot_tuple_t *hotspot_pos);

double kde_k_function(int x_tile, int y_tile, int x_hotspot, int y_hotspot, kde_settings_t settings);

void kde_map_normalizer(kde_settings_t settings, double **kde_map, hotspot_tuple_t *hotspot_pos);

void print_kde_map(kde_settings_t settings, double **kde_map);

void print_hotspots_map(kde_settings_t settings, hotspot_tuple_t *hotspot_pos);

void kde_output(FILE *fp, kde_settings_t settings, double **kde_map);

void kde_test(FILE *test_file, FILE *compare_file);

#endif //P1_KERNEL_DENSITY_ESTIMATION_H
