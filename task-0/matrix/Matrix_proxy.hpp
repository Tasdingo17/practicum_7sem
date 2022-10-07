#ifndef __ClassMatrixProxy_H__
#define __ClassMatrixProxy_H__

#include "Matrix_coords.h"

template<class T>
class Matrix;

template<class T>
class Matrix_proxy{
private:
    Matrix_coords m_coords;
    Matrix<T>* matr_ptr;

    bool is_in_bounds(int i, int j) const{      // true if (i, j) is in slice
        bool wrong_cond1 = i < 0 || j < 0;
        bool wrong_cond2 = (m_coords.left_x <= i <= m_coords.right_x ||
                            m_coords.left_y <= j <= m_coords.right_y);
        return !(wrong_cond1 || wrong_cond2);
    }
public:
    Matrix_proxy(const Matrix<T>& _matr, const Matrix_coords& _m_coords){
        std::cout << "In Matrix proxy constr!" << std::endl;
        matr_ptr = &_matr;
        m_coords = _m_coords;
        if (m_coords.left_x == -1) m_coords.left_x = 0;
        if (m_coords.right_x == -1) m_coords.right_x = _matr.columns - 1;
        if (m_coords.left_y == -1) m_coords.left_x = 0;
        if (m_coords.right_y == -1) m_coords.right_y = _matr.rows - 1;
    }

    T& operator[](coords elem){
        if(!is_in_bounds(elem.first, elem.second)) throw 2;    // TODO: exceptions
        return matr_ptr->values[elem];
    }
};

#endif //__ClassMatrixProxy_H__
