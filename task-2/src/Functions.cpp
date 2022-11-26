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

std::unique_ptr<TFunction> IdentF::clone() const{
    return std::make_unique<IdentF>(*this);
};

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

std::unique_ptr<TFunction> ConstF::clone() const{
    return std::make_unique<ConstF>(*this);
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

std::unique_ptr<TFunction> PowerF::clone() const{
    return std::make_unique<PowerF>(*this);
}

// Exponent function

double ExpF::operator()(double x) const{
    return exp(x);
}

std::string ExpF::ToString() const{
    return std::string("f(x) = e^x");
}

ExpF::ExpF(const std::initializer_list<double>& lst){};

std::unique_ptr<TFunction> ExpF::clone() const{
    return std::make_unique<ExpF>(*this);
}

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

std::unique_ptr<TFunction> PolynomialF::clone() const{
    return std::make_unique<PolynomialF>(*this);
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

std::string TArithmFunc::ToString() const{
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
    return std::string("Arithmetic function:") + lhs->ToString() + op + rhs->ToString();
}