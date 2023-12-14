//
// Created by Peter on 13/12/2023.
//

#ifndef P1_PROJECT_EVALUATION_H
#define P1_PROJECT_EVALUATION_H

void economic_evaluation(double road_cost, double road_length, double city_1_gdp, double city_2_gdp);

double compute_yearly_gain(double city_1_gdp, double city_2_gdp);

double compute_future_gain(double yearly_gain);

#endif //P1_PROJECT_EVALUATION_H
