#include "mtest.h"

#include "pathfinding.h"
#include "kernel_density_estimation.h"

#include <stdlib.h>

// Pathfinding tests.
TEST_CASE(pathfinding_queue_inserting, {
    printf("Testing pathfinding queue insert functions.\n");
    printf("Creating tiles for testing.\n");
    // Setting up some tiles to test.
    int test_num = 8;
    double random_number;
    tile_t tiles[test_num];
    printf("Randomly assigning f-scores of non-initial tiles between 1 and 32.\n");
    for (int i = 0; i < test_num; ++i) {
        random_number = 1. + (rand() % 32);
        tiles[i].f_score = random_number;
        tiles[i].queue_p = NULL;
    }
    // Setting f_score of initial tile.
    tiles[0].f_score = 0;

    printf("Creating initial queue item and queue head.\n");

    queue_item_t *start_element = (queue_item_t *) malloc(sizeof (queue_item_t));
    start_element->tile_p = &tiles[0];
    start_element->next_p = NULL;

    // Setting up the queue head, pointing towards the first tile.
    queue_item_t queue_head;
    queue_head.next_p = start_element;
    start_element->prev_p = &queue_head;

    printf("Inserting tiles into the queue.\n");
    struct queue_item_s *prev_p = NULL;
    for (int i = test_num - 1; i > 0; --i) {
        insert_queue_element(&tiles[i], queue_head);
    }

    queue_item_t* current_element = queue_head.next_p;
    printf("Queue elements f-scores: \n");
    while (current_element != NULL) {
        printf("%.1lf ", current_element->tile_p->f_score);
        current_element = current_element->next_p;
    }
    printf("\n");

    printf("Verifying that pathfinding queue elements are in order.\n");
    double current_f, next_f;
    current_element = queue_head.next_p;
    for (int i = 0; i < (test_num - 1); ++i) {
        current_f = current_element->tile_p->f_score;
        next_f = current_element->next_p->tile_p->f_score;
        CHECK_TRUE(current_f <= next_f);
        current_element = current_element->next_p;
    }

    printf("Altering two elements in the queue and then reinserting them.\n");
    tiles[1].f_score = 64 + (rand() % 64);
    insert_existing_queue_element(&tiles[1], queue_head);
    tiles[6].f_score = 64 + (rand() % 64);
    insert_existing_queue_element(&tiles[6], queue_head);

    current_element = queue_head.next_p;
    printf("Queue elements f-scores: \n");
    while (current_element != NULL) {
        printf("%.1lf ", current_element->tile_p->f_score);
        current_element = current_element->next_p;
    }
    printf("\n");

    printf("Verifying that pathfinding queue elements are still in order.\n");
    current_element = queue_head.next_p;
    for (int i = 0; i < (test_num - 1); ++i) {
        current_f = current_element->tile_p->f_score;
        next_f = current_element->next_p->tile_p->f_score;
        CHECK_TRUE(current_f <= next_f);
        current_element = current_element->next_p;
    }

    printf("Freeing memory.\n");
    free_queue(&queue_head);
    printf("\n");
})

TEST_CASE(KDE_test, {
    printf("Kernel Density Estimate function testing.\n");
    printf("Setting up a map with known settings and mine positions.\n");
    hotspot_tuple_t hotspot_pos[3];
    hotspot_pos[0].x_pos = 4; hotspot_pos[0].y_pos = 0;
    hotspot_pos[1].x_pos = 4; hotspot_pos[1].y_pos = 3;
    hotspot_pos[2].x_pos = 1; hotspot_pos[2].y_pos = 3;


    kde_settings_t kde_settings = {5, 5, 1, 3, 0};

    // Creating a 2D array for the value of the KDE values.
    double **kde_map = malloc(sizeof(double *) * kde_settings.x_size);
    for (int y = 0; y < kde_settings.y_size; y++) {
        kde_map[y] = malloc(sizeof(double) * kde_settings.y_size);
    }
    kde_main(kde_map, kde_settings, hotspot_pos);

    // Test cases, verified by hand.
    printf("Checking hand-verified equalities.\n");
    // All hotspots should have the exact same value, which should be 1.0.
    CHECK_EQ_DOUBLE(kde_map[4][0], kde_map[4][3], 1e-6);
    CHECK_EQ_DOUBLE(kde_map[4][0], kde_map[1][3], 1e-6);
    CHECK_EQ_DOUBLE(1.0, kde_map[4][0], 1e-6);

    // Checking a few other tiles which should be pairwise identical.
    CHECK_EQ_DOUBLE(kde_map[4][1], kde_map[2][3], 1e-6);
    CHECK_EQ_DOUBLE(kde_map[4][2], kde_map[3][3], 1e-6);

    // Checking column 1.
    CHECK_EQ_DOUBLE(0.006999, kde_map[0][0], 1e-6);
    CHECK_EQ_DOUBLE(0.022093, kde_map[1][0], 1e-6);
    CHECK_EQ_DOUBLE(0.141982, kde_map[2][0], 1e-6);
    CHECK_EQ_DOUBLE(0.607943, kde_map[3][0], 1e-6);

    printf("Freeing memory used.\n");
    // Frees the memory used for the map of the KDE values.
    for (int y = 0; y < kde_settings.y_size; y++) {
        free(kde_map[y]);
    }
    free(kde_map);
    printf("\n");
})

MAIN_RUN_TESTS(pathfinding_queue_inserting, KDE_test)
