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


class Init_error: public std::exception{
private:
    std::string m_error;
public:
    Init_error(const std::string& error, const std::string& _val=std::string{}){
        m_error = error + _val;
    }

    Init_error(const std::string& error, std::pair<int, int> dim1, std::pair<int, int> dim2){
        std::string tmp1 = std::string("<") + std::to_string(dim1.first) + ", " +
                           std::to_string(dim1.second) + ">";
        std::string tmp2 = std::string("<") + std::to_string(dim2.first) + ", " +
                           std::to_string(dim2.second) + ">";
        m_error = error + tmp1 + " " + tmp2;
    }

    const char* what() const noexcept override {  
        return m_error.c_str(); 
    }
};

class Shape_error: public std::exception{
private:
    std::string m_error;
public:
    Shape_error(const std::string& error, std::pair<int, int> dim1, std::pair<int, int> dim2){
        std::string tmp1 = std::string("<") + std::to_string(dim1.first) + ", " +
                           std::to_string(dim1.second) + ">";
        std::string tmp2 = std::string("<") + std::to_string(dim2.first) + ", " +
                           std::to_string(dim2.second) + ">";
        m_error = error + tmp1 + " " + tmp2;
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