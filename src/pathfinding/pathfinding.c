#include "pathfinding.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define DIAG_MULT sqrt(2)

double pathfinding_main(double **cost_map, int x_size, int y_size, int x_target, int y_target, int x_start, int y_start) {
    // Creating a 2D array for the value of the KDE values.
    // TODO, need to verify that memory has been allocated.
    tile_t **pathfinding_map = malloc(sizeof(tile_t *) * x_size);
    for (int y = 0; y < y_size; y++) {
        pathfinding_map[y] = malloc(sizeof(tile_t) * y_size);
    }

    // Setting up settings and initializing the map.
    pathfinding_settings_t settings = {x_size, y_size, x_target, y_target};
    initialize_map(pathfinding_map, cost_map, settings);

    // Assigning g- and f_scores to the starting tile.
    update_f_g_scores(pathfinding_map[x_start][y_start], 0);

    // Setting up the queue element for the start tile.
    queue_item_t *start_element = (queue_item_t *) malloc(sizeof (queue_item_t));
    start_element->tile_p = &pathfinding_map[x_start][y_start];
    start_element->next_p = NULL;

    // Setting up the queue head, pointing towards the first tile.
    queue_item_t queue_head;
    queue_head.next_p = start_element;

    int num_neighbors;
    int adjacency_status;
    double new_g_score;
    tile_t neighbor;

    // Going through the queue so long as the head of the queue is not the target tile.
    while (queue_head.next_p->tile_p->h_score != 0) {
        tile_t current_tile = *queue_head.tile_p;

        tile_t *neighbors = malloc(sizeof(tile_t) * 8);

        num_neighbors = populate_neighbors(current_tile, neighbors, pathfinding_map, settings);

        for (int i = 0; i < num_neighbors; i++) {
            neighbor = neighbors[i];

            // Need to figure out whether the connection is diagonal or orthogonal.
            adjacency_status = abs(current_tile.x - neighbor.x) + abs(current_tile.y - neighbor.y);

            /* If the connection is orthogonal, the new g score is simply the g score of the current
             * tile, plus the cost of the neighbor tile. If the connection is diagonal, multiply the
             * neighbor cost by sqrt(2) before adding it to the g score of the current tile.
             */
            if (adjacency_status == 1) {
                new_g_score = current_tile.g_score + neighbor.cost;
            } else {
                new_g_score = current_tile.g_score + neighbor.cost * DIAG_MULT;
            }


            /* If the neighbor g score has not been set, set it immediately, then add it to the queue.
             * If the g score has been set, but is larger than the new g score, update it and move
             * it to its new position in the queue.
             * In both of those cases, set the source of the neighbor to the current tile.
             * If neither condition is true, we do nothing.
             * */
            if (neighbor.g_score == -1) {
                update_f_g_scores(neighbor, new_g_score);
                neighbor.source_p = &current_tile;
                insert_queue_element(neighbor, queue_head);

            } else if (neighbor.g_score > new_g_score) {
                update_f_g_scores(neighbor, new_g_score);
                neighbor.source_p = &current_tile;
                insert_existing_queue_element(neighbor, queue_head);
            }

        }
        /* Free the memory allocated to neighbors.
         * Could probably just overwrite previous values in each loop, but I don't want to
         * do that in case something goes wrong.
         * */
        free(neighbors);

        /* Once an item has been processed, remove it from the queue,
         * freeing the memory and removing pointers to it. */
        queue_head.next_p->tile_p->queue_p = NULL;
        queue_head.next_p = queue_head.next_p->next_p;
        free(queue_head.next_p->prev_p);
        queue_head.next_p->prev_p = NULL;
    }

    tile_t target_tile = *queue_head.tile_p;

    double total_cost = target_tile.g_score;
    printf("Total cost: %lf", total_cost);

    // Freeing allocated memory.
    free_queue(&queue_head);
    free_map(pathfinding_map, y_size);

    return total_cost;
}

void initialize_map(tile_t **pathfinding_map, double **cost_map, pathfinding_settings_t settings) {
    for (int x = 0; x < settings.x_size; x++) {
        for (int y = 0; y < settings.y_size; y++) {
            tile_t tile;
            tile.x = x;
            tile.y = y;
            tile.cost = cost_map[x][y];

            // Using taxi cab distance as h_score.
            tile.h_score = 10.0 * (abs(settings.x_target - x) + abs(settings.y_target - y));
            tile.g_score = -1; // Meaning uninitialized.
            tile.f_score = -1;
            tile.source_p = NULL;
            tile.queue_p = NULL;

            pathfinding_map[x][y] = tile;
        }
    }
}

void update_f_g_scores(tile_t tile, double new_g_score) {
    tile.g_score = new_g_score;
    tile.f_score = tile.h_score + tile.g_score;
}

void free_queue(queue_item_t *head) {
    queue_item_t *current = head;
    queue_item_t *next;

    while (current != NULL) {
        next = current->next_p;
        free(current);
        current = next;
    }
}

void free_map(tile_t **pathfinding_map, int y_size) {
    for (int y = 0; y < y_size; y++) {
        free(pathfinding_map[y]);
    }
    free(pathfinding_map);
}

int populate_neighbors(tile_t current_tile, tile_t *neighbors, tile_t **pathfinding_map, pathfinding_settings_t settings) {
    int num_neighbors = 0;

    // Checking orthogonally and diagonally adjacent positions of the current tile.
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            int x_neighbor = current_tile.x + i;
            int y_neighbor = current_tile.y + j;

            // Skip if the tile is the current tile, or would be outside the map.
            if ((i == 0 && j == 0) || x_neighbor == -1 || y_neighbor == -1
                || x_neighbor == settings.x_size || y_neighbor == settings.y_size) {
                continue;
            }

            /* If the tile position is valid, add it as the ith element of neighbors
             * and increment the number of neighbors. */
            neighbors[i] = pathfinding_map[x_neighbor][y_neighbor];
            num_neighbors++;
        }
    }
    return num_neighbors;
}

void insert_queue_element(tile_t neighbor, queue_item_t queue_head) {

    queue_item_t *element_insert;
    // Creating a queue element, if the tile does not already have one.
    if (neighbor.queue_p == NULL) {
        element_insert = (queue_item_t *) malloc(sizeof(queue_item_t));
        element_insert->tile_p = &neighbor;
        element_insert->next_p = NULL;
        element_insert->prev_p = NULL;
    } else {
        element_insert = (queue_item_t *) neighbor.queue_p;
    }

    /* Look at the second item in the queue and ask whether its f score is smaller than
     * the f score of the currently examined neighbor tile. If so, examine the next
     * element in the queue, so long as the next element exists, i.e. is not NULL.
     */
    queue_item_t queue_element, queue_element_temp;

    queue_element = *queue_head.next_p;
    while (queue_element.tile_p->f_score < neighbor.f_score) {
        /* If the next queue element is not null, change the next queue element
         * to instead be the next element of the current next queue element.
         * If it is null, append the new element at the end of the list.
         * */
        if (queue_element.next_p != NULL) {
            queue_element = *queue_element.next_p;
        } else {
            queue_element.next_p = element_insert;
            element_insert->prev_p = &queue_element;
            element_insert->next_p = NULL;
        }
    }

    /* If the element_insert was not appended to the end of the list, queue_element
     * will be the last element in the list which is smaller than the new element.
     * If this is the case, we will insert the new element there.
     * We know this is the case if the pointers for the new element are unassigned.
     * */
    if (element_insert->next_p == NULL) {
        queue_element_temp = *queue_element.next_p;
        queue_element.next_p = element_insert;
        element_insert->prev_p = &queue_element;

        queue_element_temp.prev_p = element_insert;
        element_insert->next_p = &queue_element_temp;
    }
}

void insert_existing_queue_element(tile_t neighbor, queue_item_t queue_head) {
    /* Start off by removing the queue item from the list, by making the elements
     * before and after the current queue item point towards each other, and
     * setting the prev and next pointers for the current element to NULL.
     * */
    queue_item_t current_element, prev_element, next_element;
    current_element = *neighbor.queue_p;
    prev_element = *current_element.prev_p;
    next_element = *current_element.next_p;

    prev_element.next_p = &next_element;
    next_element.prev_p = &prev_element;

    current_element.prev_p = NULL;
    current_element.next_p = NULL;

    // Then, call insert_queue_element.
    insert_queue_element(neighbor, queue_head);
}