#ifndef __Selection__
#define __Selection__

#include <memory>
#include <bitset>
#include <array>
#include <numeric>

template<size_t N> 
using array_indices = std::array<int, N>;

template<size_t popSize>
class Selection{
public:
    virtual ~Selection() {};
    virtual std::unique_ptr<Selection<popSize>> clone() const = 0;
    virtual array_indices<popSize> select(const std::array<int, popSize>& surv_func_vals) = 0;
};

//#include <iostream>

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
        std::array<double, popSize> probs = std::move(vals_to_probs(surv_func_vals));
        double tmp_start = 0.0;
        double sum_of_probs = std::accumulate(probs.begin(), probs.end(), tmp_start);

        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_real_distribution<> selection_prob_dist(0, sum_of_probs);

        for (unsigned i = 0; i < popSize; i++){
            double tmp_val = selection_prob_dist(rng);
            unsigned idx = 0;
            for (idx = 0; idx < popSize; idx++){
                tmp_val -= probs[idx];
                if (tmp_val <= 0){
                    break;   
                }
            }
            res[i] = idx;
        }

        return res;
    }
private:
    std::array<double, popSize> vals_to_probs(const std::array<int, popSize>& surv_func_vals){
        int total_sum = std::accumulate(surv_func_vals.begin(), surv_func_vals.end(), 0);
        std::array<double, popSize> res;
        for (unsigned i = 0; i < popSize; i++){
            res[i] = 1 - 1.0 * surv_func_vals[i] / total_sum ;     // the lower res the higher prob
        }
        return res;
    }
};

#endif // __Selection__