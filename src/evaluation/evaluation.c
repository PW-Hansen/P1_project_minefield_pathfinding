#include "evaluation.h"
#include "../settings.h"
#include <math.h>
#include <stdio.h>

void economic_evaluation(double road_cost, double road_length, double city_1_gdp, double city_2_gdp) {
    double yearly_gain = compute_yearly_gain(city_1_gdp, city_2_gdp);
    printf("The expectedly yearly economic gain is %.2lf.\n", yearly_gain);

    double weighted_future_gain = compute_future_gain(yearly_gain);
    printf("After applying discount rates and expected growth, the weighed economic gain is %.2lf.\n", weighted_future_gain);

    double total_cost = road_cost + LOOK_FORWARD_YEARS * road_length * ROAD_MAINTENANCE_COST;
    printf("The cost of building and maintaining the road, according to the settings, will be %.2lf.\n", total_cost);

    if (total_cost > weighted_future_gain) {
        printf("\nThe projected costs outweigh the projected economic gains, so the project should not move forward.\n");
    } else {
        printf("\nThe projected economic gains exceed the projected costs, so the project should move forward.\n");
    }
}

/**
 * Naive estimation of the yearly gain connecting two cities by roads would provide.
 * @param city_1_gdp GDP of one city
 * @param city_2_gdp GFP of the other city
 * @return
 */
double compute_yearly_gain(double city_1_gdp, double city_2_gdp) {
    return city_1_gdp * city_2_gdp;
}

/**
 * Function to compute the lifetime (within a certain limit, set in settings.h)
 * @param yearly_gain
 * @return
 */
double compute_future_gain(double yearly_gain) {
    double weighted_future_gain = 0;
    double last_valuation = yearly_gain;
    for (int i = 0; i < LOOK_FORWARD_YEARS; ++i) {
        last_valuation *= DISCOUNT_RATE * EXPECTED_GROWTH;
        weighted_future_gain += last_valuation;
    }

    return weighted_future_gain;
}