#include "../Task3.h"
#include <chrono>
#include <ctime>
#include <iostream>
#include <fstream>
#include <cmath>
#include <string>

#define N_SERIES 10
#define ITERS_PER_SERIES 10

int main() {
    std::ofstream file_data;
    file_data.open("genetic_algo_research.csv");
    if (!file_data.is_open()){
        throw std::runtime_error("Failed to open file");
    }
    file_data << "series,iter,mut_prob,time_sec,best_criterion\n";
    double init_mut_prob = 1.0 / (50 * 50);
    GoLEngine game;
    for (int i = 0; i < N_SERIES; i++) {
        double mut_prob = init_mut_prob * pow(1.5, i);
        std::cout << "mut_prob: " << mut_prob << std::endl;
        RouletteSelection<POPULATION_SIZE> sel;
        GoLSurvaivalFunc<WIDTH*HEIGHT> surv_func;
        Crosser<WIDTH * HEIGHT> cros;
        Mutator<WIDTH * HEIGHT> mut(mut_prob);
        for (int j = 0; j < ITERS_PER_SERIES; j++) {
            GeneticAlgo<WIDTH * HEIGHT> gen_algo(sel, mut, cros, surv_func);
            auto t_start = std::chrono::high_resolution_clock::now();
            gen_algo.run_algorithm();
            auto t_end = std::chrono::high_resolution_clock::now();
            auto t_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(t_end - t_start).count();

            auto best_individ = gen_algo.get_best_individ();
            auto best_criterion = gen_algo.get_best_criterion();
            std::string filename1 = std::string("series_") + std::to_string(i) +
                                    "_run_" + std::to_string(j) + "_sol.txt";
            std::string filename2 = std::string("series_") + std::to_string(i) +
                                    "_run_" + std::to_string(j) + "_sol_after100.txt";
            GoLEngine::individ_to_file(best_individ, filename1.c_str());
            auto best_after100 = game.get_great_descedant(best_individ, 100);
            GoLEngine::individ_to_file(best_after100, filename2.c_str());

            file_data << i << "," << j << "," << mut_prob << "," << t_duration / 1e9 << ","
                      << best_criterion << "\n";
            file_data.flush();
        }
    }
    file_data.close();
    return 0;
}