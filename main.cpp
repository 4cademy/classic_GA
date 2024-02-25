#include <iostream>
#include <fstream>
#include <algorithm>
#include"ga.h"
#include "omp.h"

void move_best_percent(Ga* from, Ga* to, float percentage) {
    int pop_size = from->pop_size;
    int best_amount = (int)ceilf((float)(pop_size) * percentage);

    std::vector<int> indices1(pop_size);
    std::iota(indices1.begin(), indices1.end(), 0);
    std::vector<float> values1 = from->fitness;
    std::sort(indices1.begin(), indices1.end(), [&values1](size_t i1, size_t i2) {return values1[i1] < values1[i2];});

    std::vector<int> indices2(pop_size);
    std::iota(indices2.begin(), indices2.end(), 0);
    std::vector<float> values2 = to->fitness;
    std::sort(indices2.begin(), indices2.end(), [&values2](size_t i1, size_t i2) {return values2[i1] < values2[i2];});

    for (int i = 0; i < best_amount; i++) {
        to->pop[indices2[i]] = from->pop[indices1[pop_size - 1 - i]];
    }
}

int main(int argc, char* argv[]) {
    if (argc != 9) {
        std::cout << "Usage: <population size> <no_subpops> <dimensionality> <min> <max> <function number> <max evaluations> <no_sequences>" << std::endl;
        return 1;
    }

    if (std::stoi(argv[1]) % std::stoi(argv[2]) != 0) {
        std::cout << "Population size must be divisible by number of sub-populations " << std::endl;
        return 1;
    }
    if (std::stoi(argv[8]) < 1) {
        std::cout << "Number of sequences must be at least 1" << std::endl;
        return 1;
    }
    if (std::stoi(argv[7]) % std::stoi(argv[8]) != 0){
        std::cout << "Number of function evaluations must be divisible by number of sequences" << std::endl;
        return 1;
    }

    const int POPSIZE = std::stoi(argv[1]);  // Population size
    const int NO_SUBPOPS = std::stoi(argv[2]);     // Number of subpopulations
    const int DIM = std::stoi(argv[3]);  // Dimensionality of the problem
    const float MIN = std::stof(argv[4]);  // Minimum value for each gene
    const float MAX = std::stof(argv[5]);  // Maximum value for each gene
    const int FUNCTION_NO = std::stoi(argv[6]);  // Number of the objective function to be optimized
    const int FUNCTION_EVALS = std::stoi(argv[7]);  // Maximum number of function evaluations
    const int NO_SEQUENCES = std::stoi(argv[8]);   // Number of sequences 1 = no exchange, 2 = 1 exchange, 4 = 3 exchanges, etc.

    std::cout << "POPSIZE: " << POPSIZE << std::endl;
    std::cout << "NO_SUBPOPS: " << NO_SUBPOPS << std::endl;
    std::cout << "DIM: " << DIM << std::endl;
    std::cout << "MIN: " << MIN << std::endl;
    std::cout << "MAX: " << MAX << std::endl;
    std::cout << "FUNCTION_NO: " << FUNCTION_NO << std::endl;
    std::cout << "FUNCTION_EVALS: " << FUNCTION_EVALS << std::endl;
    std::cout << "NO_SEQUENCES: " << NO_SEQUENCES << std::endl;

    int subpop_size = POPSIZE / NO_SUBPOPS;

    std::vector<Ga*> ga_vector;
    for (int i = 0; i < NO_SUBPOPS; i++) {
        ga_vector.push_back(new Ga(subpop_size, DIM, MIN, MAX, FUNCTION_NO, true));
    }

    float best_percentage = 0.1;
    for (int i = 0; i < NO_SEQUENCES; i++) {
#pragma omp parallel for default(none) shared(ga_vector, best_percentage, NO_SUBPOPS, FUNCTION_EVALS, NO_SEQUENCES)
        for (int j = 0; j < NO_SUBPOPS; j++) {
            ga_vector[j]->evolve(FUNCTION_EVALS / NO_SEQUENCES);
        }

        for (int j = 0; j < NO_SUBPOPS; j++) {
            move_best_percent(ga_vector[j], ga_vector[(j+1)%NO_SUBPOPS], best_percentage);
        }
    }

    for (int i = 0; i < NO_SUBPOPS; i++) {
        ga_vector[i]->~Ga();
    }
    return 0;
}
