#ifndef __ClassComplex_H__
#define __ClassComplex_H__

#include <ostream>

template <class R = double, class T = R>
class Complex_number;

template <class R = double, class T = R>
std::ostream& operator<<(std::ostream &os, const Complex_number<R, T>& n);

template <class R, class T>
class Complex_number{
private:
    R real;
    T imag;
public:
    Complex_number();
    Complex_number(R x, T y);

    template<class U, class V>
    Complex_number(const Complex_number<U, V>& other);

    R module_square() const;   // compute module (radius-vector) 

    template<class U, class V>
    Complex_number& operator=(const Complex_number<U, V>& other);

    R get_real() const;
    T get_imag() const;

    friend std::ostream& operator<<<R, T>(std::ostream &os, const Complex_number& n);

    bool operator==(const Complex_number& rhs);
    bool operator<(const Complex_number& rhs);
    bool operator!=(const Complex_number& rhs);
    bool operator<=(const Complex_number& rhs);
    bool operator>(const Complex_number& rhs);
    bool operator>=(const Complex_number& rhs);
    
    Complex_number operator+(const Complex_number& rhs); // return type of left operand
    Complex_number operator-(const Complex_number& rhs); // return type of left operand
    Complex_number operator*(const Complex_number& rhs); // return type of left operand
    Complex_number operator/(const Complex_number& rhs); // return type of left operand

    Complex_number& operator+=(const Complex_number& rhs);
    Complex_number& operator-=(const Complex_number& rhs);
    Complex_number& operator*=(const Complex_number& rhs);
    Complex_number& operator/=(const Complex_number& rhs);

    Complex_number operator+() const;   //unar +
    Complex_number operator-() const;   //unar -
    Complex_number operator++();        //prefix incr
    Complex_number operator++(int);     //postfix incr
    Complex_number operator--();        //prefix decr
    Complex_number operator--(int);     //postfix decr

    explicit operator int() const;
    explicit operator long() const;
    explicit operator short() const;
    explicit operator double() const;
};


// Constructors
///////////////////////////////////////////////////////////////////////

template <class R, class T>
Complex_number<R, T>::Complex_number(){
    try{
        real = R();
        imag = T();
    } catch(...){
        throw 3;  // TODO: make special exception
    }
}

template <class R, class T>
Complex_number<R, T>::Complex_number(R x, T y): real(x), imag(y){};


template <class R, class T>
template <class U, class V>
Complex_number<R, T>::Complex_number(const Complex_number<U, V>& other){
    try{
        real = static_cast<R>(other.get_real());
        imag = static_cast<T>(other.get_imag());
    } catch (...){
        throw 4;  // TODO: make special exception
    }
}
///////////////////////////////////////////////////////////////////////

// Methods
///////////////////////////////////////////////////////////////////////

template <class R, class T>
std::ostream& operator<<(std::ostream &os, const Complex_number<R, T>& n){
    return os << '(' << n.real << ';' << n.imag << ')';
}

template <class R, class T>
R Complex_number<R, T>::get_real() const{
    return real;
}    

template <class R, class T>
T Complex_number<R, T>::get_imag() const{
    return imag;
}

// compute module (radius-vector) 
template <class R, class T>
R Complex_number<R, T>::module_square() const{
    R res;
    try {
        res = real * real + imag * imag;
    } catch(...) {
        throw 5;   // TODO: make special exception (R * R + T * T is not defined )
    }
    return res;
}
///////////////////////////////////////////////////////////////////////

// Comparing operations
///////////////////////////////////////////////////////////////////////

template <class R, class T>
bool Complex_number<R, T>::operator==(const Complex_number& rhs){
    return (real == rhs.get_real() && imag == rhs.get_imag());
}

template <class R, class T>
bool Complex_number<R, T>::operator<(const Complex_number& rhs){
    return module_square() < rhs.module_square();
}

template <class R, class T>
bool Complex_number<R, T>::operator<=(const Complex_number& rhs){
    return (*this == rhs || *this < rhs);
}

template <class R, class T>
bool Complex_number<R, T>::operator!=(const Complex_number& rhs){
    return !(*this == rhs);
}

template <class R, class T>
bool Complex_number<R, T>::operator>(const Complex_number& rhs){
    return !(*this <= rhs);
}

template <class R, class T>
bool Complex_number<R, T>::operator>=(const Complex_number& rhs){
    return !(*this < rhs);
}
///////////////////////////////////////////////////////////////////////

// Arithmetics
///////////////////////////////////////////////////////////////////////

// return type of left operand
template <class R, class T>
Complex_number<R, T> Complex_number<R, T>::operator+(const Complex_number& rhs){
    return Complex_number<R, T>(real + rhs.real, imag + rhs.imag);
}

// return type of left operand
template <class R, class T>
Complex_number<R, T> Complex_number<R, T>::operator-(const Complex_number& rhs){
    return Complex_number<R, T>(real - rhs.real, imag - rhs.imag);
}

// return type of left operand
template <class R, class T>
Complex_number<R, T> Complex_number<R, T>::operator*(const Complex_number& rhs){
    return Complex_number<R, T>(real * rhs.real - imag * rhs.imag, 
                                real * rhs.imag + imag * rhs.real);
}

// return type of left operand
template <class R, class T>
Complex_number<R, T> Complex_number<R, T>::operator/(const Complex_number& rhs){
    return Complex_number<R, T>((real * rhs.real + imag * rhs.imag) / rhs.module_square(), 
                                (imag * rhs.real - real * rhs.imag) / rhs.module_square());
}


///////////////////////////////////////////////////////////////////////

// Assignment operators
///////////////////////////////////////////////////////////////////////

template <class R, class T>
template <class U, class V>
Complex_number<R, T>& Complex_number<R, T>::operator=(const Complex_number<U, V>& other){
    try{
        real = static_cast<R>(other.get_real());
        imag = static_cast<T>(other.get_imag());
    } catch (...){
        throw 4;  // TODO: make special exception
    }
    return *this;
}

// return type of left operand
template <class R, class T>
Complex_number<R, T>& Complex_number<R, T>::operator+=(const Complex_number& rhs){
    return *this = (*this + rhs);
}


// return type of left operand
template <class R, class T>
Complex_number<R, T>& Complex_number<R, T>::operator-=(const Complex_number& rhs){
    return *this = (*this - rhs);
}


// return type of left operand
template <class R, class T>
Complex_number<R, T>& Complex_number<R, T>::operator*=(const Complex_number& rhs){
    return *this = (*this * rhs);
}


// return type of left operand
template <class R, class T>
Complex_number<R, T>& Complex_number<R, T>::operator/=(const Complex_number& rhs){
    return *this = (*this / rhs);
}
///////////////////////////////////////////////////////////////////////


// Unar operators
///////////////////////////////////////////////////////////////////////

//unar +
template <class R, class T>
Complex_number<R, T> Complex_number<R, T>::operator+() const{
    return Complex_number(*this);
} 

//unar -
template <class R, class T>
Complex_number<R, T> Complex_number<R, T>::operator-() const{
    Complex_number copy(*this);
    return copy * Complex_number(-1, 0);
}

//prefix incr
template <class R, class T>
Complex_number<R, T> Complex_number<R, T>::operator++(){
    return (*this += Complex_number(1, 0));
}        

//postfix incr
template <class R, class T>
Complex_number<R, T> Complex_number<R, T>::operator++(int){
    *this += Complex_number(1, 0);
    return *this - Complex_number(1, 0);
}   

//prefix decr
template <class R, class T>
Complex_number<R, T> Complex_number<R, T>::operator--(){
    return (*this -= Complex_number(1, 0));
}      

//postfix decr
template <class R, class T>
Complex_number<R, T> Complex_number<R, T>::operator--(int){
    *this -= Complex_number(1, 0);
    return *this + Complex_number(1, 0);
}

///////////////////////////////////////////////////////////////////////


// Type casters
///////////////////////////////////////////////////////////////////////

template <class R, class T>
Complex_number<R, T>::operator int() const{
    if (imag != 0){
        throw 5;    // TODO: make speciall exception
    }
    return static_cast<int>(real);
}

template <class R, class T>
Complex_number<R, T>::operator long() const{
    if (imag != 0){
        throw 5;    // TODO: make speciall exception
    }
    return static_cast<long>(real);
}

template <class R, class T>
Complex_number<R, T>::operator short() const{
    if (imag != 0){
        throw 5;    // TODO: make speciall exception
    }
    return static_cast<short>(real);
}

template <class R, class T>
Complex_number<R, T>::operator double() const{
    if (imag != 0){
        throw 5;    // TODO: make speciall exception
    }
    return static_cast<double>(real);
}

///////////////////////////////////////////////////////////////////////

#endif // __ClassComplex_H__
