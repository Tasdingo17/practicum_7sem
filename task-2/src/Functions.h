#ifndef __Functions_h__
#define __Functions_h__

#include <string>
#include <vector>
#include <memory>

enum class OPERATION{
    PLUS,
    MINUS,
    TIMES,
    DIVIDE,
};

class TFunction{
public:
    virtual ~TFunction() = default;
    virtual double operator()(double x) const = 0;
    virtual std::string ToString() const = 0;
    virtual std::unique_ptr<TFunction> clone() const = 0;
    virtual double derivative(double x) const = 0;
};

class TArithmFunc: public TFunction{
public:
    TArithmFunc(const TFunction& _lhs, const TFunction& _rhs, OPERATION _oper);
    virtual double operator()(double x) const override;
    virtual std::string ToString() const override;
    virtual std::unique_ptr<TFunction> clone() const override;
    virtual double derivative(double x) const override;
private:
    std::unique_ptr<TFunction> lhs;
    std::unique_ptr<TFunction> rhs;
    OPERATION oper;
};

class IdentF: public TFunction{
public:
    virtual double operator()(double x) const override;
    virtual std::string ToString() const;
    IdentF(const std::initializer_list<double>& lst);
    virtual std::unique_ptr<TFunction> clone() const override;
    virtual double derivative(double x) const override;
};

class ConstF: public TFunction{
public:
    virtual double operator()(double x) const override;
    virtual std::string ToString() const;
    ConstF(double val);
    ConstF(const std::initializer_list<double>& lst);
    virtual std::unique_ptr<TFunction> clone() const override;
    virtual double derivative(double x) const override;
private:
    double val;
};

class PowerF: public TFunction{
public:
    virtual double operator()(double x) const override;
    virtual std::string ToString() const;
    PowerF(double val);
    PowerF(const std::initializer_list<double>& lst);
    virtual std::unique_ptr<TFunction> clone() const override;
    virtual double derivative(double x) const override;
private:
    double power;
};

class ExpF: public TFunction{
public:
    virtual double operator()(double x) const override;
    virtual std::string ToString() const;
    ExpF(const std::initializer_list<double>& lst);
    virtual std::unique_ptr<TFunction> clone() const override;
    virtual double derivative(double x) const override;
};

class PolynomialF: public TFunction{
public:
    virtual double operator()(double x) const override;
    virtual std::string ToString() const;
    PolynomialF(const std::vector<double>& _coefs);
    PolynomialF(const std::initializer_list<double>& lst);
    virtual std::unique_ptr<TFunction> clone() const override;
    virtual double derivative(double x) const override;
private:
    std::vector<double> coefs;
};

// Arithmetics

template <typename T>
TArithmFunc operator+(const TFunction& lhs, const T& rhs) {
    if constexpr (std::is_base_of_v<TFunction, T>) {
        return TArithmFunc(lhs, rhs, OPERATION::PLUS);
    } else {
        throw std::logic_error("Wrong parameters types!");
    }
}

template <typename T>
TArithmFunc operator-(const TFunction& lhs, const T& rhs) {
    if constexpr (std::is_base_of_v<TFunction, T>) {
        return TArithmFunc(lhs, rhs, OPERATION::MINUS);
    } else {
        throw std::logic_error("Wrong parameters types!");
    }
}

template <typename T>
TArithmFunc operator*(const TFunction& lhs, const T& rhs) {
    if constexpr (std::is_base_of_v<TFunction, T>) {
        return TArithmFunc(lhs, rhs, OPERATION::TIMES);
    } else {
        throw std::logic_error("Wrong parameters types!");
    }
}

template <typename T>
TArithmFunc operator/(const TFunction& lhs, const T& rhs) {
    if constexpr (std::is_base_of_v<TFunction, T>) {
        return TArithmFunc(lhs, rhs, OPERATION::DIVIDE);
    } else {
        throw std::logic_error("Wrong parameters types!");
    }
}


/**
 * @brief Find root of f(x) == 0.
 * 
 * "f(x) == 0 is equivalent to argmin(f(x)^2)".
 * Find mininum by gradient descent with constant rate.
 * @param f - arithmetic expression to find root of;
 * @param x0 - initial solution value
 * @param nIter - maximum number of iterations of gradiend descent
 * @param rate - "convergence rate"
 * @param eps - solution precision
 * @return double - solution 
 */
double findRoot(const TFunction& f, double x0=0, unsigned nIter=100, double rate=0.1, double eps=0.00001);

#endif