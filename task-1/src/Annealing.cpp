#ifndef __Annealing__
#define __Annealing__

#include "Temperature.cpp"
#include "Schedule.cpp"
#include "Mutation.cpp"
#include <cmath>

#define MAX_NON_BEST_ITER 100
#define ITER_FOR_COOL_DOWN 5

template <class Ttask>
class Annealing{
public:
    Annealing(Solution<Ttask>& start_schedule, Temperature& _temp, Mutation<Ttask>& _mutator,
              unsigned int _iters_cool_down = ITER_FOR_COOL_DOWN):
        curr_schedule(start_schedule.clone()),
        best_schedule(start_schedule.clone()),
        temperature(_temp), 
        mutator(_mutator.clone()), 
        iters_cool_down(_iters_cool_down) {};

    void run_algorithm(){
        int stable_iter = 0, total_iter = 0;
        unsigned long long curr_metric_val = curr_schedule->get_criterion_val();
        unsigned long long best_metric_val = curr_metric_val;
        std::shared_ptr<Solution<Ttask>> new_schedule;

        while (stable_iter < MAX_NON_BEST_ITER){
            stable_iter++;
            for (unsigned int i = 0; i < iters_cool_down; i++){

                new_schedule = mutator->mutate(*curr_schedule);

                unsigned long long new_metric_val = new_schedule->get_criterion_val();
                if (new_metric_val < best_metric_val){
                    stable_iter = 0;
                    best_metric_val = new_metric_val;
                    best_schedule = new_schedule;
                }
                
                if (new_metric_val <= curr_metric_val || is_change(new_metric_val - curr_metric_val)){
                    curr_metric_val = new_metric_val;
                    curr_schedule = new_schedule;
                }
            }

            temperature.step(total_iter);
            total_iter++;
        }
        std::cout << "here" << std::endl;
    }

    Solution<Ttask>* get_best_schedule(){
        return best_schedule.get();
    }
private:
    std::shared_ptr<Solution<Ttask>> curr_schedule;
    std::shared_ptr<Solution<Ttask>> best_schedule;
    Temperature temperature;
    std::shared_ptr<Mutation<Ttask>> mutator;
    unsigned int iters_cool_down;

    bool is_change(long long delta){
        double prob = exp(- static_cast<double>(delta) / temperature.get_curr_temp());
        double chance = static_cast<double> (rand() % 10000) / 10000.0;
        return chance < prob;
    }
};


#endif // __Annealing__