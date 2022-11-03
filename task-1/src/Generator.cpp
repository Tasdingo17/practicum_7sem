#ifndef __Generator__
#define __Generator__

#include <iostream>
#include <fstream>
#include <utility>
#include <exception>

class InputGenerator{
private:
    int nProc;
    int nTask;
    std::pair<int, int> task_duration_bounds;
public:
    InputGenerator(unsigned int _nProc, unsigned int _nTask, 
                   unsigned int leftDurBound, unsigned int rightDurBound):
        nProc(_nProc), nTask(_nTask), task_duration_bounds({leftDurBound, rightDurBound})
    {    
        if (!nProc || !nTask || !leftDurBound || !rightDurBound) 
            throw std::range_error("All initial values for InputGenerator must be possitive");
    };


    /* Format:
        Processors,n
        Tasks,m
        1,d1
        2,d2
        ...
        m,dm
    */
    void generate_input_data(const char* filename){
        std::ofstream file_data;
        file_data.open(filename);
        if (!file_data.is_open()){
            throw std::runtime_error("Failed to open file");
        }

        file_data << "Processors," << nProc << "\n";
        file_data << "Tasks," << nTask << "\n";

        int tmp = task_duration_bounds.second - task_duration_bounds.first + 1;
        for (int i = 1; i <= nTask; i++) {
            file_data << i << "," << task_duration_bounds.first + (rand() % tmp) << "\n";
        }

        file_data.close();
    }
};

#endif  // __Generator__