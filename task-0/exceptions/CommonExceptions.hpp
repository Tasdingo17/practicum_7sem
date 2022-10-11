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

class File_open_error: public std::exception{
private:
    std::string m_error;
public:
    File_open_error(const std::string& error, const std::string& _val=std::string{}){
        m_error = error + _val;
    }

    const char* what() const noexcept override {  
        return m_error.c_str(); 
    }
};

class Type_error: public std::exception{
private:
    std::string m_error;
public:
    Type_error(const std::string& error, const std::string& _val=std::string{}){
        m_error = error + _val;
    }

    const char* what() const noexcept override {  
        return m_error.c_str(); 
    }
};

class Out_of_range: public std::exception{
private:
    std::string m_error;
public:
    Out_of_range(const std::string& error, const std::string& _val=std::string{}){
        m_error = error + _val;
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

    Shape_error(const std::string& error, int dim1, int dim2){
        m_error = error + std::to_string(dim1) + " and " + std::to_string(dim2);
    }

    const char* what() const noexcept override {  
        return m_error.c_str(); 
    }
};

#endif