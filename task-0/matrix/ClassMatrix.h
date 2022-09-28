#ifndef __ClassMatrix_H__
#define __ClassMatrix_H__

#include <unordered_map>
#include <ostream>
#include <string>
#include <utility>
#include <cmath>

struct pair_hash{
    template <class T1, class T2>
    std::size_t operator() (const std::pair<T1, T2>& pair) const {
        return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
    }
};

using coords = std::pair<int, int>;

template<class T>
using matr_vals = std::unordered_map<coords, T, pair_hash>;

template<class T>
class Matrix{
private:
    int rows;
    int columns;
    double eps;
    //T base_val;
    matr_vals<T> values;
    void _clear_fake_vals();    // operator() creates members of unordered_set if key is missing
    bool same_shape(const Matrix& other) const;
    matr_vals<T> key_union(const Matrix& other) const;
public:
    // unar matrix
    Matrix(int _rows, int _columns, double _eps = 0.001, 
           bool unar = false, bool fill_one = false);
    Matrix(int _rows, int _columns, const matr_vals<T>&  _values, double _eps = 0.001);
    Matrix(const Matrix& other);
    Matrix(Matrix&& other);

    Matrix& operator=(const Matrix& other);
    Matrix& operator=(Matrix&& other);
    Matrix operator+(Matrix& other); // return type of left operand
    Matrix operator-(Matrix& other); // return type of left operand
    Matrix operator*(Matrix& other); // return type of left operand
    Matrix operator-();   //unar -
    Matrix operator~();   // transposion

    T& operator()(int i, int j);
    int size();   // number of elems in values
    std::string to_string();
};

// Constructors
//////////////////////////////////

template<class T>
Matrix<T>::Matrix(int _rows, int _columns, double _eps, bool unar, bool fill_one):
    rows(_rows), columns(_columns), eps(_eps){
    if (eps >= 1 && (unar || fill_one) ) throw 7;  // TODO: special exception;
    if (unar && fill_one) throw 8;                 // TODO: special exception;
    if (unar)
        for (int i = 0; i < std::min(rows, columns); i++)
            values[{i, i}] = T(1);
    if (fill_one)
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < columns; j++)
                values[{i, j}] = T(1);
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

//////////////////////////////////

// operators
//////////////////////////////////

// operator() creates members of unordered_set if key is missing.
// We need to return reference to any value (even if missing) since we can't 
// predict if we read or set an element, so we sometimes create fake elements.
// They are cleared internally with call of some methods.
template<class T>
T& Matrix<T>::operator()(int i, int j){
    if (!(i < rows && j < columns )) throw 5;    // TODO: make special exception
    return values[{i, j}];
    /*
    auto res = values.find({i, j});
    if (res == values.end()) {
        return T(0);
    } else {
        return res->second;
    }
    */
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
    res._clear_fake_vals();
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
    res._clear_fake_vals();
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
    res._clear_fake_vals();
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
    _clear_fake_vals();
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
    _clear_fake_vals();
    std::string res("matrix complex ");
    res = res + std::to_string(rows) + " " + std::to_string(columns) + "\n";
    for (const auto& elem: values){
        res = res + "\n" + std::to_string(elem.first.first) + " " +
              std::to_string(elem.first.second) + "   " + elem.second.to_string();
    }
    return res;
}

//////////////////////////////////

#endif // __ClassMatrix_H__
