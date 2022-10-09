#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "Parser.h"

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
        throw 14;       // todo: not a number exception
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
        if (tmp_word != "matrix") throw 12;     //todo: wrong format exception
        
        line >> type;
        if (!is_possible_type(type)){
            throw 13;                      // todo: exception
        }

        line >> tmp_word;
        rows = string_to_number(tmp_word);
        line >> tmp_word;
        columns = string_to_number(tmp_word);
        if (rows <= 0 || columns <= 0) throw 14;    // todo: wrong dims exceptions
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
    } else if (type == "bit"){      // TO CHECK!
        is_single_val = true;
        delim_start = delim_end = delim_middle = '\0';
    } else {
        is_single_val = true;   // he-he
        throw 4;      // todo: unsupported type exceptions
    }
    return;
}


coords Parser::_parse_coords(std::stringstream& line){
    std::string tmp_word;
    int x_coord, y_coord;
    line >> tmp_word;
    x_coord = string_to_number(tmp_word);
    line >> tmp_word;
    y_coord = string_to_number(tmp_word);
    if (x_coord <= 0 || y_coord <= 0) throw 6;  // TODO: wrong coordinate exceptions
    return {x_coord, y_coord};
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
        if (res.first != "1" && res.first != "0") throw 17;  // TODO: exception wrong bit
            return res;
    } else {
        throw 5;    // todo:: unsupported class exception
    }
}


void Parser::_parse_and_set_matr_value(const std::string& tmp_line){
    std::stringstream line(tmp_line);

    coords pos = _parse_coords(line);
    pair_str_vals val = _parse_val(line); 
    matrix_vals[pos] = val;
}


// assume file_data is already succesfully attached to file
void Parser::_parse_matrix_values(std::ifstream& file_data){
    std::string tmp_line;
    while (std::getline(file_data, tmp_line)){
        if (empty_line(tmp_line) || comment_line(tmp_line)) continue;
        _parse_and_set_matr_value(tmp_line);
    }
}


void Parser::parse_matrix(const char* filename){
    matrix_vals.clear();
    std::ifstream file_data;
    file_data.open(filename);
    if (!file_data.is_open()){
        throw 10;   // todo:exceptions (!note: not only non-exsisting also permission failures)
    }
    _parse_matrix_config(file_data);
    std::cout << "type: " << type << " rows: " << rows << " columns: " << columns << std::endl;     // test
    _set_delimeters();
    _parse_matrix_values(file_data);

    //test
    for(const auto& elem: matrix_vals){
        std::cout << '(' << elem.first.first << ',' << elem.first.second << "): ";
        std::cout << delim_start << elem.second.first << delim_middle << elem.second.second << delim_end;
        std::cout << std::endl;
    }
    
    file_data.close();
}