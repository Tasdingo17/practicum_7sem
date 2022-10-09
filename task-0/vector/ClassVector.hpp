#ifndef __ClassVector_H__
#define __ClassVector_H__

#include<map>
#include<string>

template<class T>
using vect_vals = std::map<int, T>;

template<class T>
class Vector{
private:
    int max_size;
    constexpr static double eps = 0.01;
    vect_vals<T> values;
    void _clear_fake_vals();    // operator() creates members of unordered_set if key is missing
public:
    Vector(int _max_size, bool fill_one = false);
    Vector(int _max_size, const vect_vals<T>&  _values);
    Vector(const Vector& other);
    Vector(Vector&& other);

    // Constructor from filename (todo: parser)
    explicit Vector(const char* file_path);

    std::string to_string();
    static void set_eps(double new_eps);
    static double get_eps();

    int get_max_size() const;

    //non-zero elements
    int get_size();

    void to_file(const char* filename);       // TODO!
};

// Constructors and destructors
//////////////////////////////////

template<class T>
Vector<T>::Vector(int _max_size, bool fill_one):
    max_size(_max_size){
    if (fill_one){
        values.reserve( max_size );
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

//////////////////////////////////

// operators
//////////////////////////////////


//////////////////////////////////

// Methods
//////////////////////////////////

// operator() creates members of map if key is missing.
// We need to return reference to any value (even if missing) since we can't 
// predict if we read or set an element, so we sometimes create fake elements.
// This function removes them.
template<class T>
void Vector<T>::_clear_fake_vals(){
    for(const auto& elem: values){
        if (abs(elem.second) < eps)
            values.erase(elem.first);
    }
}

template<>
void Vector<Complex_number<>>::_clear_fake_vals(){
    for(const auto& elem: values){
        if (elem.second.module_square() < eps * eps)
            values.erase(elem.first);
    }
}


template<>
void Vector<Rational_number>::_clear_fake_vals(){
    Rational_number rat_eps = Rational_number::from_double(eps);
    for(const auto& elem: values){
        if (abs(elem.second) < rat_eps)
            values.erase(elem.first);
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


//////////////////////////////////

#endif  //__ClassVector_H__