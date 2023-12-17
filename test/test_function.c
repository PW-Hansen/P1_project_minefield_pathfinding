#include "mtest.h"
#include "../src/pathfinding/pathfinding.h"
#include <stdlib.h>


// These are currently just dummy tests with dummy names.
// Feel free to remove them as tests get added.
TEST_CASE(quotient_of_function, {
  CHECK_TRUE(1);
})

TEST_CASE(modulus_of_function, {
  CHECK_TRUE(1);
})

TEST_CASE(calculate_fibonacci, {
  CHECK_TRUE(1);
})

TEST_CASE(calculate_gcd, {
  CHECK_TRUE(1);
})

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
})

MAIN_RUN_TESTS(quotient_of_function,
               modulus_of_function,
               calculate_fibonacci,
               calculate_gcd,
               pathfinding_queue_inserting)
