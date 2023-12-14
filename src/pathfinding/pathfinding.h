//
// Created by Peter on 03/12/2023.
//

#ifndef P1_PROJECT_MINEFIELD_PATHFINDING_PATHFINDING_H
#define P1_PROJECT_MINEFIELD_PATHFINDING_PATHFINDING_H

typedef struct tile_s {
    int x, y;
    double f_score, g_score, h_score; // g is the cost of the tile, h is distance from target, f sums the two.
    double cost;
    struct tile_s *source_p;
    struct queue_item_s *queue_p;
} tile_t;

typedef struct queue_item_s {
    struct tile_s *tile_p;
    struct queue_item_s *prev_p;
    struct queue_item_s *next_p;
} queue_item_t;

typedef struct {
    int x_size;
    int y_size;
    int x_target;
    int y_target;
} pathfinding_settings_t;

typedef struct {
    double cost;
    double length;
} pathfinding_output_t;

pathfinding_output_t pathfinding_main(double **cost_map, int x_size, int y_size, int x_target, int y_target, int x_start, int y_start);

void initialize_map(tile_t **pathfinding_map, double **cost_map, pathfinding_settings_t settings);

void update_f_g_scores(tile_t *tile, double new_g_score);

void free_queue(queue_item_t *head);

void free_map_pathfinding(tile_t **pathfinding_map, int y_size);

int populate_neighbors(tile_t current_tile, tile_t **neighbors, tile_t **pathfinding_map, pathfinding_settings_t settings);

int insert_queue_element(tile_t *tile, queue_item_t queue_head);

void insert_existing_queue_element(tile_t *tile, queue_item_t queue_head);

void insert_queue_element_before(queue_item_t *new_element, queue_item_t *next_element);

#endif //P1_PROJECT_MINEFIELD_PATHFINDING_PATHFINDING_H
