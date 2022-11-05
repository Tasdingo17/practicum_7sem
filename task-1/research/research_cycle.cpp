#include "../src/Generator.cpp"
#include "../src/Annealing.cpp"
#include <chrono>
#include <ctime>
#include <iostream>
#include <fstream>

#define ITERS_FOR_AVARAGE 2
#define START_TEMP 500

int main() {
    //std::vector<unsigned int> processors = {2, 4, 6, 8, 16, 24, 32, 64, 128};
    //std::vector<unsigned int> works = {100, 200, 400, 750, 1250, 1500, 2500, 5000, 7500, 10000, 15000, 20000, 25000};
    std::vector<unsigned int> processors = {2, 4, 6};
    std::vector<unsigned int> works = {25000, 30000};
    std::vector<Temperature_law_type> laws = {Temperature_law_type::CAUCHY, 
                                              Temperature_law_type::BOLTZMANN,
                                              Temperature_law_type::LOG};

    std::ofstream file_data;
    file_data.open("sus_res_time.csv");
    if (!file_data.is_open()){
        throw std::runtime_error("Failed to open file");
    }

    for (auto &proc_n: processors) {
        for (auto &work_n: works) {
            //std::cout << "Processes: " << proc_n << " Works: " << work_n << std::endl;
            for (int it = 0; it < ITERS_FOR_AVARAGE; it++) {   // 5 times for same vals
                for (auto &temp_law: laws){
                    std::cout << "Processes: " << proc_n << " Works: " << work_n 
                              << " Law: " << (int) temp_law << std::endl;
                    srand (0);  // fix for research stability
                    InputGenerator gen(proc_n, work_n, 5, 20); 
                    gen.generate_input_data("test_input.csv");

                    Schedule s("test_input.csv", false);
                    Temperature temperature(START_TEMP, temp_law);
                    Mutator mut;

                    auto t_start = std::chrono::high_resolution_clock::now();

                    Annealing<Task> annealing(s, temperature, mut);
                    annealing.run_algorithm();

                    auto t_end = std::chrono::high_resolution_clock::now();
                    auto t_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(t_end - t_start).count();

                    file_data << "processors:" << proc_n << ",works:" << work_n << ",duration_sec:" << t_duration / 1e9
                            << ",startCriterion:" << s.get_criterion_val() 
                            << ",bestCriterion:" << annealing.get_best_criterion() 
                            << ",law:" << (int) temp_law << "\n";
                }
            }
        }
    }
    file_data.close();
    return 0;
}