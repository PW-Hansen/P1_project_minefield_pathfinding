#include "evaluation.h"
#include "../settings.h"
#include <stdio.h>

void economic_evaluation(pathfinding_output_t input) {
    double yearly_gain = compute_yearly_gain();
    printf("The expectedly yearly economic gain is %.2lf.\n", yearly_gain);

    double weighted_future_gain = compute_future_gain(yearly_gain);
    printf("After applying discount rates and expected growth, the weighed economic gain is %.2lf.\n", weighted_future_gain);

    double total_cost = input.cost + LOOK_FORWARD_YEARS * input.length * ROAD_MAINTENANCE_COST * TILE_LENGTH_CONVERSION;
    printf("The cost of building and maintaining the road, according to the settings, will be %.2lf.\n", total_cost);

    if (total_cost > weighted_future_gain) {
        printf("\nThe projected costs outweigh the projected economic gains, so the project should not move forward.\n");
    } else {
        printf("\nThe projected economic gains exceed the projected costs, so the project should move forward.\n");
    }
}

/**
 * Naive estimation of the yearly gain connecting two cities by roads would provide.
 */
double compute_yearly_gain() {
    double city_1_gdp;
    double city_2_gdp;
    printf("What is the GDP of city 1?\n> ");
    scanf(" %lf", &city_1_gdp);
    printf("What is the GDP of city 2?\n> ");
    scanf(" %lf", &city_2_gdp);
    return city_1_gdp * city_2_gdp * GDP_SCALE_FACTOR;
}

/**
 * Function to compute the lifetime (within a certain limit, set in settings.h)
 * @param yearly_gain
 * @return
 */
double compute_future_gain(double yearly_gain) {
    double last_valuation = yearly_gain;
    double weighted_future_gain = yearly_gain;
    for (int i = 0; i < LOOK_FORWARD_YEARS; ++i) {
        last_valuation *= DISCOUNT_RATE * EXPECTED_GROWTH;
        weighted_future_gain += last_valuation;
    }

    return weighted_future_gain;
}