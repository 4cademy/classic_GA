#include <iostream>
#include"ga.h"

int main(int argc, char* argv[]) {
    if (argc != 7) {
        std::cout << "Usage: <population size> <dimensionality> <min> <max> <function number> <max evaluations>" << std::endl;
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
    /*
    std::cout << "MAIN" << std::endl;
    std::cout << "POPSIZE: " << POPSIZE << std::endl;
    std::cout << "DIM: " << DIM << std::endl;
    std::cout << "MIN: " << MIN << std::endl;
    std::cout << "MAX: " << MAX << std::endl;
    std::cout << "FUNCTION_NO: " << FUNCTION_NO << std::endl;
    std::cout << "FUNCTION_EVALS: " << FUNCTION_EVALS << std::endl;
    */

    Ga *ga0 = new Ga(POPSIZE, DIM, MIN, MAX, FUNCTION_NO);
    ga0->evolve(FUNCTION_EVALS);
    ga0->~Ga();
    return 0;
}
