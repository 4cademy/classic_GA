#include <iostream>
#include <fstream>
#include <algorithm>
#include"ga.h"

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
    if (argc != 8) {
        std::cout << "Usage: <population size> <dimensionality> <min> <max> <function number> <max evaluations> <no_sequences>" << std::endl;
        return 1;
    }

    if (std::stoi(argv[1]) % 2 != 0) {
        std::cout << "Population size must be even" << std::endl;
        return 1;
    }

    const int POPSIZE = std::stoi(argv[1]);  // Population size
    const int DIM = std::stoi(argv[2]);  // Dimensionality of the problem
    const float MIN = std::stof(argv[3]);  // Minimum value for each gene
    const float MAX = std::stof(argv[4]);  // Maximum value for each gene
    const int FUNCTION_NO = std::stoi(argv[5]);  // Number of the objective function to be optimized
    const int FUNCTION_EVALS = std::stoi(argv[6]);  // Maximum number of function evaluations
    const int NO_SEQUENCES = std::stoi(argv[7]);   // Number of sequences 1 = no exchange, 2 = 1 exchange, 4 = 3 exchanges, etc.

    std::cout << "MAIN" << std::endl;
    std::cout << "POPSIZE: " << POPSIZE << std::endl;
    std::cout << "DIM: " << DIM << std::endl;
    std::cout << "MIN: " << MIN << std::endl;
    std::cout << "MAX: " << MAX << std::endl;
    std::cout << "FUNCTION_NO: " << FUNCTION_NO << std::endl;
    std::cout << "FUNCTION_EVALS: " << FUNCTION_EVALS << std::endl;
    std::cout << "NO_SEQUENCES: " << NO_SEQUENCES << std::endl;


    int subpop_size = 512;

    Ga *ga0 = new Ga(subpop_size, DIM, MIN, MAX, FUNCTION_NO, true);
    Ga *ga1 = new Ga(subpop_size, DIM, MIN, MAX, FUNCTION_NO, true);

    float best_percentage = 0.1;
    for (int i = 0; i < NO_SEQUENCES; i++) {
        ga0->evolve(FUNCTION_EVALS/NO_SEQUENCES);
        ga1->evolve(FUNCTION_EVALS/NO_SEQUENCES);

        move_best_percent(ga0, ga1, best_percentage);
        move_best_percent(ga1, ga0, best_percentage);
    }

    ga0->~Ga();
    ga1->~Ga();
    return 0;
}
