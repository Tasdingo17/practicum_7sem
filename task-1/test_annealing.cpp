#include <iostream>
#include <ctime>
#include "Task1.h"


int main(){
    srand (1);
    
    int start_temp = rand() % 1000 + 1000;
    Temperature t1(start_temp, Temperature_law_type::CAUCHY);

    InputGenerator gen(3, 50, 4, 10); 
    gen.generate_input_data("test_input.csv");
    Schedule s("test_input.csv", true);
    s.print_data();
    std::cout << s.get_criterion_val() << std::endl;

    Mutator mut;

    Annealing<Task> an(s, t1, mut);
    an.run_algorithm();
    Solution<Task>* best = an.get_best_schedule();
    Schedule* best_sch_ptr = static_cast<Schedule*>(best);
    best_sch_ptr->print_data();
    std::cout << best_sch_ptr->get_criterion_val() << std::endl;
}