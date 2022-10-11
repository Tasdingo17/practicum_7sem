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