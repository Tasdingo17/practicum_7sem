#ifndef __ClassRational_H__
#define __ClassRational_H__

#include <string>
#include <ostream>

class Rational_number{
private:
    std::string numerator;
    std::string denominator;
    bool is_number(const std::string& s) const;
    void move_minus_to_numerator();
public:
    Rational_number();
    Rational_number(const std::string& m, const std::string& n = "1");
    Rational_number(int x, int y = 1);
    Rational_number(const Rational_number& other);
    const std::string& getNumerator() const;
    const std::string& getDenominator() const;
    friend std::ostream& operator<<(std::ostream &os, const Rational_number& n);
};


#endif // __ClassRational_H__