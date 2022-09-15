#ifndef __ClassRational_H__
#define __ClassRational_H__

#include <string>
#include <ostream>

class Rational_number{
private:
    std::string numerator;   // reversed
    std::string denominator; // reversed
    bool is_negative;
    bool is_number(const std::string& s) const;
    //void move_minus_to_numerator();
public:
    Rational_number();
    Rational_number(const std::string& m, const std::string& n = "1");
    Rational_number(int x, int y = 1);
    Rational_number(const Rational_number& other);
    const std::string& getNumerator() const;
    const std::string& getDenominator() const;
    friend std::ostream& operator<<(std::ostream &os, const Rational_number& n);
    Rational_number operator+(const Rational_number& rhs) const;
    friend Rational_number operator-(const Rational_number& lhs, const Rational_number& rhs);
    friend Rational_number operator*(const Rational_number& lhs, const Rational_number& rhs);
    friend Rational_number operator/(const Rational_number& lhs, const Rational_number& rhs);
    friend bool operator==(const Rational_number& lhs, const Rational_number& rhs);
};


#endif // __ClassRational_H__