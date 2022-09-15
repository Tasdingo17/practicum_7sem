#include <iostream>
#include <algorithm>
#include "ClassRationalNumber.h"


void Rational_number::move_minus_to_numerator(){
    if (denominator[0] != '-'){
        throw 1;  // TODO: make special exception
    }
    denominator.erase(0, 1);  //erase '-'
    if (numerator[0] == '-'){
        numerator.erase(0, 1);
    } else if (numerator != "0") {
        numerator.insert(0, 1, '-');
    }
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


Rational_number::Rational_number(): numerator("0"), denominator("1") {};


Rational_number::Rational_number(const std::string& m, const std::string& n):
    numerator(m), denominator(n) {
    if (!is_number(numerator) || !is_number(denominator)) {
        throw 2;  // TODO: make special exception
    }
    if (m == "0"){
        denominator = std::string("1");
        return;
    }
    if (n[0] == '-') move_minus_to_numerator();
};


Rational_number::Rational_number(int m, int n): 
    numerator(std::to_string(m)), denominator(std::to_string(n)) {
    if (n == 0){
        throw 2;  // TODO: make special exception
    }
    if (m == 0){
        denominator = std::string("1");
        return;
    }
    if (n < 0){  // Move a minus sign to numerator
        move_minus_to_numerator();
    }
};


Rational_number::Rational_number(const Rational_number& other):
    numerator(other.numerator), denominator(other.denominator) {};


std::ostream& operator<<(std::ostream &os, const Rational_number& n){
    return os  << n.numerator << "/" << n.denominator;
}
