/**
 * @file ParserExceptions.hpp
 * @brief Еxceptions for class Parser
 * 
 */

#ifndef __ParserExcept__
#define __ParserExcept__

#include <exception>
#include <string>
#include <iostream>

class Parser_error: public std::exception{
private:
    std::string m_error;
public:
    Parser_error(const std::string& error, const std::string& _val=std::string{}){
        m_error = error + _val;
    }

    const char* what() const noexcept override {  
        return m_error.c_str(); 
    }
};


#endif  // __ParserExcept__