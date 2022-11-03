#ifndef __Mutation__
#define __Mutation__

#include "Schedule.cpp"
#include <random>

template<class Ttask>
class Mutation {
public:
    virtual void mutate(Solution<Ttask>& schedule) = 0;
    virtual ~Mutation() {};
};


class Mutator: public Mutation<Task>{
public:
    /* repeat (nTask / 4) times:
    *    proc1 = rand_proc(), proc2 = rand_proc()
    *   for i % 3 == 0:  
    *     task1 = rand_task(proc1), task2 = rand_task(proc2)
    *     swap(task1, task2)
    *   for i %% 3 != 0:
    *     task1 = rand_task(proc1)
    *     move(task1, proc2)
    * 
    * if (proc1.size == 0 && proc2.size != 0) 
    *   proc2.back => proc1.back
    */
    void mutate(Solution<Task>& _schedule) override{
        Schedule& schedule = static_cast<Schedule&>(_schedule);
        std::vector<std::vector<Task>>& data = schedule.get_data(); 

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> proc_distr(0, schedule.get_proc_number() - 1);

        for (int i = 0; i < (schedule.get_task_number() / 4); i ++){
            int proc1 = proc_distr(gen), proc2 = proc_distr(gen);
            int size1 = data[proc1].size(), size2 = data[proc2].size();
            if (size1 == 0 && size2 == 0){
                continue;
            }

            std::uniform_int_distribution<> work_distr1(0, size1 - 1);
            std::uniform_int_distribution<> work_distr2(0, size2 - 1);            

            if (i % 3 == 0){
                if (size1 != 0){
                    Task tmp = pop_task(data[proc1], work_distr1(gen));
                    data[proc2].push_back(tmp);
                } else {
                    Task tmp = pop_task(data[proc2], work_distr2(gen));
                    data[proc1].push_back(tmp);
                }
            } else {
                if (size1 == 0 && size2 != 0){
                    Task tmp = data[proc2].back();
                    data[proc2].pop_back();
                    data[proc1].push_back(tmp);
                } else if (size1 != 0 && size2 == 0){
                    Task tmp = data[proc1].back();
                    data[proc1].pop_back();
                    data[proc2].push_back(tmp);
                } else {
                    int task1 = work_distr1(gen), task2 = work_distr2(gen);
                    std::swap(data[proc1][task1], data[proc2][task2]);
                }
            }
        }
    }
private:
    Task pop_task(std::vector<Task>& proc_tasks, int idx){
        auto iter = proc_tasks.begin() + idx;
        Task tmp = *iter;
        proc_tasks.erase(iter);
        return tmp;
    }
};

#endif  // __Mutation__