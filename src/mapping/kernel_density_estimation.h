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
} mine_tuple_t;

int kerneldensity(int **hotspot_map, kde_settings_t settings);
int kde(kde_settings_t settings, mine_tuple_t* mine_pos);
void kde_map_loop(kde_settings_t settings, double** kde_map, mine_tuple_t* mine_pos);
double kde_density (int x_tile, int y_tile, kde_settings_t settings, mine_tuple_t* mine_pos);
double kde_k_function(int x_tile, int y_tile, int x_mine, int y_mine, kde_settings_t settings);
void kde_map_normalizer(kde_settings_t settings, double **kde_map, mine_tuple_t *mine_pos);
void print_kde_map(kde_settings_t settings, double** kde_map);
void print_hotspots_map(kde_settings_t settings, mine_tuple_t  *mine_pos);

#endif //P1_KERNEL_DENSITY_ESTIMATION_H
