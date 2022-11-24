#include "Functions.h"
#include <cmath>
#include <stdexcept>

// IDENT FUNCTION
double IdentF::operator()(double x) const{
    return x;
}

std::string IdentF::ToString() const{
    return std::string("f(x) = x");
}

IdentF::IdentF(const std::initializer_list<double>& lst){};

// CONSTANT FUNCTION

double ConstF::operator()(double x) const{
    return val;
}

std::string ConstF::ToString() const{
    return std::string("f(x) = ") + std::to_string(val);
}

ConstF::ConstF(double x): val(x) {};

ConstF::ConstF(const std::initializer_list<double>& lst){
    if (lst.size() != 1){
        throw std::invalid_argument("Wrong initializer_list size for Constant function");
    }
    val = *lst.begin();
}

// Power function

double PowerF::operator()(double x) const{
    return pow(x, power);
}

std::string PowerF::ToString() const{
    return std::string("f(x) = x^") + std::to_string(power);
}

PowerF::PowerF(double x): power(x) {};

PowerF::PowerF(const std::initializer_list<double>& lst){
    if (lst.size() != 1){
        throw std::invalid_argument("Wrong initializer_list size for Power function");
    }
    power = *lst.begin();
}

// Exponent function

double ExpF::operator()(double x) const{
    return exp(x);
}

std::string ExpF::ToString() const{
    return std::string("f(x) = e^x");
}

ExpF::ExpF(const std::initializer_list<double>& lst){};


// Polynomial function

double PolynomialF::operator()(double x) const{
    double res = 0;
    for (auto coef = coefs.rbegin(); coef != coefs.rend(); coef++){
        res = res * x + *coef;
    }
    return res;
}
    
std::string PolynomialF::ToString() const{
    std::string output("f(x) = ");
    unsigned short i;
    for ( i = 0; i < coefs.size() - 1; i++){
        output += std::to_string(coefs[i]) + std::string("*x^") + std::to_string(i) + " + ";
    }
    output += std::to_string(coefs[i]) + std::string("*x^") + std::to_string(i);
    return output;
}

PolynomialF::PolynomialF(const std::vector<double>& _coefs): coefs(_coefs){
    if (coefs.empty()){
        throw std::invalid_argument("Coefs of polynom must be non-empty");
    }
};

PolynomialF::PolynomialF(const std::initializer_list<double>& lst): coefs(lst){
    if (lst.size() == 0){
        throw std::invalid_argument("Coefs of polynom must be non-empty");
    }
}
