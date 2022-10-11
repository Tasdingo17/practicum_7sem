/**
 * @file MatrixExceptions.hpp
 * @brief Exceptions for class Matrix
 */

#ifndef __MatrExceptions__
#define __MatrExceptions__

#include <exception>
#include <string>
#include <iostream>

class Flag_conflict : public std::exception{
private:
    std::string m_error;
public:
    Flag_conflict(const std::string& error){
        m_error = error;
    }

    const char* what() const noexcept override {  
        return m_error.c_str(); 
    }
};

class Proxy_error: public std::exception{
private:
    std::string m_error;
public:
    Proxy_error(const std::string& error){
        m_error = error;
    }

    const char* what() const noexcept override {  
        return m_error.c_str(); 
    }
};


#endif //__MatrExceptions