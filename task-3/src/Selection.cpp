#ifndef __Selection__
#define __Selection__

#include <memory>
#include <bitset>
#include <array>
#include <numeric>
#include <algorithm>
#include <random>
#include "Sizes.h"

template<size_t N> 
using array_indices = std::array<int, N>;

template<size_t popSize>
class Selection{
public:
    virtual ~Selection() {};
    virtual std::unique_ptr<Selection<popSize>> clone() const = 0;
    virtual array_indices<popSize> select(const std::array<int, popSize>& surv_func_vals) = 0;
};


/*Roulette selection*/
template <size_t popSize>
class RouletteSelection : public Selection<popSize>{
public:
    virtual std::unique_ptr<Selection<popSize>> clone() const override{
        return std::make_unique<RouletteSelection<popSize>>(*this);
    }

    /*1) Find probabilities
    * 2) Pick a uniformly random number P between [0, sum_of_probs]
    * 3) Go through probs and substract each element until P <= 0;
    * 4) Current index is the answer (or current index - 1, depending on implementation)
    */
    virtual array_indices<popSize> select(const std::array<int, popSize>& surv_func_vals) override{
        array_indices<popSize> res;
        std::array<int, popSize> minimize_vals = std::move(vals_for_minimize(surv_func_vals));
        int tmp_start = 0;
        int sum_of_vals = std::accumulate(minimize_vals.begin(), minimize_vals.end(), tmp_start);

        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<> selection_val_dist(0, sum_of_vals);

        for (unsigned i = 0; i < popSize; i++){
            int tmp_val = selection_val_dist(rng);
            unsigned idx = 0;
            for (idx = 0; idx < popSize; idx++){
                tmp_val -= minimize_vals[idx];
                if (tmp_val <= 0){
                    break;   
                }
            }
            res[i] = idx;
        }

        return res;
    }
private:
    std::array<int, popSize> vals_for_minimize(const std::array<int, popSize>& surv_func_vals){
        int max_val = *std::max_element(surv_func_vals.begin(), surv_func_vals.end()) + 1;
        std::array<int, popSize> res;
        for (unsigned i = 0; i < popSize; i++){
            res[i] = max_val - surv_func_vals[i];
            res[i] = (res[i] > 0) ? res[i] : 0;
        }
        return res;
    }
};

#endif // __Selection__