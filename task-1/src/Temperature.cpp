#ifndef __Temperature__
#define __Temperature__

#include <cmath>

class Temperature_decrease_law{
public:
    virtual double step(double init_temp, unsigned int iter_n) const = 0;
    virtual ~Temperature_decrease_law() {};
};


// T = T0 / ln(1 + i)
class Boltzmann_law : public Temperature_decrease_law{
public:
    double step(double init_temp, unsigned int iter_n) const override{
        return init_temp / std::log(iter_n + 1);
    }
};


// T = T0 / (1 + i)
class Cauchy_law : public Temperature_decrease_law{
public:
    double step(double init_temp, unsigned int iter_n) const override{
        return init_temp / (iter_n + 1);
    }
};


// T = T0 * ln(1 + i) / (1 + i)
class Log_law : public Temperature_decrease_law{
public:
    double step(double init_temp, unsigned int iter_n) const override{
        return init_temp * std::log(iter_n + 1) / (iter_n + 1);
    }
};

enum class Temperature_law_type{
    BOLTZMANN,
    CAUCHY,
    LOG,
};

class Temperature{
private:
    int start_temp;
    int curr_temp;
    Temperature_law_type law_type;
    Temperature_decrease_law* decr_law;
public:
    Temperature(int _start_temp, Temperature_law_type _law_type):
    start_temp(_start_temp), curr_temp(_start_temp), law_type(_law_type) {
        if (law_type == Temperature_law_type::BOLTZMANN){
            decr_law = new Boltzmann_law;
        } else if (law_type == Temperature_law_type::CAUCHY) {
            decr_law = new Cauchy_law;
        } else if (law_type == Temperature_law_type::LOG) {
            decr_law = new Log_law;
        }
    }

    int get_curr_temp() const {
        return curr_temp;
    }

    void set_curr_temp(int new_temp) {
        curr_temp = new_temp;
    }

    int get_start_temp() const {
        return start_temp;
    }

    void step(unsigned int iter_n){
        curr_temp = decr_law->step(start_temp, iter_n);
    }

    ~Temperature(){
        delete decr_law;
    }
};

#endif //__Temperature__