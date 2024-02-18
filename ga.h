//
// Created by Marcel Beyer on 11.09.2023.
//

#include <vector>
#include <cmath>

#ifndef GENETIC_ALGORITHM_VLSI_V2_GA_H
#define GENETIC_ALGORITHM_VLSI_V2_GA_H


class Ga {
public:
    Ga()= delete;
    Ga(int pop_size, int dim, float min_gene, float max_gene, int function_no);
    Ga(const Ga& ga) = delete;
    Ga(Ga&& ga) = delete;
    ~Ga() = default;
    void evolve(int generations);

    int pop_size{};
    int dim{};
    float min_gene{};  // min value for each gene
    float max_gene{};  // max value for each gene
    int function_no{};
    std::vector<std::vector<float>> pop{};
    std::vector<float> fitness{};
    float min_fitness{};
    int min_fitness_index{};
    float max_fitness{};
    int max_fitness_index{};
    float best_fitness = HUGE_VALF;
    std::vector<float> best_solution{};

private:
    void compute_fitness();
    std::vector<int> linear_rank_selection();
    std::vector<std::vector<float>> uniform_crossover(const std::vector<int>& mating_list);
    void non_uniform_mutation();
};


#endif //GENETIC_ALGORITHM_VLSI_V2_GA_H
