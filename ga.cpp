//
// Created by Marcel Beyer on 11.09.2023.
//

#include <cstdio>
#include <random>
#include "ga.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <thread>
#include <vector>

#include "objective_functions.h"

std::random_device shade_rd;

void Ga::write_to_log( const std::string &text ){
    std::ofstream log_file(
            log_file_name, std::ios_base::out | std::ios_base::app );
    log_file << text << std::endl;
}

// constructor for the class Ga
Ga::Ga(int pop_size, int dim, float min_gene, float max_gene, int function_no, bool log) {
    auto now = std::chrono::system_clock::now();
    const std::random_device::result_type seed = shade_rd();
    gen = std::mt19937_64(seed + std::hash<std::thread::id>{}(std::this_thread::get_id()) + std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count());

    this->pop_size = pop_size;
    this->dim = dim;
    this->min_gene = min_gene;
    this->max_gene = max_gene;
    this->function_no = function_no;
    this->log = log;
    std::stringstream ss;
    ss << this;
    this->log_file_name = ss.str() + ".csv";

    // initialize population
    pop = std::vector<std::vector<float>>(pop_size, std::vector<float>(dim));
    fitness = std::vector<float>(pop_size);

    std::uniform_real_distribution<float> dist(Ga::min_gene, Ga::max_gene);
    for (int i = 0; i < pop_size; i++) {
        for (int j = 0; j < dim; j++) {
            pop[i][j] = dist(gen);
            // pop[i][j] = 1; // for testing
        }
    }

    // compute fitness
    compute_fitness();
}

// destructor for the class Ga
Ga::~Ga() {
    if (log) {
        write_to_log(std::to_string(best_fitness));
    }
}

void Ga::compute_fitness() {
    fitness[0] = objective_function_no(pop[0], dim, function_no);
    min_fitness = fitness[0];
    min_fitness_index = 0;
    max_fitness = fitness[0];
    max_fitness_index = 0;

    for (int i = 1; i < pop_size; i++) {
        fitness[i] = objective_function_no(pop[i], dim, function_no);
        if (fitness[i] < min_fitness) {
            min_fitness = fitness[i];
            min_fitness_index = i;
        }
        if (fitness[i] > max_fitness) {
            max_fitness = fitness[i];
            max_fitness_index = i;
        }
    }
    if (min_fitness < best_fitness) {
        best_fitness = min_fitness;
        best_solution = pop[min_fitness_index];
    }

    if (log) {
        write_to_log(std::to_string(min_fitness));
    }
}

std::vector<int> Ga::linear_rank_selection(){
    std::vector<int> indices(pop_size);
    std::iota(indices.begin(), indices.end(), 0);
    std::vector<float> values = fitness;


    std::sort(indices.begin(), indices.end(), [&values](size_t i1, size_t i2) {return values[i1] < values[i2];});

    // create list of indices of individuals to be mated
    std::uniform_real_distribution<float> dist(0, (float)((pop_size + 1) * pop_size) / 2);
    std::vector<int> mating_list(pop_size);
    for (int i = 0; i < pop_size; i++) {
        float r = dist(gen);

        int current_sum = 0;
        for (int j = 0; j < pop_size; j++) {
            current_sum += pop_size - j;
            if ((float)current_sum >= r) {
                mating_list[i] = indices[j];
                break;
            }
        }

    }
    return mating_list;
}

std::vector<std::vector<float>> Ga::uniform_crossover(const std::vector<int>& mating_list) {
    std::uniform_real_distribution<float> dist(0, 1);

    std::vector<std::vector<float>> trial_population(pop_size, std::vector<float>(dim));

    for (int i = 0; i < pop_size/2; i++) {
        for (int j = 0; j < dim; j++) {
            if (dist(gen) < 0.5) {
                trial_population[2*i][j] = pop[mating_list[2*i]][j];
                trial_population[2*i+1][j] = pop[mating_list[2*i+1]][j];
            } else {
                trial_population[2*i][j] = pop[mating_list[2*i+1]][j];
                trial_population[2*i+1][j] = pop[mating_list[2*i]][j];
            }
        }
    }
    return trial_population;
}

void Ga::non_uniform_mutation(){
std::uniform_real_distribution<float> uni_dist(0, 1);
    for (int i = 0; i < pop_size; i++) {
        for (int j = 0; j < dim; j++) {
            if (uni_dist(gen) < 0.001) {    // mutation probability
                std::uniform_real_distribution<float> normal_dist(pop[i][j], (max_gene-min_gene)/6);   // mutation step size
                pop[i][j] = normal_dist(gen);
                if (pop[i][j] < min_gene) {
                    pop[i][j] = min_gene;
                } else if (pop[i][j] > max_gene) {
                    pop[i][j] = max_gene;
                }
            }
        }
    }
}

void Ga::evolve(int generations) {
    for(int i = 0; i < generations; i++) {
        std::vector<int> mating_list = linear_rank_selection();
        std::vector<std::vector<float>> trial_population = uniform_crossover(mating_list);
        pop = trial_population;
        non_uniform_mutation();
        compute_fitness();
    }
}
