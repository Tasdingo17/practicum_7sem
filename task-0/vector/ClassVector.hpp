#ifndef __ClassVector_H__
#define __ClassVector_H__

#include<map>
#include<set>
#include<string>

#include"../rational/ClassRationalNumber.h"
#include"../complex/ClassComplex.h"
#include"../matrix/ClassMatrix.h"

template<class T>
using vect_vals = std::map<int, T>;

template<class T>
class Vector{
private:
    int max_size;
    constexpr static double eps = 0.01;
    vect_vals<T> values;
    void _clear_fake_vals();    // operator() creates members of unordered_set if key is missing
    bool same_shape(const Vector& other) const;
    vect_vals<T> key_union(const Vector& other) const;
    std::ofstream _open_write_file(const char* filename, bool append = false) const;
public:
    Vector(int _max_size, bool fill_one = false);
    Vector(int _max_size, const vect_vals<T>&  _values);
    Vector(const Vector& other);
    Vector(Vector&& other);

    explicit Vector(const Matrix_proxy<T>& proxy);

    // Constructor from filename (todo: parser)
    explicit Vector(const char* file_path);

    T& operator()(int i);
    Vector& operator=(const Vector& other);
    Vector& operator=(Vector&& other);
    Vector operator+(Vector& other); // return type of left operand
    Vector operator-(Vector& other); // return type of left operand
    Vector operator-();   //unar -

    bool operator==(const Vector& other);       // todo?
    bool operator!=(const Vector& other);       // todo?

    template<typename TValueLeft, typename TValueRight>
    friend Vector<TValueLeft> operator+(Vector<TValueLeft> lhs, const TValueRight& rhs);

    template<typename TValueLeft, typename TValueRight>
    friend Vector<TValueLeft> operator-(Vector<TValueLeft> lhs, const TValueRight& rhs);

    template<typename TValueLeft, typename TValueRight>
    friend Vector<TValueLeft> operator*(Vector<TValueLeft> lhs, const TValueRight& rhs);

    template<typename TValueLeft, typename TValueRight>
    friend Vector<TValueLeft> operator/(Vector<TValueLeft> lhs, const TValueRight& rhs);

    std::string to_string();
    static void set_eps(double new_eps);
    static double get_eps();

    int get_max_size() const;

    //non-zero elements
    int get_size();

    void to_file(const char* filename, bool append = false);       // TODO!
};

// Constructors and destructors
//////////////////////////////////

template<class T>
Vector<T>::Vector(int _max_size, bool fill_one):
    max_size(_max_size){
    if (fill_one){
        for (int i = 0; i < max_size; i++)
            values[i] = std::move(T(1));
    }
}

template<class T>
Vector<T>::Vector(int _max_size, const vect_vals<T>&  _values):
    max_size(_max_size){
    for (const auto& elem : _values){
        int pos = elem.first;
        if (!(pos < max_size)) throw 5;   // TODO: make special exception
        if (!(abs(elem.second) < eps)){
            values[pos] = elem.second;
        }
    }
}

template<>
Vector<Complex_number<>>::Vector(int _max_size, const vect_vals<Complex_number<>>&  _values):
    max_size(_max_size){
    for (const auto& elem : _values){
        int pos = elem.first;
        if (!(pos < max_size)) throw 5;   // TODO: make special exception
        if (!(elem.second.module_square() < eps * eps)){
            values[elem.first] = elem.second;
        }
    }
}

template<class T>
Vector<T>::Vector(const Vector& other){
    max_size = other.max_size;
    values = other.values;
}

template<class T>
Vector<T>::Vector(Vector&& other){
    max_size = std::move(other.max_size);
    values = std::move(other.values);
}

template<class T>
Vector<T>::Vector(const char* file_path){
    throw 123;  // todo: not generally specialized
}

template<>
Vector<Rational_number>::Vector(const char* file_path){
    Parser parser;
    parser.parse_vector(file_path);
    if (parser.get_type() != "rational"){
        throw 9;    // todo: type mismatch exceptions
    }
    max_size = parser.get_max_size();

    for (const auto& elem : parser.get_vector_vals()){
        int pos = elem.first;
        pos -= 1;     // in file numeration from 1
        if (!(pos < max_size)) throw 5;   // TODO: make special exception
        Rational_number val(elem.second.first, elem.second.second);
        if (!(abs(val) < eps)){
            values[pos] = val;
        }
    }
}

template<>
Vector<Complex_number<>>::Vector(const char* file_path){
    Parser parser;
    parser.parse_vector(file_path);
    if (parser.get_type() != "complex"){
        throw 9;    // todo: type mismatch exceptions
    }
    max_size = parser.get_max_size();

    for (const auto& elem : parser.get_vector_vals()){
        int pos = elem.first;
        pos -= 1;   // in file numeration from 1
        if (!(pos < max_size)) throw 5;   // TODO: make special exception
        Complex_number<> val(std::stod(elem.second.first), std::stod(elem.second.second));
        if (!(val.module_square() < eps * eps)){
            values[pos] = val;
        }
    }
}

//////////////////////////////////

// operators
//////////////////////////////////

// operator() creates members of map if key is missing.
// We need to return reference to any value (even if missing) since we can't 
// predict if we read or write an element, so we sometimes create fake elements.
// They are cleared internally with call of some methods.
template<class T>
T& Vector<T>::operator()(int i){
    if (!(0<= i < max_size)) throw 5;    // TODO: make special exception
    return values[i];
}

template<class T>
Vector<T>& Vector<T>::operator=(const Vector& other){
    if (!same_shape(other)) throw 6;    // TODO: special exception
    values = other.values;
    return *this;
}

template<class T>
Vector<T>& Vector<T>::operator=(Vector&& other){
    if (!same_shape(other)) throw 6;    // TODO: special exception
    values = std::move(other.values);
    return *this;
}

// return type of left operand
template<class T>
Vector<T> Vector<T>::operator+(Vector& other){
    if (!same_shape(other)) throw 6;   // TODO: special exception
    decltype(values) tmp_vals = key_union(other);
    for (const auto& elem : tmp_vals){
        tmp_vals[elem.first] = values[elem.first] + other.values[elem.first];
    }
    Vector<T> res(max_size, tmp_vals);

    _clear_fake_vals();
    other._clear_fake_vals();
    return res;
}

// return type of left operand
template<class T>
Vector<T> Vector<T>::operator-(Vector& other){
    if (!same_shape(other)) throw 6;   // TODO: special exception
    decltype(values) tmp_vals = key_union(other);
    for (const auto& elem : tmp_vals){
        tmp_vals[elem.first] = values[elem.first] - other.values[elem.first];
    }
    Vector<T> res(max_size, tmp_vals);

    _clear_fake_vals();
    other._clear_fake_vals();
    return res;
}

// unar -
template<class T>
Vector<T> Vector<T>::operator-(){
    Vector<T> copy(*this);
    for(auto& elem : copy.values){
        elem.second = -elem.second;
    }
    return copy;
}

template<typename TValueLeft, typename TValueRight>
Vector<TValueLeft> operator+(Vector<TValueLeft> lhs, const TValueRight& rhs){
    Vector<TValueLeft> res(lhs);
    for (int i = 0; i < res.max_size; i ++) {
        res.values[i] += rhs;
    }

    res._clear_fake_vals();
    return res;
}

template<typename TValueLeft, typename TValueRight>
Vector<TValueLeft> operator-(Vector<TValueLeft> lhs, const TValueRight& rhs){
    Vector<TValueLeft> res(lhs);
    for (int i = 0; i < res.max_size; i ++) {
        res.values[i] -= rhs;
    }
    res._clear_fake_vals();
    return res;
}

template<typename TValueLeft, typename TValueRight>
Vector<TValueLeft> operator*(Vector<TValueLeft> lhs, const TValueRight& rhs){
    if (rhs == TValueRight((int) 0)){
        return Vector<TValueLeft>(lhs);
    }
    Vector<TValueLeft> res(lhs);
    for (auto& elem : res.values) {
        elem.second *= rhs;
    }

    res._clear_fake_vals();
    return res;
}

template<typename TValueLeft, typename TValueRight>
Vector<TValueLeft> operator/(Vector<TValueLeft> lhs, const TValueRight& rhs){
    if (rhs == TValueRight((int) 0)) throw 6; // todo: ZeroDivision
    Vector<TValueLeft> res(lhs);
    for (auto& elem : res.values) {
        elem.second /= rhs;
    }
    return lhs;
}

//////////////////////////////////

// Methods
//////////////////////////////////

// operator() creates members of map if key is missing.
// We need to return reference to any value (even if missing) since we can't 
// predict if we read or set an element, so we sometimes create fake elements.
// This function removes them.
template<class T>
void Vector<T>::_clear_fake_vals(){
    std::set<int> to_delete;
    for(auto it = values.begin(); it != values.end(); it++){
        if (abs(it->second) < eps)
            to_delete.insert(it->first);
    }

    for (auto& key : to_delete) {
        values.erase(key);
    }
}

template<>
void Vector<Complex_number<>>::_clear_fake_vals(){
    std::set<int> to_delete;
    for(auto it = values.begin(); it != values.end(); it++){
        if (it->second.module_square() < eps * eps)
            to_delete.insert(it->first);
    }

    for (auto& key : to_delete) {
        values.erase(key);
    }
}

template<>
void Vector<Rational_number>::_clear_fake_vals(){
    Rational_number rat_eps = Rational_number::from_double(eps);
    std::set<int> to_delete;
    for(auto it = values.begin(); it != values.end(); it++){
        if (abs(it->second) < rat_eps){
            to_delete.insert(it->first);
        }
    }
    for (auto& key : to_delete) {
        values.erase(key);
    }
}

template<class T>
std::string Vector<T>::to_string(){
    _clear_fake_vals();
    std::string res("vector ");
    res = res + typeid(T).name() + " " + std::to_string(max_size) + "\n";
    for (const auto& elem: values){
        res = res + "\n" + std::to_string(elem.first) + " " + std::to_string(elem.second);
    }
    return res;
}

template<>
std::string Vector<Rational_number>::to_string(){
    _clear_fake_vals();
    std::string res("vector rational ");
    res = res + std::to_string(max_size) + "\n";
    for (const auto& elem: values){
        res = res + "\n" + std::to_string(elem.first) + " " + elem.second.to_string();
    }
    return res;
}

template<>
std::string Vector<Complex_number<>>::to_string(){
    _clear_fake_vals();
    std::string res("matrix complex ");
    res = res + std::to_string(max_size) + "\n";
    for (const auto& elem: values){
        res = res + "\n" + std::to_string(elem.first) + " " + elem.second.to_string();
    }
    return res;
}

template<class T>
int Vector<T>::get_max_size() const{
    return max_size;
}

template<class T>
double Vector<T>::get_eps(){
    return eps;
}

template<class T>
int Vector<T>::get_size(){
    _clear_fake_vals();
    return values.size();
}

template<class T>
void Vector<T>::set_eps(double new_eps){
    eps = new_eps;
}

template<class T>
bool Vector<T>::same_shape(const Vector& other) const{
    return max_size == other.max_size;
}

// union of keys, values are "random"
template<class T>
vect_vals<T> Vector<T>::key_union(const Vector& other) const{
    vect_vals<T> tmp_vals = values;
    for (const auto& elem: other.values){
        tmp_vals[elem.first] = elem.second;
    }
    return tmp_vals;
}

template<class T>
std::ofstream Vector<T>::_open_write_file(const char* filename, bool append) const{
    std::ofstream file_data;
    if (append){
        file_data.open(filename, std::ios_base::app);
    } else {
        file_data.open(filename);
    }
    if (!file_data.is_open()){
        throw 10;   // todo:exceptions (!note: not only non-exsisting also permission failures)
    }
    return file_data;
}

template<class T>
void Vector<T>::to_file(const char* filename, bool append){
    _clear_fake_vals();
    auto file_data = _open_write_file(filename, append);

    std::string res("vector ");
    res = res + typeid(T).name() + " " + std::to_string(max_size) + "\n";
    for (const auto& elem: values){
        res = res + "\n" + std::to_string(elem.first + 1) + "  " + std::to_string(elem.second);
    }

    file_data << res;
    if (file_data.fail()) throw 12; //todo: exceptions
    file_data.close();
}

template<>
void Vector<Rational_number>::to_file(const char* filename, bool append){
    _clear_fake_vals();
    auto file_data = _open_write_file(filename, append);

    std::string res("vector rational ");
    res = res + std::to_string(max_size) + "\n";
    for (const auto& elem: values){
        res = res + "\n" + std::to_string(elem.first + 1) + "  " + elem.second.to_string();
    }

    file_data << res;
    if (file_data.fail()) throw 12; //todo: exceptions
    file_data.close();
}

template<>
void Vector<Complex_number<>>::to_file(const char* filename, bool append){
    _clear_fake_vals();
    auto file_data = _open_write_file(filename, append);

    std::string res("vector complex ");
    res = res + std::to_string(max_size) + "\n";
    for (const auto& elem: values){
        res = res + "\n" + std::to_string(elem.first + 1) + "  " + elem.second.to_string();
    }

    file_data << res;
    if (file_data.fail()) throw 12; //todo: exceptions
    file_data.close();
}


//////////////////////////////////

#endif  //__ClassVector_H__