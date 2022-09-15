#include <iostream>
#include <algorithm>
#include "ClassRationalNumber.h"


int max(int a, int b){
    return (a >= b) ? a : b;
}

// erase a '-' sign and return int value as reversed string
std::string int_to_val_reversed(int x){         
    int a = (x >= 0) ? x : -x;
    std::string s = std::to_string(a);
    std::reverse(s.begin(), s.end());
    return s;
}


bool Rational_number::is_number(const std::string& s) const{
    if (s[0] == '-'){  // if negative value
        return !s.empty() && std::find_if(s.begin() + 1,  s.end(), 
            [](unsigned char c) { return !std::isdigit(c); }) == s.end();
    } else {
        return !s.empty() && std::find_if(s.begin(),  s.end(), 
            [](unsigned char c) { return !std::isdigit(c); }) == s.end();
    }
}


Rational_number::Rational_number(): numerator("0"), denominator("1"),
                                    is_negative(false) {};


Rational_number::Rational_number(const std::string& m, const std::string& n):
    numerator(m), denominator(n) {
    is_negative = ((m[0] == '-') != (n[0] == '-')) && (m != "0");  // xor
    if (!is_number(numerator) || !is_number(denominator)) {
        throw 2;  // TODO: make special exception
    }
    if (m == "0"){
        denominator = std::string("1");
        is_negative = false;
        return;
    }
    if (numerator[0] == '-') numerator.erase(0, 1);
    if (denominator[0] == '-') denominator.erase(0, 1);
    std::reverse(numerator.begin(), numerator.end());
    std::reverse(denominator.begin(), denominator.end());
};


Rational_number::Rational_number(int m, int n){
    is_negative = (m >= 0) != (n > 0);  // xor
    if (n == 0){
        throw 2;  // TODO: make special exception
    }
    numerator = int_to_val_reversed(m);
    if (m == 0){
        denominator = std::string("1");
        is_negative = false;
        return;
    }
    denominator = int_to_val_reversed(n);
};


Rational_number::Rational_number(const Rational_number& other):
    numerator(other.numerator), denominator(other.denominator), 
    is_negative(other.is_negative) {};


std::ostream& operator<<(std::ostream &os, const Rational_number& n){
    std::string tmp1 = n.numerator, tmp2 = n.denominator;
    std::reverse(tmp1.begin(), tmp1.end()); 
    std::reverse(tmp2.begin(), tmp2.end());
    return os << (n.is_negative ? "-" : "") << tmp1 << '/' << tmp2;
}


//bool operator==(const Rational_number& lhs, const Rational_number& rhs){
//    return lhs.numerator * rhs.denominator == lhs.denominator * rhs.numerator;
//}