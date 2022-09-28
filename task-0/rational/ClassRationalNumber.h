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
public:
    Rational_number();
    Rational_number(const std::string& m, const std::string& n = "1");
    Rational_number(const char* m, const char* n = "1");
    Rational_number(long int x, long int y = 1);
    Rational_number(const Rational_number& other);
    void make_canonical();
    long long floor() const;
    long long round() const; 
    	
    explicit operator int() const;
    explicit operator long() const;
    explicit operator short() const;

    friend std::ostream& operator<<(std::ostream &os, const Rational_number& n);
    friend Rational_number abs(const Rational_number& obj);
    std::string to_string() const;

    friend bool operator==(const Rational_number& lhs, const Rational_number& rhs);
    friend bool operator<(const Rational_number& lhs, const Rational_number& rhs);
    friend bool operator!=(const Rational_number& left, const Rational_number& right);
    friend bool operator<=(const Rational_number& left, const Rational_number& right);
    friend bool operator>(const Rational_number& left, const Rational_number& right);
    friend bool operator>=(const Rational_number& left, const Rational_number& right);
    
    friend Rational_number operator+(const Rational_number& lhs, const Rational_number& rhs);
    friend Rational_number operator-(const Rational_number& lhs, const Rational_number& rhs);
    friend Rational_number operator*(const Rational_number& lhs, const Rational_number& rhs);
    friend Rational_number operator/(const Rational_number& lhs, const Rational_number& rhs);

    Rational_number& operator=(const Rational_number& v);
    Rational_number& operator+=(const Rational_number& v);
    Rational_number& operator-=(const Rational_number& v);
    Rational_number& operator*=(const Rational_number& v);
    Rational_number& operator/=(const Rational_number& v);

    Rational_number operator+() const;   //unar +
    Rational_number operator-() const;   //unar -
    Rational_number operator++();        //prefix incr
    Rational_number operator++(int);     //postfix incr
    Rational_number operator--();        //prefix decr
    Rational_number operator--(int);     //postfix decr
};


#endif // __ClassRational_H__