#ifndef __GenerAlgo__
#define __GenerAlgo__

#include <cmath>
#include <memory>
#include <array>
#include <algorithm>
#include "Sizes.h"
#include "Mutation.cpp"
#include "Selection.cpp"
#include "SurvaivalFunc.cpp"
#include "Crossing.cpp"

#define MAX_NON_BEST_ITER 50

#include <iostream>

template <size_t Ngens>
class GenerationAlgo{
public:
    GenerationAlgo(Selection<POPULATION_SIZE>& _selector, Mutation<Ngens>& _mutator,
                   Crossing<Ngens>& _crosser, SurvaivalFunc<Ngens>& _surv_func):
    selector(_selector.clone()), mutator(_mutator.clone()),
    crosser(_crosser.clone()), surv_func(_surv_func.clone()),
    best_res(Ngens) {};

    void run_algorithm(){
        int stable_iter = 0;
        init_population();
        best_individ = population[0];
        std::array<int, POPULATION_SIZE> survaival_vals;
        
        while (stable_iter < MAX_NON_BEST_ITER){
            stable_iter++;
            //std::cout << "stable_iter: " << stable_iter << std::endl;
            survaival_vals = std::move(population_survaival_func());
            //survaival_vals = population_survaival_func();

            if (is_new_best(survaival_vals)){
                stable_iter = 0;
            }

            auto selected_individs = std::move(population_selection(survaival_vals));
            population = std::move(population_crossing(selected_individs));
            population_mutation();
        }
        //std::cout << "Done!" << best_individ << " " << best_res << std::endl;
    }

    std::bitset<Ngens> get_best_individ() const{
        return best_individ;
    }

    int get_best_criterion() const{
        return best_res;
    }

private:
    std::shared_ptr<Selection<POPULATION_SIZE>> selector;
    std::shared_ptr<Mutation<Ngens>> mutator;
    std::shared_ptr<Crossing<Ngens>> crosser;
    std::shared_ptr<SurvaivalFunc<Ngens>> surv_func;
    std::array<std::bitset<Ngens>, POPULATION_SIZE> population;
    
    std::bitset<Ngens> best_individ;
    int best_res;

    bool is_change(long long delta){
        double chance = static_cast<double> (rand() % 10000) / 10000.0;
        return chance < delta;
    }

    void init_population(){
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<> dist(0, 1); // distribution in discrete range [0, 1]
        std::bitset<Ngens> tmp_individ;
        for (unsigned i = 0; i < POPULATION_SIZE; i++){
            for (unsigned j = 0; j < Ngens; j++){
                tmp_individ[j] = dist(rng);
            }
            population[i] = tmp_individ;
        }
        //std::cout << "Inited population" << std::endl;
    }

    std::array<int, POPULATION_SIZE> 
    population_survaival_func(){
        std::array<int, POPULATION_SIZE> res;
        for (unsigned i = 0; i < POPULATION_SIZE; i++){
            res[i] = surv_func->get_criterion_val(population[i]);
            //std::cout << "got criterion for i: " << i << std::endl; 
        }
        return res;
    }

    std::array<std::bitset<Ngens>, POPULATION_SIZE>
    population_selection(const std::array<int, POPULATION_SIZE>& surv_func_vals){
        array_indices<POPULATION_SIZE> indices = selector->select(surv_func_vals);
        std::array<std::bitset<Ngens>, POPULATION_SIZE> selected_individs;
        for (unsigned i = 0; i < POPULATION_SIZE; i++){
            selected_individs[i] = population[indices[i]];
        }
        //std::cout << "Selected population!" << std::endl;
        return selected_individs;
    }

    std::array<std::bitset<Ngens>, POPULATION_SIZE> 
    population_crossing(const std::array<std::bitset<Ngens>, POPULATION_SIZE>& selected_individs){
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<> ind_dist(0, POPULATION_SIZE - 1); // ?
        std::uniform_real_distribution<> cross_prob_dist(0, 1);
        
        int i, j, k = 0;
        decltype(population) new_population;
        double cross_prob = crosser->get_crossing_prob();
        while (k < POPULATION_SIZE - 1){    // as step is 2
            //std::cout << "here1" << std::endl;
            do{
            //    std::cout << "here2" << std::endl;
                i = ind_dist(rng);
                j = ind_dist(rng);
            } while (i == j);
            if (cross_prob_dist(rng) < cross_prob){
                auto descedants = crosser->cross(selected_individs[i], selected_individs[j]);
                new_population[k] = std::move(descedants.first);
                new_population[k+1] = std::move(descedants.second); 
                k += 2;
            } else {
                //std::cout << "cross_prob_dist: " << cross_prob_dist(rng) << std::endl;
            }
        }

        //std::cout << "Crossed population!" << std::endl;
        return new_population;
    }

    void population_mutation(){
        for(unsigned i = 0; i < POPULATION_SIZE; i++){
            population[i] = mutator->mutate(population[i]);
        }
        //std::cout << "Mutated population!" << std::endl;
    }

    // true if best changed, false else
    inline bool is_new_best(const std::array<int, POPULATION_SIZE>& surv_func_vals){
        auto argmin_iter = std::min_element(surv_func_vals.begin(), surv_func_vals.end());
        int argmin = argmin_iter - surv_func_vals.begin();
        if( *argmin_iter < best_res){
            best_individ = population[argmin];
            best_res = surv_func_vals[argmin];
            return true;
        } else {
            return false;
        }
    }
};

#endif  // __GenerAlgo__