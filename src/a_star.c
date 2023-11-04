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
    int in_queue; /* TODO, used if printing images of the progress.*/
} tile_t;

typedef struct {
    int x, y;
    int source;
    int g_score_added;
} neighbor_info_t;

void initialize_map(tile_t map[MAP_SIZE][MAP_SIZE]);

void initialize_queue();

void initialize_start_tile(tile_t map[MAP_SIZE][MAP_SIZE], int x_start, int y_start);

void print_tile(tile_t tile);

tile_t queue_fetch_first_element(tile_t map[MAP_SIZE][MAP_SIZE]);

void populate_neighbors_info(tile_t current_tile, neighbor_info_t neighbors_info[8]);

void initialize_h_score(tile_t *tile);

void update_g_f_scores(tile_t *tile, int new_g_val);

void queue_insert_tile(tile_t new_tile);

int queue_items = 0;
int x_target, y_target;
tile_t queue[MAP_SIZE * MAP_SIZE]; /* Could probably be much smaller. */

int main() {
    tile_t map[MAP_SIZE][MAP_SIZE];

    /* Setting values manually for now. In the future, this should be either loaded
     * from the command line or through an image of some kind. */
    int x_start = 2, y_start = 2;
    x_target = 8, y_target = 5;

    /* Initializing the map, the queue, and the starting tile. */
    initialize_map(map);
    initialize_queue();
    initialize_start_tile(map, x_start, y_start);

    /* Looping over elements in the queue to iterate towards the target. */
    while (queue_items > 0) {
        /* Fetch the first item in the queue, removing them from the queue.*/
        tile_t current_tile = queue_fetch_first_element(map);
        print_tile(current_tile);

        /* If the current tile has a h_score of 0, we have reached the target tile
         * and no longer need to iterate over the queue. Instead, reconstruct the
         * path to the target file and print it out. */
        if (current_tile.h_score == 0) {
            /* TODO, reconstruct path. */
            break;
        }

        /* Examine the (up to) eight neighbors of the tile, returning information needed later,
         * then iterate over each valid (i.e., not outside the map) neighbor.*/
        neighbor_info_t neighbors_info[8];
        populate_neighbors_info(current_tile, neighbors_info);

        for (int i = 0; i < NUM_NEIGHBORS; i++) {
            /*  If the neighbor would be invalid, such as being outside the map, skip it. */
            if (neighbors_info[i].source == -1) {
                continue;
            }

            /* Otherwise, load the tile and determine what g-score it would have,
             * if accessed from the current tile.*/
            tile_t neighbor_tile = map[neighbors_info[i].x][neighbors_info[i].y];
            int prelim_g_score = current_tile.g_score + neighbors_info[i].g_score_added;

            /* Check whether the neighboring tile has had scores initialized yet. If not
             * initialize its scores, with the current tile as its source, and insert it
             * into the queue in the appropriate location.
             * If not, check whether accessing the tile from the current tile would lead
             * to a lower g-score, and if so, update its values and its queue position. */
            if (neighbor_tile.source == -1) {
                neighbor_tile.source = neighbors_info[i].source;
                initialize_h_score(&neighbor_tile);
                update_g_f_scores(&neighbor_tile, prelim_g_score);
                queue_insert_tile(neighbor_tile);
            } else if (neighbor_tile.g_score > prelim_g_score) {
                continue; /* TODO, also needs to move the old instance of the tile. */
                neighbor_tile.source = neighbors_info[i].source;
                update_g_f_scores(&neighbor_tile, prelim_g_score);
            }

            /* Update the map with the information processed above for the
             * neighboring tile. */
            map[neighbors_info[i].x][neighbors_info[i].y] = neighbor_tile;
        }
    }

    print_tile(map[x_target][y_target]);

    return (0);
}

void print_tile(tile_t tile) {
    printf("Position: ( %d, %d )\n", tile.x, tile.y);
    printf("Source: %d\n", tile.source);
    printf("g-val: %d\n", tile.g_score);
    printf("h-val: %d\n", tile.h_score);
    printf("f-val: %d\n", tile.f_score);
}

void print_neighbor_info(neighbor_info_t neighbor_info) {
    printf("x:       %d", neighbor_info.x);
    printf("y:       %d", neighbor_info.y);
    printf("source:  %d", neighbor_info.source);
    printf("g added: %d", neighbor_info.g_score_added);
}

void initialize_map(tile_t map[MAP_SIZE][MAP_SIZE]) {
    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            map[i][j].x = i;
            map[i][j].y = j;
            map[i][j].source = -1; /* Source is used as a proxy for uninitialized f, g, and h values. */
        }
    }
}

void initialize_queue() {
    for (int i = 0; i < MAP_SIZE * MAP_SIZE; i++) {
        queue[i].f_score = F_INF;
    }
}

void initialize_start_tile(tile_t map[MAP_SIZE][MAP_SIZE], int x_start, int y_start) {
    tile_t start_tile = map[x_start][y_start];
    initialize_h_score(&start_tile);
    update_g_f_scores(&start_tile, 0);

    queue[0] = start_tile;

    queue_items++;
}

tile_t queue_fetch_first_element(tile_t map[MAP_SIZE][MAP_SIZE]) {
    tile_t current_tile = queue[0];

    for (int i = 0; i < queue_items; i++) {
        queue[i] = queue[i + 1];
    }
    queue_items--;

    map[current_tile.x][current_tile.y] = current_tile;

    return current_tile;
}

void populate_neighbors_info(tile_t current_tile, neighbor_info_t neighbors_info[8]) {
    /* Setting source to -1 initially, as to be able to distinguish between
     * neighbors_info which are and are not set by this function. */
    for (int i = 0; i < NUM_NEIGHBORS; i++) {
        neighbors_info[i].source = -1;
    }

    int source = 0;
    int neighbors_info_populated = 0;

    /* Checking orthogonally and diagonally adjacent positions of the current tile,
     * making sure to throw out positions which would be located outside the map.
     * */
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            int x_neighbor = current_tile.x + i;
            int y_neighbor = current_tile.y + j;

            if ((i == 0 && j == 0) || x_neighbor == -1 || y_neighbor == -1
                || x_neighbor == MAP_SIZE || y_neighbor == MAP_SIZE) {
                source++;
                continue;
            }

            /* Adjacency status, will be 1 if orthogonal and 2 if diagonal.
             * After that, set g scores appropriately.*/
            int adjacency_status = abs(i) + abs(j);
            int g_score_added;
            if (adjacency_status == 1) {
                g_score_added = G_SCORE_ORTH;
            } else {
                g_score_added = G_SCORE_DIAG;
            }

            neighbors_info[neighbors_info_populated].x = x_neighbor;
            neighbors_info[neighbors_info_populated].y = y_neighbor;
            neighbors_info[neighbors_info_populated].source = source;
            neighbors_info[neighbors_info_populated].g_score_added = g_score_added;
            neighbors_info_populated++;
            source++;
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

void queue_insert_tile(tile_t new_tile) {
    /* Use binary sort to figure out where the new tile should be put in the queue. */
    /* This could be a linear sort instead, as new tiles added to the queue are likely
     * to be near the top, as they will be at most one step away from the until then
     * best tile. */
    int low = 0;
    int high = queue_items;
    int mid = (int) (high - low) / 2 + low;
    int f_score_actual = new_tile.f_score;

    while (low < high) {
        if (queue[mid].f_score < f_score_actual) {
            low = mid + 1;
        } else if (queue[mid].f_score > f_score_actual) {
            high = mid - 1;
        } else {
            break;
        }
        mid = (int) (high - low) / 2 + low;
    }

    /* Now move all elements which are above mid one space to the right,
     * then insert the new tile in place mid in the queue. */
    for (int i = queue_items; i >= mid; i--) {
        queue[i] = queue[i - 1];
    }
    queue[mid] = new_tile;

    queue_items++;
}