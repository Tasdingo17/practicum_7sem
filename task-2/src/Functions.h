#ifndef __Functions_h__
#define __Functions_h__

#include <string>
#include <vector>

class TFunction{
public:
    virtual ~TFunction() = default;
    virtual double operator()(double x) const = 0;
    virtual std::string ToString() const = 0;
};

class IdentF: public TFunction{
public:
    virtual double operator()(double x) const override;
    virtual std::string ToString() const;
    IdentF(const std::initializer_list<double>& lst);
};

class ConstF: public TFunction{
public:
    virtual double operator()(double x) const override;
    virtual std::string ToString() const;
    ConstF(double val);
    ConstF(const std::initializer_list<double>& lst);
private:
    double val;
};

class PowerF: public TFunction{
public:
    virtual double operator()(double x) const override;
    virtual std::string ToString() const;
    PowerF(double val);
    PowerF(const std::initializer_list<double>& lst);
private:
    double power;
};

class ExpF: public TFunction{
public:
    virtual double operator()(double x) const override;
    virtual std::string ToString() const;
    ExpF(const std::initializer_list<double>& lst);
};

class PolynomialF: public TFunction{
public:
    virtual double operator()(double x) const override;
    virtual std::string ToString() const;
    PolynomialF(const std::vector<double>& _coefs);
    PolynomialF(const std::initializer_list<double>& lst);
private:
    std::vector<double> coefs;
};

#endif