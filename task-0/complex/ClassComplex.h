/**
 * @file ClassComplex.h
 * @brief Template class Complex_number
 */

#ifndef __ClassComplex_H__
#define __ClassComplex_H__

#include <ostream>

template <class R = double, class T = R>
class Complex_number;

#include "../exceptions/CommonExceptions.hpp"
#include "../exceptions/ComplNumbersExceptions.hpp"

template <class R = double, class T = R>
std::ostream& operator<<(std::ostream &os, const Complex_number<R, T>& n);

/**
 * @brief Class to store complex numbers and perform operation with them.
 * 
 * R and T must support constructors with parameter 0.
 * Comparison is carried out on the basis of the squares of the modules of numbers.
 * Operations where left operand in not Complex_number are forbiden.
 * 
 * @tparam R - type for real part (default value: double)
 * @tparam T - type for imaginary part (default value: R)
 */
template <class R, class T>
class Complex_number{
private:
    R real;
    T imag;
public:
    /**
     * @brief  Default Rational_number constructor
     * 
     * R(0) and T(0) are called
     * @return 0 as complex number
     */
    Complex_number();

    /**
     * @brief Construct a new Complex_number object
     * 
     * "Make complex number out of real": denominator is 0
     * @param x - real part
     */
    Complex_number(R x);

    /**
     * @brief Construct a new Complex_number object
     * 
     * @param x - real part
     * @param y - imaginary part
     */
    Complex_number(R x, T y);

    /**
     * @brief Copy constructor for Complex_number
     * 
     * @param other - complex number to copy 
     * @tparam U - type of other.real, casted to R
     * @tparam V - type of other.imag, casted to T
     */
    template<class U, class V>
    Complex_number(const Complex_number<U, V>& other);


    /**
     * @brief Get square of module of complex number
     * 
     * Return type is R in assumption that type R is "wider" than type T
     * @return R - square of module
     */
    R module_square() const;   // compute module (radius-vector) 


    /**
     * @brief Assignment operator
     
     * @param other value to assign
     * @tparam U type of other.real
     * @tparam V type of other.imag
     * @return Complex_number& 
     */
    template<class U, class V>
    Complex_number& operator=(const Complex_number<U, V>& other);


    /**
     * @brief Get the real part
     * 
     * @return R - real part
     */
    R get_real() const;

    /**
     * @brief Get the imaginagy imag
     * 
     * @return T - imag part
     */
    T get_imag() const;


    /**
     * @brief Output specification for Complex_number object
     * 
     * @param os output stream
     * @param n Complex_number object to print
     * @return std::ostream& 
     */
    friend std::ostream& operator<<<R, T>(std::ostream &os, const Complex_number& n);

    /**
     * @brief Get string representation of Complex_number object
     * 
     * @return std::string, string representation ("(real, imag)")
     */
    std::string to_string() const;


    /**
     * @brief Check if two complex numbers are equal
     * 
     * Left operand is this object
     * @param rhs - right operand
     * @return bool
     */
    bool operator==(const Complex_number& rhs) const;

    /**
     * @brief Check if this object is less than rhs (in term of modules)
     * 
     * Left operand is this object
     * @param rhs - right operand
     * @return bool
     */
    bool operator<(const Complex_number& rhs) const;

    /**
     * @brief Check if this object is not equal to rhs
     * 
     * Left operand is this object
     * @param rhs - right operand
     * @return bool
     */
    bool operator!=(const Complex_number& rhs) const;

    /**
     * @brief Check if this object is less or equal than rhs (in term of modules)
     * 
     * Left operand is this object
     * @param rhs - right operand
     * @return bool
     */
    bool operator<=(const Complex_number& rhs) const;

    /**
     * @brief Check if this object is greater than rhs (in term of modules)
     * 
     * Left operand is this object
     * @param rhs - right operand
     * @return bool
     */
    bool operator>(const Complex_number& rhs) const;

    /**
     * @brief Check if this object is greater or equal to rhs (in term of modules)
     * 
     * Left operand is this object
     * @param rhs - right operand
     * @return bool
     */
    bool operator>=(const Complex_number& rhs) const;


    /**
     * @brief Sum two complex numbers (this + rhs)
     * 
     * Left operand is this object. 
     * @param rhs right operand
     * @return return type of left operand
     */
    Complex_number operator+(const Complex_number& rhs) const;

    /**
     * @brief Substraction of two complex numbers (this - rhs)
     * 
     * Left operand is this object. 
     * @param rhs right operand
     * @return return type of left operand
     */
    Complex_number operator-(const Complex_number& rhs) const;

    /**
     * @brief Product of two complex numbers (this * rhs)
     * 
     * Left operand is this object. 
     * @param rhs right operand
     * @return return type of left operand
     */
    Complex_number operator*(const Complex_number& rhs) const;

    /**
     * @brief Division two complex numbers (this / rhs)
     * 
     * Left operand is this object. 
     * @param rhs right operand
     * @return return type of left operand
     * @throw int Zero_division if rhs is zero
     */
    Complex_number operator/(const Complex_number& rhs) const;

    
    /**
     * @brief Assignment operator +
     * 
     * @param rhs value to add
     * @return Complex_number& 
     */
    Complex_number& operator+=(const Complex_number& rhs);

    /**
     * @brief Assignment operator -
     *
     * @param rhs value to substract
     * @return Complex_number& 
     */
    Complex_number& operator-=(const Complex_number& rhs);

    /**
     * @brief Assignment operator *
     * 
     * @param rhs value to product on
     * @return Complex_number& 
     */
    Complex_number& operator*=(const Complex_number& rhs);

    /**
     * @brief Assignment operator /
     * 
     * @param rhs value to divide on
     * @return Complex_number& 
     * 
     * @throw int TODO
     */
    Complex_number& operator/=(const Complex_number& rhs);


    /// @brief unar +
    Complex_number operator+() const;
    
    /// @brief unar -
    Complex_number operator-() const;

    /// @brief prefix increment (+(1,0))
    Complex_number operator++();

    /// @brief  postfix increment(+(1,0))
    Complex_number operator++(int);

    /// @brief prefix decrement (-(1,0))
    Complex_number operator--();

    /// @brief postfix decrement (-(1,0)) 
    Complex_number operator--(int);


    /**
     * @brief Explicit cast to int if possible
     * 
     * Possible if: imaginary part is 0 and static_cast<int>(real) is possible
     * 
     * @throw Bad_cast if cast isn't possible
     * @return int 
     */
    explicit operator int() const;

    /**
     * @brief Explicit cast to long if possible
     * 
     * Possible if: imaginary part is 0 and static_cast<long>(real) is possible
     * 
     * @throw Bad_cast if cast isn't possible
     * @return long 
     */
    explicit operator long() const;

    /**
     * @brief Explicit cast to short if possible
     * 
     * Possible if: imaginary part is 0 and static_cast<short>(real) is possible
     * 
     * @throw Bad_cast if cast isn't possible
     * @return short 
     */
    explicit operator short() const;

    /**
     * @brief Explicit cast to double if possible
     * 
     * Possible if: imaginary part is 0 and static_cast<double>(real) is possible
     * 
     * @throw Bad_cast if cast isn't possible
     * @return int 
     */
    explicit operator double() const;
};


// Constructors
///////////////////////////////////////////////////////////////////////

template <class R, class T>
Complex_number<R, T>::Complex_number(){
    try{
        real = R(0);
        imag = T(0);
    } catch(...){
        throw Bad_construct("Bad_construct: error during processing real(0) or imag(0), check types");
    }
}

template <class R, class T>
Complex_number<R, T>::Complex_number(R x): real(x), imag((long) 0){};

template <class R, class T>
Complex_number<R, T>::Complex_number(R x, T y): real(x), imag(y){};


template <class R, class T>
template <class U, class V>
Complex_number<R, T>::Complex_number(const Complex_number<U, V>& other){
    try{
        real = static_cast<R>(other.get_real());
        imag = static_cast<T>(other.get_imag());
    } catch (...){
        throw Bad_construct("Bad_construct: error during casting fields of other to types R and T");
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
    R res = real * real + imag * imag;
    return res;
}

template<class R, class T>
std::string Complex_number<R, T>::to_string() const{
    std::string res("(");
    res = res + std::to_string(real) + ", " + std::to_string(imag) + ")";
    return res;
}

///////////////////////////////////////////////////////////////////////

// Comparing operations
///////////////////////////////////////////////////////////////////////

template <class R, class T>
bool Complex_number<R, T>::operator==(const Complex_number& rhs) const{
    return (real == rhs.get_real() && imag == rhs.get_imag());
}

template <class R, class T>
bool Complex_number<R, T>::operator<(const Complex_number& rhs) const{
    return module_square() < rhs.module_square();
}

template <class R, class T>
bool Complex_number<R, T>::operator<=(const Complex_number& rhs) const{
    return (*this == rhs || *this < rhs);
}

template <class R, class T>
bool Complex_number<R, T>::operator!=(const Complex_number& rhs) const{
    return !(*this == rhs);
}

template <class R, class T>
bool Complex_number<R, T>::operator>(const Complex_number& rhs) const{
    return !(*this <= rhs);
}

template <class R, class T>
bool Complex_number<R, T>::operator>=(const Complex_number& rhs) const{
    return !(*this < rhs);
}
///////////////////////////////////////////////////////////////////////

// Arithmetics
///////////////////////////////////////////////////////////////////////

// return type of left operand
template <class R, class T>
Complex_number<R, T> Complex_number<R, T>::operator+(const Complex_number& rhs) const{
    return Complex_number<R, T>(real + rhs.real, imag + rhs.imag);
}

// return type of left operand
template <class R, class T>
Complex_number<R, T> Complex_number<R, T>::operator-(const Complex_number& rhs) const{
    return Complex_number<R, T>(real - rhs.real, imag - rhs.imag);
}

// return type of left operand
template <class R, class T>
Complex_number<R, T> Complex_number<R, T>::operator*(const Complex_number& rhs) const{
    return Complex_number<R, T>(real * rhs.real - imag * rhs.imag, 
                                real * rhs.imag + imag * rhs.real);
}

// return type of left operand
template <class R, class T>
Complex_number<R, T> Complex_number<R, T>::operator/(const Complex_number& rhs) const{
    if (rhs == Complex_number<R, T>(0)){
        throw Zero_division("zero division: ", rhs.to_string(), this->to_string());
    }
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
        throw Bad_construct("Error during copy constructor");  // TODO: make special exception
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
        throw Bad_cast<R, T>("Bad_cast to int: ", *this);
    }
    int res;
    try{
        res = static_cast<int>(real);
    }
    catch(...){
        throw Bad_cast<R, T>("Bad_cast to int: ", *this);
    }
    return res;
}

template <class R, class T>
Complex_number<R, T>::operator long() const{
    if (imag != 0){
        throw Bad_cast<R, T>("Bad_cast to long: ", *this);
    }
    long res;
    try{
        res = static_cast<long>(real);
    }
    catch(...){
        throw Bad_cast<R, T>("Bad_cast to long: ", *this);
    }
    return res;
}

template <class R, class T>
Complex_number<R, T>::operator short() const{
    if (imag != 0){
        throw Bad_cast<R, T>("Bad_cast to short: ", *this);
    }
    short res;
    try{
        res = static_cast<short>(real);
    }
    catch(...){
        throw Bad_cast<R, T>("Bad_cast to short: ", *this);
    }
    return res;
}

template <class R, class T>
Complex_number<R, T>::operator double() const{
    if (imag != 0){
        throw Bad_cast<R, T>("Bad_cast to double: ", *this);
    }
    double res;
    try{
        res = static_cast<double>(real);
    }
    catch(...){
        throw Bad_cast<R, T>("Bad_cast to double: ", *this);
    }
    return res;
}

///////////////////////////////////////////////////////////////////////

#endif // __ClassComplex_H__
