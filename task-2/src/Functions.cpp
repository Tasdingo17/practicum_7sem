#include "Functions.h"
#include <cmath>
#include <stdexcept>
#include <sstream>

template <typename T>
std::string to_string_with_precision(const T a_value, const int n = 6){
    std::ostringstream out;
    out.precision(n);
    out << std::fixed << a_value;
    return out.str();
}

// IDENT FUNCTION
double IdentF::operator()(double x) const{
    return x;
}

std::string IdentF::ToString(const int precision) const{
    return std::string("f(x) = x");
}

IdentF::IdentF(const std::initializer_list<double>& lst){};

std::unique_ptr<TFunction> IdentF::clone() const{
    return std::make_unique<IdentF>(*this);
};

double IdentF::derivative(double x) const{
    return 1;
}

// CONSTANT FUNCTION

double ConstF::operator()(double x) const{
    return val;
}

std::string ConstF::ToString(const int precision) const{
    return std::string("f(x) = ") + to_string_with_precision(val, precision);
}

ConstF::ConstF(double x): val(x) {};

ConstF::ConstF(const std::initializer_list<double>& lst){
    if (lst.size() != 1){
        throw std::invalid_argument("Wrong initializer_list size for Constant function");
    }
    val = *lst.begin();
}

std::unique_ptr<TFunction> ConstF::clone() const{
    return std::make_unique<ConstF>(*this);
}

double ConstF::derivative(double x) const{
    return 0;
}

// Power function

double PowerF::operator()(double x) const{
    return pow(x, power);
}

std::string PowerF::ToString(const int precision) const{
    return std::string("f(x) = x^") + to_string_with_precision(power, precision);
}

PowerF::PowerF(double x): power(x) {};

PowerF::PowerF(const std::initializer_list<double>& lst){
    if (lst.size() != 1){
        throw std::invalid_argument("Wrong initializer_list size for Power function");
    }
    power = *lst.begin();
}

std::unique_ptr<TFunction> PowerF::clone() const{
    return std::make_unique<PowerF>(*this);
}

double PowerF::derivative(double x) const{
    return power * pow(x, power - 1);
}

// Exponent function

double ExpF::operator()(double x) const{
    return exp(x);
}

std::string ExpF::ToString(const int precision) const{
    return std::string("f(x) = e^x");
}

ExpF::ExpF(const std::initializer_list<double>& lst){};

std::unique_ptr<TFunction> ExpF::clone() const{
    return std::make_unique<ExpF>(*this);
}

double ExpF::derivative(double x) const{
    return this->operator()(x);
}

// Polynomial function

double PolynomialF::operator()(double x) const{
    double res = 0;
    for (auto coef = coefs.rbegin(); coef != coefs.rend(); coef++){
        res = res * x + *coef;
    }
    return res;
}
    
std::string PolynomialF::ToString(const int precision) const{
    std::string output("f(x) = ");
    unsigned short i;
    for ( i = 0; i < coefs.size() - 1; i++){
        output += to_string_with_precision(coefs[i], precision) + 
                  std::string("*x^") + std::to_string(i) + " + ";
    }
    output += to_string_with_precision(coefs[i], precision) + 
              std::string("*x^") + std::to_string(i);
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

std::unique_ptr<TFunction> PolynomialF::clone() const{
    return std::make_unique<PolynomialF>(*this);
}

double PolynomialF::derivative(double x) const{
    double res = 0;
    int n = this->coefs.size() - 1;
    for (auto coef = coefs.rbegin(); coef != coefs.rend() - 1; coef++){
        res = res * x + *coef * n;
        n -= 1;
    }
    return res;
}

// Arithmetics

TArithmFunc::TArithmFunc(const TFunction& _lhs, const TFunction& _rhs, OPERATION _oper):
    lhs(_lhs.clone()), rhs(_rhs.clone()), oper(_oper) {};

std::unique_ptr<TFunction> TArithmFunc::clone() const{
    return std::make_unique<TArithmFunc>(*lhs, *rhs, oper);
}

double TArithmFunc::operator()(double x) const{
    switch (oper){
        case OPERATION::PLUS:
            return (*lhs)(x) + (*rhs)(x);
        case OPERATION::MINUS:
            return (*lhs)(x) - (*rhs)(x);
        case OPERATION::TIMES:
            return (*lhs)(x) * (*rhs)(x);
        case OPERATION::DIVIDE:
            return (*lhs)(x) / (*rhs)(x);
        default:    // unreachable
            throw std::logic_error("Wrong operation");
    }
}

std::string TArithmFunc::ToString(const int precision) const{
    std::string op;
    switch(oper){
        case OPERATION::PLUS:
            op = " + ";
            break;
        case OPERATION::MINUS:
            op = " - ";
            break;
        case OPERATION::TIMES:
            op = " * ";
            break;
        case OPERATION::DIVIDE:
            op = " / ";
        default:    // unreachable
            throw std::logic_error("Wrong operation");
    }
    return lhs->ToString(precision) + op + rhs->ToString(precision);
}

double TArithmFunc::derivative(double x) const{
    switch(oper){
        case OPERATION::PLUS:
            return lhs->derivative(x) + rhs->derivative(x);
        case OPERATION::MINUS:
            return lhs->derivative(x) - rhs->derivative(x);
        case OPERATION::TIMES:
            return lhs->derivative(x) * rhs->operator()(x) + lhs->operator()(x) * rhs->derivative(x);
        case OPERATION::DIVIDE:
            return (lhs->derivative(x) * rhs->operator()(x) - lhs->operator()(x) * rhs->derivative(x)) /
                   (rhs->operator()(x) * rhs->operator()(x)); 
        default:    // unreachable
            throw std::logic_error("Wrong operation");
    }
    return 0;
}

// Solve f(x) == 0;
//#include <iostream>
double findRoot(const TFunction& f, double _x0, unsigned nIter, double rate){//, double eps){
    double x_0 = _x0, x_1 = x_0;   // x0
    //auto g = f * f; // g = f(x)^2
    unsigned iter = 0;
    double x_best = x_0;
    double f_best = std::abs(f(x_best));
    while (iter < nIter){
        x_0 = x_1;
        x_1 = x_0 - rate * f.derivative(x_0) * (f(x_0) > 0 ? 1 : -1);
        //if (std::abs(x_1 - x_0) < eps){
        //    break;
        //}
        if (std::abs(f(x_1)) < f_best){
            x_best = x_1;
            f_best = std::abs(f(x_1));
        }
        iter++;
    }
    return x_best;
}