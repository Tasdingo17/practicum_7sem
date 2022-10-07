#include "Matrix_coords.h"

bool Matrix_coords::is_consistent() const{
    bool wrong_cond1 = left_x < -1 || left_y < -1 || right_x < -1 || right_y < -1;
    bool wrong_cond2 = (left_x > right_x && left_x != -1 && right_x !=-1) ||
                 (left_y > right_y && left_y != -1 && right_y !=-1);
    return !(wrong_cond1 || wrong_cond2);
}

Matrix_coords::Matrix_coords():
    left_x(-1), left_y(-1), right_x(-1), right_y(-1) {};

Matrix_coords::Matrix_coords(const coords& left, const coords& right){
    left_x = left.first;
    left_y = left.second;
    right_x = right.first;
    right_y = right.second;
    if (!is_consistent()) throw 8;  // TODO: make exception
}

Matrix_coords::Matrix_coords(const coords& left): Matrix_coords(left, left){};

Matrix_coords::Matrix_coords(const Matrix_coords& other):
    left_x(other.left_x), left_y(other.left_y), right_x(other.right_x),
    right_y(other.right_y) {};

Matrix_coords::Matrix_coords(Matrix_coords&& other){
    left_x = std::move(other.left_x);
    left_y = std::move(other.left_y);
    right_x = std::move(other.right_x);
    right_y = std::move(other.right_y);
}

Matrix_coords& Matrix_coords::operator=(const Matrix_coords& other){
    left_x = other.left_x; 
    left_y = other.left_y;
    right_x = other.right_x;
    right_y = other.right_y;
    return *this;
}

Matrix_coords& Matrix_coords::operator=(Matrix_coords&& other){
    left_x = std::move(other.left_x);
    left_y = std::move(other.left_y);
    right_x = std::move(other.right_x);
    right_y = std::move(other.right_y);
    return *this;
}

std::string Matrix_coords::to_string() const{
    std::string res("<(");
    res = res + std::to_string(left_x) + "," + std::to_string(left_y) + "), (" +
          std::to_string(right_x) + "," + std::to_string(right_y) + ")>";
    return res;
}

std::ostream& operator<<(std::ostream &os, const Matrix_coords& coords){
    return os << coords.to_string();
}

bool Matrix_coords::has(const coords& dot) const{
    // dot.first is x coord, dot.second is y coord
    bool is_top_ok = left_y <= dot.second;
    bool is_left_ok = left_x <= dot.first;
    bool is_bottom_ok = dot.second <= right_y || right_y == -1;
    bool is_right_ok = dot.first <= right_x || right_x == -1;
    return is_top_ok && is_bottom_ok && is_left_ok && is_right_ok;
}

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

Matrix_row_coord::Matrix_row_coord(): row_index(0){};

Matrix_row_coord::Matrix_row_coord(int index){
    if (index < 0) throw 1; // TODO: exception
    row_index = index;
}

int Matrix_row_coord::get_row_index() const{
    return row_index; 
}

bool Matrix_row_coord::has(const coords& dot) const{
    return dot.first == row_index;
}

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

Matrix_column_coord::Matrix_column_coord(): column_index(0){};

Matrix_column_coord::Matrix_column_coord(int index){
    if (index < 0) throw 1; // TODO: exception
    column_index = index;
}

int Matrix_column_coord::get_column_index() const{
    return column_index; 
}

bool Matrix_column_coord::has(const coords& dot) const{
    return dot.second == column_index;
}