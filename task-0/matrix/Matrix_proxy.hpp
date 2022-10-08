#ifndef __ClassMatrixProxy_H__
#define __ClassMatrixProxy_H__

#include <memory>
#include <map>
#include <unordered_map>
#include "Matrix_coords.h"

template<class T>
class Matrix;


enum class Matrix_proxy_type {
    ROW,
    COLUMN,
    RECTANGLE,
};


template<class T>
class Matrix_proxy{
private:
    Matrix_proxy_type type;
    Matrix_coords m_coords;
    std::unique_ptr<Matrix<T>> matr_ptr;

    bool is_in_bounds(int i, int j) const{      // true if (i, j) is in slice
        bool wrong_cond1 = i < 0 || j < 0;
        bool wrong_cond2 = (m_coords.left_x <= i <= m_coords.right_x ||
                            m_coords.left_y <= j <= m_coords.right_y);
        return !(wrong_cond1 || wrong_cond2);
    }
public:
    Matrix_proxy(const Matrix<T>& _matr, const Matrix_coords& _m_coords){
        type = Matrix_proxy_type::RECTANGLE;
        m_coords = _m_coords;
        matr_ptr = std::make_unique<Matrix<T>>(_matr);
        matr_ptr->add_proxy(this);

        if (m_coords.left_x == -1) m_coords.left_x = 0;
        if (m_coords.right_x == -1) m_coords.right_x = _matr.columns - 1;
        if (m_coords.left_y == -1) m_coords.left_x = 0;
        if (m_coords.right_y == -1) m_coords.right_y = _matr.rows - 1;
        // TODO: check for consistency
    }

    Matrix_proxy(const Matrix<T>& _matr, const Matrix_column_coord& coords){
        type = Matrix_proxy_type::COLUMN;
        matr_ptr = std::make_unique<Matrix<T>>(_matr);
        matr_ptr->add_proxy(this);
        m_coords = Matrix_coords({0, coords.get_column_index()}, {_matr.rows - 1, coords.get_column_index()});
        // TODO: check for consistency
    }

    Matrix_proxy(const Matrix<T>& _matr, const Matrix_row_coord& coords){
        type = Matrix_proxy_type::ROW;
        matr_ptr = std::make_unique<Matrix<T>>(_matr);
        matr_ptr->add_proxy(this);
        m_coords = Matrix_coords({coords.get_row_index(), 0}, {coords.get_row_index(), _matr.columns - 1});
        // TODO: check for consistency
    }

    Matrix_proxy(const Matrix_proxy& other) = delete;
    Matrix_proxy& operator=(const Matrix_proxy& other) = delete;

    // Get size of slice
    std::pair<int, int> get_dim() const{
        if (matr_ptr.get() == nullptr) throw 1;     // TODO: exceptions
        int rows = m_coords.right_x - m_coords.left_x + 1;
        int columns = m_coords.right_y - m_coords.left_y + 1;
        return {rows, columns};
    }

    double get_eps() const{
        if (matr_ptr.get() == nullptr) throw 1;     // TODO: exceptions
        return matr_ptr->eps;
    }

    // return row number if proxy type is ROW
    int get_row_coord() const{
        if (type != Matrix_proxy_type::ROW) throw 1;    // TODO:: exceptions
        return m_coords.left_x;
    }

    // return column number if proxy type is COLUMN
    int get_column_coord() const{
        if (type != Matrix_proxy_type::COLUMN) throw 2; // TODO: exceptions
        return m_coords.left_y;
    }

    T& operator()(const coords& elem) {
        if (matr_ptr.get() == nullptr) throw 1;     // TODO:exceptions
        if(!is_in_bounds(elem.first, elem.second)) throw 2;
        if (type == Matrix_proxy_type::RECTANGLE) {
            return matr_ptr->operator()(elem.first, elem.second);
        } else {
            throw 3;  //IllegalStateException("Cannot get element of a row/column slice by two coordinates.");
        }
    }

    // calls operator()({i, j})
    T& operator()(int& i, int j) {
        return operator()({i, j});
    }
        
    T& operator()(int idx) {
        if (matr_ptr.get() == nullptr) throw 1;     // TODO: exceptions
        switch (type) {
            case Matrix_proxy_type::ROW:
                return matr_ptr->operator()({get_row_coord(), idx});
            case Matrix_proxy_type::COLUMN:
                return matr_ptr->operator()({idx, get_column_coord()});
            case Matrix_proxy_type::RECTANGLE:
                throw 3; //IllegalStateException("Cannot get element of a rectangle slice by single coordinate.");
        }
    }

    // values of current slice as std::unordered_map where coords is a key.
    // vector version
    std::map<int, T> get_values_as_map() const {
        if (matr_ptr.get() == nullptr) throw 3; // TODO:exceptions
        switch (type) {
            case Matrix_proxy_type::ROW:
                return matr_ptr->get_row_vals(get_row_coord());
            case Matrix_proxy_type::COLUMN:
                return matr_ptr->get_column_vals(get_column_coord());
            case Matrix_proxy_type::RECTANGLE:
                throw 1;        //IllegalStateException("Cannot get rectangle slice values as std::map");
        }
    }


    // values of current slice as std::unordered_map where coords is a key.
    // matrix version
    std::unordered_map<coords, T> get_values_as_hash_map() const {
        if (matr_ptr.get() == nullptr) throw 3;   // TODO exceptions
        return matr_ptr->get_submatrix_vals(m_coords);
    }

    void unlink() {
        matr_ptr.release();
    }

    ~Matrix_proxy() {
        if (matr_ptr.get() != nullptr) {
            matr_ptr->remove_proxy(this);
        }
    }
};

#endif //__ClassMatrixProxy_H__
