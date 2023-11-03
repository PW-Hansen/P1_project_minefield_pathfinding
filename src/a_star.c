#include <stdio.h>
#include <math.h>

/* Currently assumes that the map is a square. Add rectangle functionality later?
 * */

#define MAP_SIZE 10
#define NUM_NEIGHBORS 8
#define F_INF 1000000
#define G_SCORE_ORTH 10
#define G_SCORE_DIAG 14
/* This value should be larger than any value the f_value could theoretically reach.*/

typedef struct {
    int x, y;
    int f_score, g_score, h_score;
    int source;
    int in_queue;
} tile_t;

typedef struct {
    int x, y;
    int source;
    int g_val_added;
} neighbor_info_t;

void map_initialize(tile_t map[MAP_SIZE][MAP_SIZE]);

void tile_print(tile_t tile);

tile_t queue_open_fetch_first_element(tile_t map[MAP_SIZE][MAP_SIZE]);

void populate_neighbors(tile_t current_tile, neighbor_info_t neighbors[8]);

void initialize_h_score(tile_t *tile);

void update_g_f_scores(tile_t *tile, int new_g_val);

void queue_open_insert_tile(tile_t queue_open[MAP_SIZE * MAP_SIZE], tile_t new_tile);

int queue_items = 0;
int x_target, y_target;
tile_t queue_open[MAP_SIZE * MAP_SIZE]; /* Could probably be much smaller. */

int main() {
    tile_t map[MAP_SIZE][MAP_SIZE];
    int x_start = 2, y_start = 2;
    x_target = 8, y_target = 5;

    for (int i = 0; i < MAP_SIZE * MAP_SIZE; i++) {
        queue_open[i].f_score = F_INF;
    }

    map_initialize(map);

    queue_open[0] = map[x_start][y_start];
    queue_open[0].g_score = 0;
    queue_open[0].h_score = 10 * (abs(x_target - x_start) + abs(y_target - y_start));
    queue_open[0].f_score = queue_open[0].g_score + queue_open[0].h_score;
    queue_open[0].in_queue = 0;

    queue_items++;

    while (queue_items > 0) {
        tile_t current_tile = queue_open_fetch_first_element(map);

        tile_print(current_tile);

        if (current_tile.h_score == 0) {
            /* TODO, reconstruct path. */
            break;
        }

        neighbor_info_t neighbors[8];
        populate_neighbors(current_tile, neighbors);

        for (int i = 0; i < NUM_NEIGHBORS; i++) {
            /* Skips surplus neighbors, as could happen if the current tile is
             * adjacent to a border of the map. */
            if (neighbors[i].source == -1) {
                continue;
            }

            tile_t neighbor_tile = map[neighbors[i].x][neighbors[i].y];

            int prelim_g_score = current_tile.g_score + neighbors[i].g_val_added;

            /* Check whether the neighboring tile has had scores initialized yet. If not
             * initialize its scores. */
            if (neighbor_tile.source == -1) {
                neighbor_tile.source = neighbors[i].source;
                initialize_h_score(&neighbor_tile);
                update_g_f_scores(&neighbor_tile, prelim_g_score);
                queue_open_insert_tile(queue_open, neighbor_tile);
                tile_print(neighbor_tile);
            } else if (neighbor_tile.g_score > prelim_g_score) {
                continue; /* TODO, needs to move the old instance of the tile.*/
            }

            map[neighbors[i].x][neighbors[i].y] = neighbor_tile;
        }
    }

    tile_print(map[x_target][y_target]);

    return (0);
};

void tile_print(tile_t tile) {
    printf("Position: ( %d, %d )\n", tile.x, tile.y);
    printf("Source: %d\n", tile.source);
    printf("g-val: %d\n", tile.g_score);
    printf("h-val: %d\n", tile.h_score);
    printf("f-val: %d\n", tile.f_score);
}

void map_initialize(tile_t map[MAP_SIZE][MAP_SIZE]) {
    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            map[i][j].x = i;
            map[i][j].y = j;
            map[i][j].source = -1; /* Source is used as a proxy for uninitialized f, g, and h values. */
        }
    }
}

tile_t queue_open_fetch_first_element(tile_t map[MAP_SIZE][MAP_SIZE]) {
    tile_t current_tile = queue_open[0];

    for (int i = 0; i < queue_items; i++) {
        queue_open[i] = queue_open[i + 1];
    }
    queue_items--;

    map[current_tile.x][current_tile.y] = current_tile;

    return current_tile;
}

void populate_neighbors(tile_t current_tile, neighbor_info_t neighbors[8]) {
    /* Setting source to -1 initially, as to be able to distinguish between
     * neighbors which are and are not set by this function. */
    for (int i = 0; i < NUM_NEIGHBORS; i++) {
        neighbors[i].source = -1;
    }

    int source = 0;
    int neighbors_populated = 0;

    /* Checking orthogonally and diagonally adjacent positions of the current tile,
     * making sure to throw out positions which would be located outside of the map.
     * */
    for (int i = -1; i < 2; i++, source++) {
        for (int j = -1; j < 2; j++, source++) {
            int x_neighbor = current_tile.x + i;
            int y_neighbor = current_tile.y + j;

            if ((i == 0 && j == 0) || x_neighbor == -1 || y_neighbor == -1
                || x_neighbor == MAP_SIZE || y_neighbor == MAP_SIZE) {

                continue;
            }

            /* Adjacency status, will be 1 if orthogonal and 2 if diagonal.
             * After that, set g scores appropriately.*/
            int adjacency_status = abs(i) + abs(j);
            int g_val_added;
            if (adjacency_status == 1) {
                g_val_added = G_SCORE_ORTH;
            } else {
                g_val_added = G_SCORE_DIAG;
            }

            neighbors[neighbors_populated].x = x_neighbor;
            neighbors[neighbors_populated].y = y_neighbor;
            neighbors[neighbors_populated].source = source;
            neighbors[neighbors_populated].g_val_added = g_val_added;
            neighbors_populated++;
        }
    }
}

void initialize_h_score(tile_t *tile) {
    int new_h_val = 10 * (abs(x_target - tile->x) + abs(y_target - tile->y));
    tile->h_score = new_h_val;
}

void update_g_f_scores(tile_t *tile, int new_g_val) {
    tile->g_score = new_g_val;
    tile->f_score = tile->g_score + tile->h_score;
}

void queue_open_insert_tile(tile_t queue_open[MAP_SIZE * MAP_SIZE], tile_t new_tile) {
    /* TODO, doesn't work right now. */
    /* Use binary sort to figure out where the new tile should be put in the open queue. */
    int low = 0;
    int high = queue_items;
    int mid;
    int g_score_actual = new_tile.g_score;

    while (low < high) {
        mid = (int) (high - low) / 2 + low;
        if (queue_open[mid].g_score < g_score_actual) {
            low = mid + 1;
        } else if (queue_open[mid].g_score > g_score_actual) {
            high = mid - 1;
        } else {
            break;
        }
    }

    /* Now move all elements which are above mid one space to the right,
     * then insert the new tile in place mid in the open queue. */
    for (int i = queue_items; i >= mid; i--) {
        queue_open[i] = queue_open[i - 1];
    }
    queue_open[mid] = new_tile;

    queue_items++;
}