/**
 * @file
 * @brief Header file with Rational_number class description.
*/

#ifndef __ClassRational_H__
#define __ClassRational_H__

#include <string>
#include <ostream>

/**
 * @brief Class to store rational numbers and perform operation with them.
 * 
 *  Numerator and denominator are stored as reversed strings.
 * Operations are performed using long arithmetics (c++ logic).
 * Operations with rational numbers  also can be performed 
 * when integer base type is specified as one of the operands.
 *  Exceptions are stored in ../exceptions/RatNumbersExceptions.hpp 
 * and ../exceptions/CommonExceptions.hpp
*/
class Rational_number{
private:
    std::string numerator;   // reversed
    std::string denominator; // reversed
    bool is_negative;
    bool is_number(const std::string& s) const;
public:
    /**
     * @brief Default Rational_number constructor
     * 
     * @return 0 as Rational_number ("0/1")
     */
    Rational_number();

    /**
     * @brief Construct a new Rational_number object
     * 
     * If numerator is 0 then res is alway "0/1".
     * Auto convert to canonical form.
     * @param m numerator
     * @param n denumerator
     * 
     * @throw Not_a_number if given string is not a whole number
     * @throw Zero_division_rat if nominator is zero
     */
    Rational_number(const std::string& m, const std::string& n = "1");

    /**
     * @brief Construct a new Rational_number object
     * 
     * If numerator is 0 then res is alway "0/1".
     * Auto convert to canonical form.
     * @param m numerator
     * @param n denumerator
     * 
     * @throw Not_a_number if given string is not a whole number
     * @throw Zero_division_rat if nominator is zero
     */
    Rational_number(const char* m, const char* n = "1");

    /**
     * @brief Construct a new Rational_number object
     * 
     * If numerator is 0 then res is alway "0/1".
     * Auto convert to canonical form.
     * @param m numerator
     * @param n denumerator
     * 
     * @throw Zero_division_rat if nominator is zero
     */
    Rational_number(long int x, long int y = 1);

    /**
     * @brief Copy constructor for Rational_number object
     * 
     * @param other - rational number to copy value from
     * 
     */
    Rational_number(const Rational_number& other);

    /**
     * @brief Convert rational number to canonical form
     * 
     * Automaticall called almost always: during construction, after operations...
     */
    void make_canonical();

    /**
     * @brief Perform floor on rational number if possible
     * 
     * @return floor() result as in math
     * 
     * @throw Out_of_bounds if result is out of long long range
     */
    long long floor() const;

    /**
     * @brief Perform round on rational number if possible
     * 
     * @return round() result as in math
     * 
     * @throw Out_of_bounds if result is out of long long range
     */
    long long round() const; 

    /**
     * @brief Explicit cast to int if possible
     * 
     * Possible if: denominator is 1 and fits in type's bound
     * @return int
     * 
     * @throw Out_of_bounds if result is out of int range
     * @throw Bad_cast if denominator is not 1s
     */
    explicit operator int() const;

    /**
     * @brief Explicit cast to long of possible
     * 
     * Possible if: denominator is 1 and fits in type's bounds
     * @return long 
     * 
     * @throw Out_of_bounds if result is out of long range
     * @throw Bad_cast if denominator is not 1
     */
    explicit operator long() const;

    /**
     * @brief Explicit cast to short of possible
     * 
     * Possible if: denominator is 1 and fits in type's bounds
     * @return  short
     * 
     * @throw Out_of_bounds if result is out of short range
     * @throw Bad_cast if denominator is not 1
     */
    explicit operator short() const;


    /**
     * @brief Output specification for Rational_number object
     * 
     * @param os output stream
     * @param n Rational_number object to print
     * @return std::ostream& 
     */
    friend std::ostream& operator<<(std::ostream &os, const Rational_number& n);

    /**
     * @brief Get absolute value of Rational_number object
     * 
     * @param obj object to get absolute value of
     * @return Rational_number, same numerator and denominator but possitive
     */
    friend Rational_number abs(const Rational_number& obj);

    /**
     * @brief Get string representation of Rational_number object
     * 
     * @return std::string, string representation ("<numerator/denominator>")
     */
    std::string to_string() const;

    /**
     * @brief Get Rational_number object from double value
     * 
     * Static method due to:
     * - ambiguity avoidance during construction
     * - a number of assumptions (max 15 decimal places after .)
     * - inaccuracy with double values
     * @param x duble value to convert
     * @return Rational_number 
     */
    static Rational_number from_double(double x);

    /**
     * @brief Check if two rational numbers are equal
     * 
     * @param lhs left operand
     * @param rhs right operand
     * @return true if values are equal
     * @return false if values are not equal
     */
    friend bool operator==(const Rational_number& lhs, const Rational_number& rhs);

    /**
     * @brief Check if lhs is less than rhs
     * 
     * @param lhs left operand
     * @param rhs right operand
     * @return true lhs < rhs
     * @return false lhs >= rhs
     */
    friend bool operator<(const Rational_number& lhs, const Rational_number& rhs);

    /**
     * @brief Check if lhs is not equal to rhs
     * 
     * @param lhs left operand
     * @param rhs right operand
     * @return true lhs not equal to rhs
     * @return false lhs equal to ths
     */
    friend bool operator!=(const Rational_number& left, const Rational_number& right);

    /**
     * @brief Check if lhs is less ot equal to rhs
     * 
     * @param lhs left operand
     * @param rhs right operand
     * @return true lhs <= rhs
     * @return false lhs > rhs
     */
    friend bool operator<=(const Rational_number& left, const Rational_number& right);

    /**
     * @brief Check if lhs is greater than rhs
     * 
     * @param lhs left operand
     * @param rhs right operand
     * @return true lhs > rhs
     * @return false lhs <= rhs
     */
    friend bool operator>(const Rational_number& left, const Rational_number& right);

    /**
     * @brief Check if lhs is greater or equal to rhs
     * 
     * @param lhs left operand
     * @param rhs right operand
     * @return true lhs >= rhs
     * @return false lhs < rhs
     */
    friend bool operator>=(const Rational_number& left, const Rational_number& right);
    

    /**
     * @brief Sum of two rational numbers
     * 
     * @param lhs left operand
     * @param rhs right operand
     * @return Rational_number lhs + rhs
     */
    friend Rational_number operator+(const Rational_number& lhs, const Rational_number& rhs);
    
    /**
     * @brief Substract two rational numbers
     * 
     * @param lhs left operand
     * @param rhs right operand
     * @return Rational_number, lhs - rhs
     */
    friend Rational_number operator-(const Rational_number& lhs, const Rational_number& rhs);

    /**
     * @brief Product of two rational numbers
     * 
     * @param lhs left operand
     * @param rhs right operand
     * @return Rational_number sum result
     */
    friend Rational_number operator*(const Rational_number& lhs, const Rational_number& rhs);

    /**
     * @brief Division two rational numbers
     * 
     * @param lhs left operand
     * @param rhs right operand
     * @return Rational_number lhs / rhs
     * 
     * @throw Zero_division_rat if rhs is zero
     */
    friend Rational_number operator/(const Rational_number& lhs, const Rational_number& rhs);

    /**
     * @brief Assignment operator
     * 
     * @param v value to assign
     * @return Rational_number& 
     */
    Rational_number& operator=(const Rational_number& v);

    /**
     * @brief Assignment operator +
     * 
     * @param v value to add
     * @return Rational_number& 
     */
    Rational_number& operator+=(const Rational_number& v);
    
    /**
     * @brief Assignment operator -
     * 
     * @param v value to sub
     * @return Rational_number& 
     */
    Rational_number& operator-=(const Rational_number& v);

    /**
     * @brief Assignment operator *
     * 
     * @param v value to product
     * @return Rational_number& 
     */
    Rational_number& operator*=(const Rational_number& v);

    /**
     * @brief Assignment operator
     * 
     * @param v value to divide on
     * @return Rational_number& 
     * @throw Zero_division_rat if rhs is zero
     */
    Rational_number& operator/=(const Rational_number& v);


    /// @brief Unar +
    Rational_number operator+() const;

    /// @brief Unar -
    Rational_number operator-() const;

    /// @brief Prefix increment
    Rational_number operator++();

    /// @brief Postfix increment
    Rational_number operator++(int);

    /// @brief Prefix decrement
    Rational_number operator--();

    /// @brief Postfix decrement
    Rational_number operator--(int);
};


#endif // __ClassRational_H__