/**
 * @file CommonExceptions.hpp
 * @brief Common exceptions
 */
#ifndef __CommonExcept__
#define __CommonExcept__

#include <exception>
#include <string>
#include <iostream>

class Zero_division: public std::exception{
private:
    std::string m_error;
public:
    Zero_division(const std::string& error, const std::string& denom=std::string{}, 
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

#endif