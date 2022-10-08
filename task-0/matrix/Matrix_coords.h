#ifndef __ClassMatrixCoords_H__
#define __ClassMatrixCoords_H__

#include <utility>
#include <string>
#include <iostream>

using coords = std::pair<int, int>;

template<class T>
class Matrix;

class Matrix_coords{
private:
    int left_x, left_y, right_x, right_y; // top left, bottom right coordinates
    bool is_consistent() const;  // if coords are consistent: left <= right (but -1)

    template<class T>
    friend class Matrix_proxy;
public:
    Matrix_coords();    // default ({0, 0}, {-1, -1})
    Matrix_coords(const coords& left);  // call Matrix_coords(left, left)
    Matrix_coords(const coords& left, const coords& right);
    Matrix_coords(const Matrix_coords& other);
    Matrix_coords(Matrix_coords&& other);
    Matrix_coords& operator=(const Matrix_coords& other);
    Matrix_coords& operator=(Matrix_coords&& other);

    std::string to_string() const;
    friend std::ostream& operator<<(std::ostream &os, const Matrix_coords& coords);

    // check if dot belongs to Matrix_coords
    bool has(const coords& dot) const;

    int get_right_x() const;
    int get_right_y() const;
    int get_left_x() const;
    int get_left_y() const;
};

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

class Matrix_row_coord{
private:
    int row_index;
public:
    Matrix_row_coord(); // 0
    explicit Matrix_row_coord(int index);

    int get_row_index() const;

    bool has(const coords& dot) const;
};

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

class Matrix_column_coord{
private:
    int column_index;
public:
    Matrix_column_coord(); // 0
    explicit Matrix_column_coord(int index);

    int get_column_index() const;

    bool has(const coords& dot) const;
};

#endif // __ClassMatrixCoords_H__