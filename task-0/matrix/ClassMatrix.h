#ifndef __ClassMatrix_H__
#define __ClassMatrix_H__

#include <unordered_map>
#include <unordered_set>
#include <map>
#include <ostream>
#include <string>
#include <utility>
#include <cmath>

#include "Matrix_coords.h"
#include "Matrix_proxy.hpp"

#ifndef __Matr_vals__
#define __Matr_vals__
struct pair_hash{
    template <class T1, class T2>
    std::size_t operator() (const std::pair<T1, T2>& pair) const {
        return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
    }
};

using coords = std::pair<int, int>;

template<class T>
using matr_vals = std::unordered_map<coords, T, pair_hash>;
#endif  //__Matr_vals__

/**
 * @brief Class for sparse matrices.
 * 
 * Has eps parameter: all values less than eps are considered zero.
 * There is an opportunity to make slices of matrix.
 * Possible operations: +, -, *, unar -, ^ is transposing.
 * Matrix can be parsed out of file and written to file.
 * 
 * @tparam T - type of matrix's elements (designed for standard types, Rational_number, Complex_number)
 */
template<class T>
class Matrix{
private:
    int rows;
    int columns;
    double eps;
    matr_vals<T> values;

    friend class Matrix_proxy<T>;
    std::unordered_set<Matrix_proxy<T>*> proxies; // all related proxies for a certain matrix
    void add_proxy(Matrix_proxy<T>* proxy);
    void remove_proxy(Matrix_proxy<T>* proxy);

    void _clear_fake_vals();    // operator() creates members of unordered_set if key is missing
    bool same_shape(const Matrix& other) const;
    matr_vals<T> key_union(const Matrix& other) const;
public:
    Matrix(int _rows, int _columns, double _eps = 0.001, 
           bool unar = false, bool fill_one = false);
    Matrix(int _rows, int _columns, const matr_vals<T>&  _values, double _eps = 0.001);
    Matrix(const Matrix& other);
    Matrix(Matrix&& other);
    ~Matrix();

    // Constructor from filename (todo: parser)
    explicit Matrix(const char* file_path);

    // Constructor from proxy
    Matrix(const Matrix_proxy<T>& proxy);

    Matrix& operator=(const Matrix& other);
    Matrix& operator=(Matrix&& other);
    Matrix operator+(Matrix& other); // return type of left operand
    Matrix operator-(Matrix& other); // return type of left operand
    Matrix operator*(Matrix& other); // return type of left operand
    Matrix operator-();   //unar -
    Matrix operator~();   // transposion

    T& operator()(int i, int j);
    T& operator()(const coords& pos);
    Matrix_proxy<T> operator[](const Matrix_coords& coords);
    Matrix_proxy<T> operator[](const Matrix_row_coord& row);
    Matrix_proxy<T> operator[](const Matrix_column_coord& row);
    int size();   // number of elems in values
    std::string to_string();
    void set_eps(double new_eps);

    matr_vals<T> get_submatrix_vals(const Matrix_coords& range);    // for matrix
    std::map<int, T> get_row_vals(int idx);   // for vector
    std::map<int, T> get_column_vals(int idx);    // for vector
};

// Constructors and destructors
//////////////////////////////////

template<class T>
Matrix<T>::Matrix(int _rows, int _columns, double _eps, bool unar, bool fill_one):
    rows(_rows), columns(_columns), eps(_eps){
    if (eps >= 1 && (unar || fill_one) ) throw 7;  // TODO: special exception;
    if (unar && fill_one) throw 8;                 // TODO: special exception;
    if (unar)
        for (int i = 0; i < std::min(rows, columns); i++)
            values[{i, i}] = T(1);
    if (fill_one){
        values.reserve( rows * columns );
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < columns; j++)
                values[{i, j}] = std::move(T(1));
    }
};


template<class T>
Matrix<T>::Matrix(int _rows, int _columns, const matr_vals<T>&  _values, double _eps):
    rows(_rows), columns(_columns), eps(_eps){
    for (const auto& elem : _values){
        coords tmp = elem.first;
        if (!(tmp.first < rows && tmp.second < columns)) throw 5;   // TODO: make special exception
        if (!(abs(elem.second) < eps)){
            values[elem.first] = elem.second;
        }
    }
}

template<>
Matrix<Complex_number<>>::Matrix(int _rows, int _columns, const matr_vals<Complex_number<>>&  _values, double _eps):
    rows(_rows), columns(_columns), eps(_eps){
    for (const auto& elem : _values){
        coords tmp = elem.first;
        if (!(tmp.first < rows && tmp.second < columns)) throw 5;   // TODO: make special exception
        if (!(elem.second.module_square() < eps * eps)){
            values[elem.first] = elem.second;
        }
    }
}

template<class T>
Matrix<T>::Matrix(const Matrix& other){
    rows = other.rows;
    columns = other.columns;
    eps = other.eps;
    values = other.values;
}

template<class T>
Matrix<T>::Matrix(Matrix&& other){
    rows = std::move(other.rows);
    columns = std::move(other.columns);
    eps = std::move(other.eps);
    std::swap(values, other.values);
}

template<class T>
Matrix<T>::Matrix(const Matrix_proxy<T>& proxy){
    std::pair<int, int> dims = proxy.get_dim();
    rows = dims.first;
    columns = dims.second;
    eps = proxy.get_eps();
    values = proxy.get_values_as_hash_map();
}

template<class T>
Matrix<T>::~Matrix(){
    for (auto proxy: proxies) {
        remove_proxy(proxy);
    }
}

//////////////////////////////////

// operators
//////////////////////////////////

// operator() creates members of unordered_set if key is missing.
// We need to return reference to any value (even if missing) since we can't 
// predict if we read or write an element, so we sometimes create fake elements.
// They are cleared internally with call of some methods.
template<class T>
T& Matrix<T>::operator()(int i, int j){
    if (!(i < rows && j < columns )) throw 5;    // TODO: make special exception
    return values[{i, j}];
}

//call operator(pos.first, pos.second)
template<class T>
T& Matrix<T>::operator()(const coords& pos){
    return this->operator()(pos.first, pos.second);
}

template<class T>
Matrix<T>& Matrix<T>::operator=(const Matrix& other){
    if (!same_shape(other)) throw 6;    // TODO: special exception
    eps = other.eps;
    values = other.values;
    return *this;
}

template<class T>
Matrix<T>& Matrix<T>::operator=(Matrix&& other){
    if (!same_shape(other)) throw 6;    // TODO: special exception
    eps = std::move(other.eps);
    values = std::move(other.values);
    return *this;
}

template<class T>
Matrix<T> Matrix<T>::operator+(Matrix& other){
    if (!same_shape(other)) throw 6;   // TODO: special exception
    decltype(values) tmp_vals = key_union(other);
    for (const auto& elem : tmp_vals){
        tmp_vals[elem.first] = values[elem.first] + other.values[elem.first];
    }
    Matrix<T> res(rows, columns, tmp_vals);

    _clear_fake_vals();
    other._clear_fake_vals();
    return res;
}

template<class T>
Matrix<T> Matrix<T>::operator-(Matrix& other){
    if (!same_shape(other)) throw 6;   // TODO: special exception
    decltype(values) tmp_vals = key_union(other);
    for (const auto& elem : tmp_vals){
        tmp_vals[elem.first] = values[elem.first] - other.values[elem.first];
    }
    Matrix<T> res(rows, columns, tmp_vals);

    _clear_fake_vals();
    other._clear_fake_vals();
    return res;
}

template<class T>
Matrix<T> Matrix<T>::operator*(Matrix& other){
    if (columns != other.rows) throw 7;     // TODO: special exception
    decltype(values) tmp_vals;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < other.columns; j++) {
            T tmp((long) 0), tmp1((long) 0);
            for (int k = 0; k < columns; k++)
                if (values.find({i, k}) != values.end() && 
                    other.values.find({k, j}) != other.values.end()){
                    tmp += values[{i, k}] * other.values[{k, j}];
                }
            if (tmp != tmp1) tmp_vals[{i, j}] = tmp;    // if changed
        }
    }
    
    Matrix<T> res(rows, other.columns, tmp_vals, eps);

    _clear_fake_vals();
    other._clear_fake_vals();
    return res;
}

// unar -
template<class T>
Matrix<T> Matrix<T>::operator-(){
    Matrix<T> copy(*this);
    for(auto& elem : copy.values){
        elem.second = -elem.second;
    }
    return copy;
}

// transposion
template<class T>
Matrix<T> Matrix<T>::operator~(){
    Matrix<T> copy(*this);
    for(auto& elem : copy.values){
        coords tmp_coords = elem.first;
        if (copy.values.find({tmp_coords.second, tmp_coords.first}) == copy.values.end()){
            copy.values[{tmp_coords.second, tmp_coords.first}] = elem.second;
            copy.values.erase(tmp_coords);
        } else if (tmp_coords.first < tmp_coords.second) {      // for 1 swap only
            T tmp_val = copy.values[{tmp_coords.second, tmp_coords.first}];
            copy.values[{tmp_coords.second, tmp_coords.first}] = copy.values[tmp_coords];
            copy.values[tmp_coords] = tmp_val;
        }
    }
    return copy;
}

template<class T>
Matrix_proxy<T> Matrix<T>::operator[](const Matrix_coords& coords){
    if (coords.has({rows, columns})){
        throw 3;       // TODO: exceptions out of range
    }
    return Matrix_proxy<T>(*this, coords);
}

template<class T>
Matrix_proxy<T> Matrix<T>::operator[](const Matrix_row_coord& row) {
    if (row.get_row_index() < 0 || rows <= row.get_row_index()){
        throw 3;    //OutOfRangeException();
    }
    return Matrix_proxy<T>(*this, row);
}

template<class T>
Matrix_proxy<T> Matrix<T>::operator[](const Matrix_column_coord& column) {
    if (column.get_column_index() < 0 || columns <= column.get_column_index()){
        throw 3;    // OutOfRangeException();
    }
    return Matrix_proxy<T>(*this, column);
}

//////////////////////////////////


// Methods
//////////////////////////////////

template<class T>
bool Matrix<T>::same_shape(const Matrix& other) const{
    return (rows == other.rows && columns == other.columns);
}

// union of keys, values are "random"
template<class T>
matr_vals<T> Matrix<T>::key_union(const Matrix& other) const{
    matr_vals<T> tmp_vals = values;
    for (const auto& elem: other.values){
        tmp_vals[elem.first] = elem.second;
    }
    return tmp_vals;
}

// number of elems in values
template<class T>
int Matrix<T>::size(){
    _clear_fake_vals();
    return values.size();
}

// operator() creates members of unordered_set if key is missing.
// We need to return reference to any value (even if missing) since we can't 
// predict if we read or set an element, so we sometimes create fake elements.
// This function removes them.
template<class T>
void Matrix<T>::_clear_fake_vals(){
    for(const auto& elem: values){
        //std::cout << "here: " << elem.second << " " << eps << " " << (abd(elem.second) < eps) <<std::endl;
        if (abs(elem.second) < eps)
            values.erase(elem.first);
    }
}

template<>
void Matrix<Complex_number<>>::_clear_fake_vals(){
    for(const auto& elem: values){
        if (elem.second.module_square() < eps * eps)
            values.erase(elem.first);
    }
}


template<>
void Matrix<Rational_number>::_clear_fake_vals(){
    Rational_number rat_eps = Rational_number::from_double(eps);
    for(const auto& elem: values){
        if (abs(elem.second) < rat_eps)
            values.erase(elem.first);
    }
}

template<class T>
matr_vals<T> Matrix<T>::get_submatrix_vals(const Matrix_coords& range){
    //_clear_fake_vals();
    matr_vals<T> res_vals;
        for (const auto& elem: values) {
            if (range.has(elem.first)) {
                res_vals.insert(elem);
            }
        }
    return res_vals;
}

template<class T>
std::map<int, T> Matrix<T>::get_row_vals(int idx){
    //_clear_fake_vals();
    Matrix_row_coord range(idx);
    std::map<coords, T> res_vals;
        for (const auto& elem: values) {
            if (range.has(elem.first.first)) {      // elem.first.first - X coord
                res_vals.insert({elem.first.first, elem.second});
            }
        }
    return res_vals;
}

template<class T>
std::map<int, T> Matrix<T>::get_column_vals(int idx){
    //_clear_fake_vals();
    Matrix_column_coord range(idx);
    std::unordered_map<coords, T> res_vals;
        for (const auto& elem: values) {
            if (range.has(elem.first.second)) {     // elem.first.second - Y coord
                res_vals.insert({elem.first.second, elem.second});
            }
        }
    return res_vals;
}


template<class T>
std::string Matrix<T>::to_string(){
    _clear_fake_vals();
    std::string res("matrix ");
    res = res + typeid(T).name() + " " + std::to_string(rows) + 
          " " + std::to_string(columns) + "\n";
    for (const auto& elem: values){
        res = res + "\n" + std::to_string(elem.first.first) + " " + 
              std::to_string(elem.first.second) + "   " + std::to_string(elem.second);
    }
    return res;
}

template<>
std::string Matrix<Rational_number>::to_string(){
    std::string res("matrix rational ");
    res = res + std::to_string(rows) + " " + std::to_string(columns) + "\n";
    for (const auto& elem: values){
        res = res + "\n" + std::to_string(elem.first.first) + " " +
              std::to_string(elem.first.second) + "   " + elem.second.to_string();
    }
    return res;
}

template<>
std::string Matrix<Complex_number<>>::to_string(){
    std::string res("matrix complex ");
    res = res + std::to_string(rows) + " " + std::to_string(columns) + "\n";
    for (const auto& elem: values){
        res = res + "\n" + std::to_string(elem.first.first) + " " +
              std::to_string(elem.first.second) + "   " + elem.second.to_string();
    }
    return res;
}

template<class T>
void Matrix<T>::add_proxy(Matrix_proxy<T>* proxy){
    proxies.insert(proxy);
}

template<class T>
void Matrix<T>::remove_proxy(Matrix_proxy<T>* proxy) {
    proxy->unlink();
    proxies.erase(proxy);
}

template<class T>
void Matrix<T>::set_eps(double new_eps){
    eps = new_eps;
    _clear_fake_vals();
}

//////////////////////////////////

#endif // __ClassMatrix_H__
