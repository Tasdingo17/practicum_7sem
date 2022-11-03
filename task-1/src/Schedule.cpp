#ifndef __Schedule__
#define __Schedule__

#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <exception>
#include <random>


template<class Ttask>
class Solution{
public:
    virtual long long get_criterion_val() const = 0;
    virtual ~Solution() {};

    virtual std::vector<std::vector<Ttask>> get_data() const{
        return data;
    }

    int get_proc_number() const{
        return nProc;
    }

    int get_task_number() const{
        return nTask;
    }
protected:
    std::vector<std::vector<Ttask>> data;
    unsigned int nProc;
    unsigned int nTask;
};

class Task{
public:
    Task(int _number, int _duration): duration(_duration), number(_number) {};
    
    int get_duration() const{
        return duration;
    }

    int get_number() const{
        return number;
    }
private:
    int duration;
    int number;
};


class Schedule: public Solution<Task>{
public:
    Schedule(const char* filename, bool randomize = false){
        // read from csv file, format described in src/Generator.cpp
        std::ifstream file_data = _open_file_safe(filename);

        try{
            parse_nProc(file_data);
            parse_nTasks(file_data);

            for (unsigned int i = 0; i < nProc; i++) { 
                data.emplace_back(std::vector<Task>()); 
            }
            parse_tasks(file_data, randomize);
        } catch(...){
            file_data.close();
            throw;
        }

        file_data.close();
    }

    void print_data(bool by_number = false) const{
        int i = 1;
        for (const auto& tmp_proc : data){
            std::cout << "proc " << i << ": ";
            if (by_number){
                for (const auto& tmp_task: tmp_proc){
                        std::cout << tmp_task.get_number() << ", ";
                }
            } else {
                for (const auto& tmp_task: tmp_proc){
                    std::cout << tmp_task.get_duration() << ", ";
                }
            }
            std::cout << std::endl;
            i++;
        }
    }

    // on one processor
    long long get_proc_total_dur(unsigned int proc) const{
        unsigned long long res = 0;
        unsigned long long tmp_time;
        for (auto &task : data[proc]) {
            res += tmp_time + task.get_duration();
            tmp_time += task.get_duration();
        }
        return res;
    }

    std::vector<std::vector<Task>>& get_data(){
        return data;
    }

    // Criterion 2: total waiting time (sum completion time by all tasks)
    long long get_criterion_val() const override{
        unsigned long long res = 0;
        unsigned long long tmp_time;
        for (auto &proc : data) {
            tmp_time = 0;
            for (auto &task : proc) {
                res += tmp_time + task.get_duration();
                tmp_time += task.get_duration();
            }
        }
        return res;
    }

private:
    std::ifstream _open_file_safe(const char* filename) const{
        std::ifstream file_data;
        file_data.open(filename);
        if (!file_data.is_open()){
            throw std::runtime_error("Failed to open file");
        }
        return file_data;
    }

    void parse_nProc(std::ifstream& file_data){
        std::string tmp_line, tmp_word;
        std::getline(file_data, tmp_line);
        std::stringstream line(tmp_line);
        
        getline(line, tmp_word, ',');
        if (tmp_word != "Processors"){
            throw std::runtime_error("Parsing error: expected 'Processors' in the first line");
        }

        line >> nProc;
        if (!nProc){
            throw std::runtime_error("Value error: nProc must be possitive");
        }
    }

    void parse_nTasks(std::ifstream& file_data){
        std::string tmp_line, tmp_word;
        std::getline(file_data, tmp_line);
        std::stringstream line(tmp_line);
        
        getline(line, tmp_word, ',');
        if (tmp_word != "Tasks"){
            throw std::runtime_error("Parsing error: expected 'Tasks' in the second line");
        }

        line >> nTask;
    }

    void parse_tasks(std::ifstream& file_data, bool randomize = false){
        std::string tmp_line, tmp_word;

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> proc_dist(0, nProc - 1);

        int n, d;
        for (unsigned int i = 1; i <= nTask; i++){
            std::getline(file_data, tmp_line);
            std::stringstream line(tmp_line);

            getline(line, tmp_word, ',');
            n = std::stoi(tmp_word);
            line >> d;

            if (randomize){
                //int proc_n = rand() % nProc;
                int proc_n = proc_dist(gen);
                data[proc_n].emplace_back(Task(n, d));
            } else {
                data[0].emplace_back(Task(n, d));
            }
        }
    }
};

#endif  // __Schedule__