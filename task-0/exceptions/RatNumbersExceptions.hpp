/**
 * @file RatNumbersExceptions.hpp
 * @brief Еxceptions for class Rational_number
 * 
 */

#ifndef __RatExcept__
#define __RatExcept__


#include <exception>
#include <string>
#include <iostream>
#include "../rational/ClassRationalNumber.h"


class Not_a_number : public std::exception{
private:
    std::string m_error;
public:
    Not_a_number(const std::string& error, const std::string& _val=std::string{}){
        m_error = error + _val;
    }

    const char* what() const noexcept override {  
        return m_error.c_str(); 
    }
};

class Zero_division_rat: public std::exception{
private:
    std::string m_error;
public:
    Zero_division_rat(const std::string& error, const std::string& denom=std::string{}, 
                      const std::string& num=std::string{}){
        if (num.empty() || denom.empty()){
            m_error = error;
        } else {
            m_error = error + num + std::string(" / ") + denom;
        }

    }

    const char* what() const noexcept override {  
        return m_error.c_str(); 
    }
};

class Out_of_bounds: public std::exception{
private:
    std::string m_error;
public:
    Out_of_bounds(const std::string& error, const Rational_number& val = Rational_number("0")){
        if (val == Rational_number("0", "1")){
            m_error = error;
        } else {
            m_error = error + val.to_string();
        }
    }

    const char* what() const noexcept override {  
        return m_error.c_str(); 
    }
};

class Bad_cast: public std::exception{
private:
    std::string m_error;
public:
    Bad_cast(const std::string& error, const Rational_number& val = Rational_number("0")){
        if (val == Rational_number("0", "1")){
            m_error = error;
        } else {
            m_error = error + val.to_string();
        }
    }

    const char* what() const noexcept override {  
        return m_error.c_str(); 
    }
};

#endif