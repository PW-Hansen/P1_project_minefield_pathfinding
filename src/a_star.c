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
    int valid;
} neighbor_info_t;

void initialize_map(tile_t map[MAP_SIZE][MAP_SIZE]);

void initialize_queue();

tile_t initialize_start_tile(tile_t map[MAP_SIZE][MAP_SIZE], int x_start, int y_start);

void print_tile(tile_t tile);

void print_neighbor_info(neighbor_info_t neighbor_info);

void print_path(tile_t end_tile, tile_t map[MAP_SIZE][MAP_SIZE]);

void print_queue_f_scores();

tile_t queue_fetch_first_element(tile_t map[MAP_SIZE][MAP_SIZE]);

void populate_neighbors_info(tile_t current_tile, neighbor_info_t neighbors_info[8]);

void initialize_h_score(tile_t *tile);

void update_g_f_scores(tile_t *tile, int new_g_val);

void queue_insert_tile(tile_t new_tile);

void queue_move_updated_tile(tile_t updated_tile, int old_f_score);

int queue_items = 0;
int x_target, y_target;
tile_t queue[MAP_SIZE * MAP_SIZE]; /* Could probably be much smaller. */

int main() {
    tile_t map[MAP_SIZE][MAP_SIZE];
    //
    /* Setting values manually for now. In the future, this should be either loaded
     * from the command line or through an image of some kind. */
    int x_start = 2, y_start = 2;
    x_target = 8, y_target = 5;

    /* Initializing the map, the queue, and the starting tile. */
    initialize_map(map);
    initialize_queue();
    map[x_start][y_start] = initialize_start_tile(map, x_start, y_start);

    /* Looping over elements in the queue to iterate towards the target. */
    while (queue_items > 0) {
        /* Fetch the first item in the queue, removing them from the queue.*/
        print_queue_f_scores();
        tile_t current_tile = queue_fetch_first_element(map);
        /* print_tile(current_tile); */

        /* If the current tile has a h_score of 0, we have reached the target tile
         * and no longer need to iterate over the queue. Instead, reconstruct the
         * path to the target file and print it out. */
        if (current_tile.h_score == 0) {
            print_path(current_tile, map);
            break;
        }

        /* Examine the (up to) eight neighbors of the tile, returning information needed later,
         * then iterate over each valid (i.e., not outside the map) neighbor.*/
        neighbor_info_t neighbors_info[8];
        populate_neighbors_info(current_tile, neighbors_info);

        for (int i = 0; i < NUM_NEIGHBORS; i++) {
            /*  If the neighbor would be invalid, such as being outside the map, skip it. */
            if (neighbors_info[i].valid == 1) {
                continue;
            }

            /* Otherwise, load the tile and determine what g-score it would have,
             * if accessed from the current tile.*/
            tile_t neighbor_tile = map[neighbors_info[i].x][neighbors_info[i].y];
            int prelim_g_score = current_tile.g_score + neighbors_info[i].g_score_added;

            /* Check whether the neighboring tile has had scores initialized yet. If not
             * initialize its scores, with the current tile as its source, and insert it
             * into the queue in the appropriate location.
             * If scores have been initialized, check whether accessing the tile from the
             * current tile would lead to a lower g-score, and if so, update its values
             * and its queue position. */
            if (neighbor_tile.f_score == -1) {
                neighbor_tile.source = neighbors_info[i].source;
                initialize_h_score(&neighbor_tile);
                update_g_f_scores(&neighbor_tile, prelim_g_score);
                queue_insert_tile(neighbor_tile);
            } else if (neighbor_tile.g_score > prelim_g_score) {
                int old_f_score = neighbor_tile.g_score;
                neighbor_tile.source = neighbors_info[i].source;
                update_g_f_scores(&neighbor_tile, prelim_g_score);
                queue_move_updated_tile(neighbor_tile, old_f_score);
                /* TODO, needs testing, but testing needs invalid tile functionality,
                 * which hasn't been done yet. */
            }

            /* Update the map with the information processed above for the
             * neighboring tile. */
            map[neighbors_info[i].x][neighbors_info[i].y] = neighbor_tile;
        }
    }


    /* print_tile(map[x_target][y_target]); */

    return (0);
}

void print_tile(tile_t tile) {
    printf("Position: ( %d, %d )\n", tile.x, tile.y);
    printf("Source:  %d\n", tile.source);
    printf("g-score: %d\n", tile.g_score);
    printf("h-score: %d\n", tile.h_score);
    printf("f-score: %d\n", tile.f_score);
}

void print_neighbor_info(neighbor_info_t neighbor_info) {
    printf("x:       %d", neighbor_info.x);
    printf("y:       %d", neighbor_info.y);
    printf("source:  %d", neighbor_info.source);
    printf("g added: %d", neighbor_info.g_score_added);
}

void print_path(tile_t end_tile, tile_t map[MAP_SIZE][MAP_SIZE]) {
    int direction_x[9] = {1, 1, 1, 0, 0, 0, -1, -1, -1};
    int direction_y[9] = {1, 0, -1, 1, 0, -1, 1, 0, -1};
    int x, y;

    tile_t path_tile = end_tile;

    printf("End tile:\n");
    while (path_tile.g_score != 0) {
        print_tile(path_tile);
        int source = path_tile.source;
        x = path_tile.x + direction_x[source];
        y = path_tile.y + direction_y[source];
        path_tile = map[x][y];
    }
    printf("Start tile:\n");
    print_tile(path_tile);

}

void print_queue_f_scores() {
    for (int i = 0; i < queue_items; i++) {
        printf("%4d ", queue[i].f_score);
    }
    printf("\n");
}

void initialize_map(tile_t map[MAP_SIZE][MAP_SIZE]) {
    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            map[i][j].x = i;
            map[i][j].y = j;
            map[i][j].f_score = -1; /* Negative f-score, meaning that the tile has not yet been initialized. */
        }
    }
}

void initialize_queue() {
    for (int i = 0; i < MAP_SIZE * MAP_SIZE; i++) {
        queue[i].f_score = F_INF;
    }
}

tile_t initialize_start_tile(tile_t map[MAP_SIZE][MAP_SIZE], int x_start, int y_start) {
    tile_t start_tile = map[x_start][y_start];
    initialize_h_score(&start_tile);
    update_g_f_scores(&start_tile, 0);
    start_tile.source = -1;

    queue[0] = start_tile;

    queue_items++;

    return start_tile;
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
    /* Tiles are assumed to be invalid by default. Valid tiles  will have this
     * value set to 1 by the program. */
    for (int i = 0; i < NUM_NEIGHBORS; i++) {
        neighbors_info[i].valid = 0;
    }

    int source = -1;
    int neighbor_info_num = 0;

    /* Checking orthogonally and diagonally adjacent positions of the current tile. */
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            source++;
            int x_neighbor = current_tile.x + i;
            int y_neighbor = current_tile.y + j;

            /* Skip if the tile would be outside the map. */
            if ((i == 0 && j == 0) || x_neighbor == -1 || y_neighbor == -1
                || x_neighbor == MAP_SIZE || y_neighbor == MAP_SIZE) {
                continue;
            }
            /* TODO, skip if tile is invalid. */

            /* Adjacency status, will be 1 if orthogonal and 2 if diagonal.
             * After that, set g scores appropriately.*/
            int adjacency_status = abs(i) + abs(j);
            int g_score_added;
            if (adjacency_status == 1) {
                g_score_added = G_SCORE_ORTH;
            } else {
                g_score_added = G_SCORE_DIAG;
            }

            neighbors_info[neighbor_info_num].x = x_neighbor;
            neighbors_info[neighbor_info_num].y = y_neighbor;
            neighbors_info[neighbor_info_num].source = source;
            neighbors_info[neighbor_info_num].g_score_added = g_score_added;
            neighbor_info_num++;
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

void queue_move_updated_tile(tile_t updated_tile, int old_f_score) {
    /* Changes the position of a tile already in the queue.
     * First, finds where the tile currently is in the queue,
     * then figure out where it should be. Bump all elements
     * between those two points one position back in the queue,
     * then insert the tile into its new position.*/
    int low = 0;
    int high = queue_items;
    int mid = (int) (high - low) / 2 + low;
    int f_score_actual = old_f_score;

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
    /* There is a chance that mid is in the middle of a succession of tiles
     * with identical f-scores to the tile we are updating. Thus, we need to
     * decrement mid so long as the preceding tile has equal f-score.*/
    while (queue[mid - 1].f_score == queue[mid].f_score) {
        mid--;
        /* Stop the loop if the mid variable reaches 0. Otherwise, the while
         * condition would try to access something outside the array. */
        if (mid == 0) {
            break;
        }
    }

    /* Now start at mid and increment until the updated tile has been found.
     * Before that, set a variable, allowing a check of whether the tile
     * has been found. */
    int found = 0;
    int old_pos;
    for (int i = mid; queue[i].f_score == old_f_score; i++) {
        tile_t queue_tile = queue[i];
        if (queue_tile.x == updated_tile.x && queue_tile.y == updated_tile.y) {
            found = 1;
            old_pos = 1;
            break;
        }
    }

    /* Warning in case */
    if (found == 0) {
        printf("Tile queue position not updated! Pos: (%d, %d)\n", updated_tile.x, updated_tile.y);
        return;
    }

    /* Now move all queue items above the old position in the queue
     * one place to the left in the queue. */
    for (int i = old_pos; i <= queue_items; i--) {
        queue[i] = queue[i + 1];
    }
    /* Since we removed an item from the queue, albeit temporarily,
     * queue_items should be decremented. */
    queue_items--;

    /* Finally, call the function to insert the updated tile into the queue. */
    queue_insert_tile(updated_tile);
}