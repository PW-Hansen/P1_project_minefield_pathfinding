#include "../pathfinding/pathfinding.h"

#ifndef P1_PROJECT_EVALUATION_H
#define P1_PROJECT_EVALUATION_H

void economic_evaluation(pathfinding_output_t input);

double compute_yearly_gain();

double compute_future_gain(double yearly_gain);

#endif //P1_PROJECT_EVALUATION_H
