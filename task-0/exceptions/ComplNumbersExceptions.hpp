/**
 * @file ComplNumbersExceptions.hpp
 * @brief Exceptions for class Complex_number
 */

#ifndef __ComplExcept__
#define __ComplExcept__

#include <exception>
#include <string>
#include <iostream>
#include "../complex/ClassComplex.h"

class Bad_construct: public std::exception{
private:
    std::string m_error;
public:
    Bad_construct(const std::string& error){
        m_error = error;
    }

    const char* what() const noexcept override {  
        return m_error.c_str(); 
    }
};


template<class R, class T>
class Bad_cast: public std::exception{
private:
    std::string m_error;
public:
    Bad_cast(const std::string& error, const Complex_number<R, T>& val){
        //if (val == nullptr){
        //    m_error = error;
        //} else {
        m_error = error + val.to_string();
        //}
    }

    const char* what() const noexcept override {  
        return m_error.c_str(); 
    }
};

#endif