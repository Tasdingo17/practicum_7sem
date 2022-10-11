#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "Parser.h"
#include "../exceptions/CommonExceptions.hpp"
#include "../exceptions/ParserExceptions.hpp"


// return true if str.length() == 0
bool empty_line(const std::string& str){
    return str.length() == 0;
}

bool comment_line(const std::string& str){
    std::string tmp_word;
    std::stringstream line(str);
    line >> tmp_word;

    return tmp_word[0] == '#';
}

// check if s is integer number
bool is_number(const std::string& s){
    if (s[0] == '-'){  // if negative value
        return !s.empty() && std::find_if(s.begin() + 1,  s.end(), 
            [](unsigned char c) { return !std::isdigit(c); }) == s.end();
    } else {
        return !s.empty() && std::find_if(s.begin(),  s.end(), 
            [](unsigned char c) { return !std::isdigit(c); }) == s.end();
    }
}

void remove_spaces(std::string& s){
    s.erase(std::remove_if(s.begin(), s.end(), isspace), s.end());
}

int string_to_number(const std::string& s){
    if (is_number(s)){
        return std::stoi(s);
    } else {
        throw Not_a_number("Not a number during parsing: ", s);
    }
}

//remove whitespaces, TODO!!!!!!!!: make check for number
std::string string_to_string_number(const std::string& s){
    std::string res(s);
    remove_spaces(res);
    return res;
}

bool is_possible_type(const std::string& type){
    return type == "rational" || type == "complex" || type == "bit";
}

// assume file_data is already succesfully attached to file
void Parser::_parse_matrix_config(std::ifstream& file_data){
    std::string tmp_line, tmp_word;
    while (std::getline(file_data, tmp_line)){
        if (empty_line(tmp_line) || comment_line(tmp_line)) continue;

        std::stringstream line(tmp_line);
        line >> tmp_word;
        if (tmp_word != "matrix"){
            throw Parser_error("Parser_error: wrong struct type, expected 'matrix', got: ", tmp_word);
        }
        
        line >> type;
        if (!is_possible_type(type)){
            throw Parser_error("Parser_error: unsupported elements parsing type: ", type);
        }

        line >> tmp_word;
        rows = string_to_number(tmp_word);
        line >> tmp_word;
        columns = string_to_number(tmp_word);
        if (rows <= 0 || columns <= 0){
            throw Parser_error("Parser_error: matrix rows and column must be possitive integers");
        }
        break;
    }
}

// assume file_data is already succesfully attached to file
void Parser::_parse_vector_config(std::ifstream& file_data){
    std::string tmp_line, tmp_word;
    while (std::getline(file_data, tmp_line)){
        if (empty_line(tmp_line) || comment_line(tmp_line)) continue;

        std::stringstream line(tmp_line);
        line >> tmp_word;
        if (tmp_word != "vector"){
            throw Parser_error("Parser_error: wrong struct type, expected 'vector', got: ", tmp_word);
        }
        
        line >> type;
        if (!is_possible_type(type)){
            throw Parser_error("Parser_error: unsupported elements parsing type: ", type);
        }

        line >> tmp_word;
        max_size = string_to_number(tmp_word);
        if (max_size <= 0){
            throw Parser_error("Parser_error: vector max_size must be possitive integer");
        }
        break;
    }
}

void Parser::_set_delimeters(){
    if (type == "rational"){
        delim_start = '<';
        delim_middle = '/';
        delim_end = '>';
        is_single_val = false;
    } else if (type == "complex"){
        delim_start = '(';
        delim_middle = ',';
        delim_end = ')';
        is_single_val = false;
    } else if (type == "bit"){
        is_single_val = true;
        delim_start = delim_end = delim_middle = '\0';
    } else {                    // unreachable?
        is_single_val = true;   // he-he
        throw Parser_error("Unsupported value type: ", type);
    }
    return;
}


coords Parser::_parse_matrix_coords(std::stringstream& line){
    std::string tmp_word;
    int x_coord, y_coord;
    line >> tmp_word;
    x_coord = string_to_number(tmp_word);
    line >> tmp_word;
    y_coord = string_to_number(tmp_word);
    if (x_coord <= 0 || y_coord <= 0){
        throw Parser_error("Parser_error: matrix element coordinates must be possitive integers!");
    }
    return {x_coord, y_coord};
}

int Parser::_parse_vector_coords(std::stringstream& line){
    std::string tmp_word;
    int pos;
    line >> tmp_word;
    pos = string_to_number(tmp_word);
    if (pos <= 0){
        throw Parser_error("Parser_error: vector element index must be possitive integer!");
    }
    return pos;
}


pair_str_vals Parser::__parse_single_val(std::stringstream& line){
    std::string first_val, second_val, tmp_word;
    std::getline(line, tmp_word);
    
    std::stringstream tmp_line(tmp_word);
    std::getline(tmp_line, tmp_word, '#');      //Cut comments (case: '1#comment')
    remove_spaces(tmp_word);

    first_val = std::to_string(string_to_number(tmp_word));     //check is_number, remove whispaces and leading zeros
    second_val = "";
    return {first_val, second_val};
}

// rational value can be <num/denom> and <num>
pair_str_vals Parser::__parse_rational(std::stringstream& line){
    std::string first_val, second_val, tmp_word;
    std::getline(line, tmp_word, delim_start);  // skip to start of value
    std::getline(line, tmp_word, delim_end);    // get the whole value, tmp_word stores is
    
    int delim_pos = tmp_word.find(delim_middle);
    first_val =  string_to_string_number(tmp_word.substr(0, delim_pos));

    if (delim_pos == -1){    // no delimeter in rational value => it's a whole number
        second_val = "1";
    } else {
        second_val = string_to_string_number(tmp_word.substr(delim_pos + 1));
    }
    return {first_val, second_val};
}

pair_str_vals Parser::__parse_complex(std::stringstream& line){
    std::string first_val, second_val, tmp_word;
    std::getline(line, tmp_word, delim_start);  // skip to start of value
    std::getline(line, tmp_word, delim_middle);
    first_val = string_to_string_number(tmp_word);

    std::getline(line, tmp_word, delim_end);
    second_val = string_to_string_number(tmp_word);
    return {first_val, second_val};
}

pair_str_vals Parser::_parse_val(std::stringstream& line){
    if (type == "rational"){
        return __parse_rational(line);
    } else if(type == "complex"){
        return __parse_complex(line);
    } else if (type == "bit" || is_single_val){
        // for future
        pair_str_vals res = std::move(__parse_single_val(line));
        if (res.first != "1" && res.first != "0"){
            throw Parser_error("bit value must be 1 or 0, got: ", res.first);
        }
            return res;
    } else {
        throw Parser_error("Unsupported value type: ", type);
    }
}


void Parser::_parse_and_set_matr_value(const std::string& tmp_line){
    std::stringstream line(tmp_line);

    coords pos = _parse_matrix_coords(line);
    pair_str_vals val = _parse_val(line); 
    matrix_vals[pos] = val;
}

void Parser::_parse_and_set_vect_value(const std::string& tmp_line){
    std::stringstream line(tmp_line);

    int pos = _parse_vector_coords(line);
    pair_str_vals val = _parse_val(line); 
    vector_vals[pos] = val;
}


// assume file_data is already succesfully attached to file
void Parser::_parse_matrix_values(std::ifstream& file_data){
    std::string tmp_line;
    while (std::getline(file_data, tmp_line)){
        if (empty_line(tmp_line) || comment_line(tmp_line)) continue;
        _parse_and_set_matr_value(tmp_line);
    }
}

// assume file_data is already succesfully attached to file
void Parser::_parse_vector_values(std::ifstream& file_data){
    std::string tmp_line;
    while (std::getline(file_data, tmp_line)){
        if (empty_line(tmp_line) || comment_line(tmp_line)) continue;
        _parse_and_set_vect_value(tmp_line);
    }
}

std::ifstream Parser::_open_file_safe(const char* filename) const{
    std::ifstream file_data;
    file_data.open(filename);
    if (!file_data.is_open()){
        throw File_open_error("Fail opening file: ", std::string(filename));
    }
    return file_data;
}

void Parser::parse_matrix(const char* filename){
    vector_vals.clear();
    matrix_vals.clear();
    
    std::ifstream file_data = _open_file_safe(filename);
    
    try{
        _parse_matrix_config(file_data);
        _set_delimeters();
        _parse_matrix_values(file_data);
    } catch (...){
        file_data.close();
        throw;
    }

    file_data.close();
}

void Parser::parse_vector(const char* filename){
    vector_vals.clear();
    matrix_vals.clear();
    
    std::ifstream file_data = _open_file_safe(filename);

    try{
        _parse_vector_config(file_data);
        _set_delimeters();
        _parse_vector_values(file_data);
    } catch (...){
        file_data.close();
        throw;
    }

    file_data.close();
}

int Parser::get_rows_number() const{
    return rows;
}

int Parser::get_columns_number() const{
    return columns;
}

std::pair<int, int> Parser::get_dims() const{
    return {rows, columns};
}

std::string Parser::get_type() const {
    return type;
}

std::unordered_map<coords, pair_str_vals, pair_hash > Parser::get_matrix_vals() const{
    return matrix_vals;
}

int Parser::get_max_size() const{
    return max_size;
}

std::map<int, pair_str_vals> Parser::get_vector_vals() const{
    return vector_vals;
}

void Parser::print_matrix_config() const{
    std::cout << "type: " << type << " rows: " << rows << " columns: " << columns << std::endl;     // test
}

void Parser::print_matrix_values() const{
    for(const auto& elem: matrix_vals){
        std::cout << '(' << elem.first.first << ',' << elem.first.second << "): ";
        std::cout << delim_start << elem.second.first << delim_middle << elem.second.second << delim_end;
        std::cout << std::endl;
    }
}

void Parser::print_vector_config() const{
    std::cout << "type: " << type << " max_size: " << max_size << std::endl;
}

void Parser::print_vector_values() const{
    for(const auto& elem: vector_vals){
        std::cout << elem.first << ": ";
        std::cout << delim_start << elem.second.first << delim_middle << elem.second.second << delim_end;
        std::cout << std::endl;
    }
}