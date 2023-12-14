#include "pathfinding.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define DIAG_MULT sqrt(2)
#define G_INF 1000000 // Value exceeding any number g should ever reach.


double pathfinding_main(double **cost_map, int x_size, int y_size, int x_target, int y_target, int x_start, int y_start) {
    // Creating a 2D array for the value of the KDE values.
    // TODO, need to verify that memory has been allocated.
    printf("Creating pathfinding map\n");
    tile_t **pathfinding_map = malloc(sizeof(tile_t *) * x_size);
    for (int y = 0; y < y_size; y++) {
        pathfinding_map[y] = malloc(sizeof(tile_t) * y_size);
    }

    printf("Initializing settings and map\n");
    // Setting up settings and initializing the map.
    pathfinding_settings_t settings = {x_size, y_size, x_target, y_target};
    initialize_map(pathfinding_map, cost_map, settings);

    // Assigning g- and f_scores to the starting tile.
    update_f_g_scores(&pathfinding_map[x_start][y_start], 0);

    printf("Creating initial queue element for start tile\n");
    // Setting up the queue element for the start tile.
    queue_item_t *start_element = (queue_item_t *) malloc(sizeof (queue_item_t));
    start_element->tile_p = &pathfinding_map[x_start][y_start];
    start_element->next_p = NULL;

    // Setting up the queue head, pointing towards the first tile.
    queue_item_t queue_head;
    queue_head.next_p = start_element;
    start_element->prev_p = &queue_head;

    int num_neighbors;
    int adjacency_status;
    double new_g_score;

    printf("Allocating neighbors\n");
    tile_t **neighbors[8];
    tile_t *neighbor;

    printf("Starting the loop\n");
    // Going through the queue so long as the head of the queue is not the target tile.
    while (queue_head.next_p->tile_p->h_score != 0) {


        tile_t current_tile = *(queue_head.next_p->tile_p);

        printf("Current tile: (%d, %d), (g, h): (%lf, %lf)\n",
               current_tile.x, current_tile.y, current_tile.g_score, current_tile.h_score);

        num_neighbors = populate_neighbors(current_tile, neighbors, pathfinding_map, settings);

        for (int i = 0; i < num_neighbors; i++) {
            neighbor = neighbors[i];

            // Need to figure out whether the connection is diagonal or orthogonal.
            adjacency_status = abs(current_tile.x - neighbor->x) + abs(current_tile.y - neighbor->y);

            /* If the connection is orthogonal, the new g score is simply the g score of the current
             * tile, plus the cost of the neighbor tile. If the connection is diagonal, multiply the
             * neighbor cost by sqrt(2) before adding it to the g score of the current tile.
             */
            if (adjacency_status == 1) {
                new_g_score = current_tile.g_score + neighbor->cost;
            } else {
                new_g_score = current_tile.g_score + neighbor->cost * DIAG_MULT;
            }


            /* If the neighbor g score has not been set, set it immediately, then add it to the queue.
             * If the g score has been set, but is larger than the new g score, update it and move
             * it to its new position in the queue.
             * In both of those cases, set the source of the neighbor to the current tile.
             * If neither condition is true, we do nothing.
             * */
            if (neighbor->g_score == G_INF) {
                update_f_g_scores(neighbor, new_g_score);
                neighbor->source_p = queue_head.next_p->tile_p;
                insert_queue_element(&pathfinding_map[neighbor->x][neighbor->y], queue_head);

            } else if (neighbor->g_score > new_g_score) {
                update_f_g_scores(neighbor, new_g_score);
                neighbor->source_p = queue_head.next_p->tile_p;
                insert_existing_queue_element(&pathfinding_map[neighbor->x][neighbor->y], queue_head);
            }
        }
        // printf("Deleting the current queue element from the queue, making the second queue element the first\n");
        /* Once an item has been processed, remove it from the queue,
         * freeing the memory and removing pointers to it. */
        queue_head.next_p->tile_p->queue_p = NULL;
        queue_head.next_p = queue_head.next_p->next_p;
        free(queue_head.next_p->prev_p);
        queue_head.next_p->prev_p = NULL;
    }

    tile_t target_tile = *(queue_head.next_p->tile_p);

    tile_t tile = target_tile;
    tile_t* source_p = tile.source_p;

    // Print out the path found.
    while ( source_p != NULL ) {
        printf("%d, %d\n", source_p->x, source_p->y);
        tile = *source_p;
        source_p = tile.source_p;
    }

    double total_cost = target_tile.g_score;

    printf("Total cost: %lf\n", total_cost);

    // Freeing allocated memory.
    printf("Freeing memory used for pathfinding.\n");
    free_map_pathfinding(pathfinding_map, y_size);
    printf("Map cleared.\n");
    free_queue(&queue_head);
    printf("Queue cleared.\n");
    printf("All memory freed.\n");

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
            tile.g_score = G_INF; // Meaning uninitialized.
            tile.f_score = G_INF;
            tile.source_p = NULL;
            tile.queue_p = NULL;

            pathfinding_map[x][y] = tile;
        }
    }
}

void update_f_g_scores(tile_t *tile, double new_g_score) {
    tile->g_score = new_g_score;
    tile->f_score = tile->h_score + tile->g_score;
}

void free_queue(queue_item_t *head) {
    queue_item_t *current = head->next_p;
    queue_item_t *next;

    while (current != NULL) {
        next = current->next_p;
        free(current);
        current = next;
    }
}

void free_map_pathfinding(tile_t **pathfinding_map, int y_size) {
    for (int y = 0; y < y_size; y++) {
        free(pathfinding_map[y]);
    }
    free(pathfinding_map);
}

int populate_neighbors(tile_t current_tile, tile_t **neighbors, tile_t **pathfinding_map, pathfinding_settings_t settings) {
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
            neighbors[num_neighbors] = &pathfinding_map[x_neighbor][y_neighbor];
            num_neighbors++;
        }
    }
    return num_neighbors;
}

int insert_queue_element(tile_t *tile, queue_item_t queue_head) {

    queue_item_t *element_insert;
    // Creating a queue element, if the tile does not already have one.
    if (tile->queue_p == NULL) {
        element_insert = (queue_item_t *) malloc(sizeof(queue_item_t));
        element_insert->tile_p = tile;
        element_insert->next_p = NULL;
        element_insert->prev_p = NULL;
    } else {
        element_insert = tile->queue_p;
    }

    // Look at the second item in the queue, since the first is always the currently active tile.
    queue_item_t* queue_element;
    queue_element = queue_head.next_p->next_p;

    // If the second element is a null p, set the inserted element as the second element and return.
    if (queue_element == NULL ) {
        queue_head.next_p->next_p = element_insert;
        element_insert->prev_p = queue_head.next_p;
        return (0);
    }

    /* Otherwise, ask whether the second queue element has a larger f_score than the
     * inserted element. If so, make the inserted element the second queue element and return.
     */
    if (tile->f_score < queue_element->tile_p->f_score) {
        insert_queue_element_before(element_insert, queue_element);
        return (0);
    }

    /* If the function has not returned yet, loop over the remaining queue elements,
     * by asking whether the current queue element has a smaller f_score than the
     * f_score of the neighbor. If so, move queue element one down in the queue.
     */
    while (queue_element->tile_p->f_score < tile->f_score) {
        queue_element = queue_element->next_p;
        /* Now look at the next pointer of the queue element. If this pointer
         * is the null pointer, it means that the queue element is the final
         * element in the queue, and so the inserted element should simply
         * be added to the end of the list.
         * Otherwise, queue_element is the first element in the queue which is
         * greater than or equal to the inserted element.
         */
        if (queue_element->next_p == NULL) {
            queue_element->next_p = element_insert;
            element_insert->prev_p = queue_element;
            element_insert->next_p = NULL;
            return (0);
        }
    }
    /* If queue_element is the first element in the queue which is
     * greater than or equal to the inserted element, we insert the
     * new element in its place, bumping the queue_element up in the queue. */

    insert_queue_element_before(element_insert, queue_element);
    return (0);
}

void insert_existing_queue_element(tile_t *tile, queue_item_t queue_head) {
    /* Start off by removing the queue item from the list, by making the elements
     * before and after the current queue item point towards each other, and
     * setting the prev and next pointers for the current element to NULL.
     * */

    queue_item_t *current_element, *prev_element, *next_element;
    current_element = tile->queue_p;
    prev_element = current_element->prev_p;
    next_element = current_element->next_p;

    prev_element->next_p = next_element;
    next_element->prev_p = prev_element;

    current_element->prev_p = NULL;
    current_element->next_p = NULL;

    // Then, call insert_queue_element.
    insert_queue_element(tile, queue_head);
}

void insert_queue_element_before(queue_item_t *new_element, queue_item_t *next_element) {
    new_element->next_p = next_element;
    new_element->prev_p = next_element->prev_p;
    next_element->prev_p = new_element;
    if (new_element->prev_p != NULL) {
        new_element->prev_p->next_p = new_element;
    }
}
