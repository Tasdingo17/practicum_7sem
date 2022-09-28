#include <iostream>
#include <algorithm>
#include <limits>
#include "ClassRationalNumber.h"

#define BASE 10 
#define LONG_MAX 2147483647
#define LONG_MIN -2147483648

template <class T>
T max(T a, T b){
    return (a >= b) ? a : b;
}

template <class T>
T pow(T v, int power){
    T res = 1;
    while (power > 0) {
        res *= v;
        power -= 1;
    }
    return res;
}

std::string int_to_val_reversed(long int x);

void remove_leading_zeros(std::string& s){      // from reversed number
    while (s.back() == '0' && s.size() > 1 ) s.pop_back();
}

// Convert string to big number (no '-', reversed, no leading zeros)
void _str_to_big_number(std::string& s){
    if (s[0] == '-') s.erase(0, 1);
    std::reverse(s.begin(), s.end());
    remove_leading_zeros(s);
}

bool operator==(const std::string& lhs, const std::string& rhs){
    if (lhs.size() != rhs.size()) return false;
    for(auto it1 = lhs.rbegin(), it2 = rhs.rbegin(); it1 != lhs.rend() && it2 != rhs.rend(); it1++, it2++){
        if (*it1 != *it2) return false;
    }
    return true;
}

bool operator<(const std::string& lhs, const std::string& rhs){     // for reversed possitive numbers
    if (lhs == rhs) return false;
    if (lhs.size() != rhs.size()) {
        return lhs.size() < rhs.size();
    }
    for(auto it1 = lhs.rbegin(), it2 = rhs.rbegin(); it1 != lhs.rend() && it2 != rhs.rend(); it1++, it2++){
        if (*it1 != *it2){
            return *it1 < *it2;
        }
    }
    return false;
}

bool operator!=(const std::string& left, const std::string& right) {
    return !(left == right);
}

bool operator<=(const std::string& left, const std::string& right) {
    return (left < right || left == right);
}

bool operator >(const std::string& left, const std::string& right) {
    return !(left <= right);
}

bool operator >=(const std::string& left, const std::string& right) {
    return !(left < right);
}

// for reversed possitive numbers
const std::string operator+(std::string left, const std::string& right){  
    int carry = 0; // флаг переноса из предыдущего разряда
    for (size_t i = 0; i < std::max(left.size(), right.size()) || carry != 0; ++i) {
        if (i == left.size()) left.push_back('0');
        left[i] += carry + (i < right.size() ? right[i] : '0') - '0';
        carry = (left[i] - '0') >= BASE;
        if (carry != 0){
            left[i] = left[i] - BASE;
        }
    }
    return left;
}

// for reversed possitive numbers, left >= right
const std::string operator-(std::string left, const std::string& right){  
    int carry = 0;
    for (size_t i = 0; i < right.size() || carry != 0; ++i) {
        left[i] -= carry + (i < right.size() ? right[i] : '0') - '0';
        carry = (left[i] - '0')  < 0;
        if (carry != 0) left[i] += BASE;
    }
    remove_leading_zeros(left);
    return left;
}

// for reversed possitive numbers
const std::string operator*(const std::string& left, const std::string& right){
    if (left == "0" || right == "0") return std::string("0");
    std::string result;
    result.resize(left.size() + right.size(), '0');
    for (size_t i = 0; i < left.size(); ++i) {
        int carry = 0;
        for (size_t j = 0; j < right.size() || carry != 0; ++j) {
            long long curr = (result[i + j] - '0') + 
                            (left[i] - '0') * 1LL * (j < right.size() ? (right[j] - '0') : 0) + carry;
            result[i + j] = static_cast<char>(curr % BASE) + '0';
            carry = static_cast<int>(curr / BASE);
        }
    }
    remove_leading_zeros(result);
    return result;
}

// shift string number by 1 
void _shift_right(std::string& s){
    if (s.size() == 0) {
        s.push_back('0');
        return;
    }
    s.push_back(s.back());
    for (size_t i = s.size() - 2; i > 0; --i) s[i] = s[i-1];
    s[0] = '0';
}

// for reversed possitive numbers
const std::string operator/(const std::string& left, const std::string& right){
    // no zero division
    if (right == "0") throw 3;  // TODO: make special exception
    if (left < right) return std::string("0");
    std::string b = right;
    std::string result, current;
    result.resize(left.size(), '0');
    for (long long i = static_cast<long long>(left.size()) - 1; i >= 0; --i) {
        _shift_right(current);
        current[0] = left[i];
        remove_leading_zeros(current);
        int x = 0, l = 0, r = BASE;
        while (l <= r) {
            int m = (l + r) / 2;
            std::string tmp_m = int_to_val_reversed(m);
            std::string t = b * tmp_m;
            if (t <= current) {
                x = m;
                l = m + 1;
            }
            else r = m - 1;
        }
        result[i] = x + '0';
        std::string tmp_x = int_to_val_reversed(x);
        current = current - b * tmp_x;
    }
    remove_leading_zeros(result);
    return result;
}

// for reversed possitive numbers
const std::string operator %(const std::string& left, const std::string& right) {
    std::string result = left - (left / right) * right;
    return result;
}

// greatest common divisor for possitive reversed numbers
std::string gcd(std::string s1, std::string s2){
    
    while (s1 != "0" and s2 != "0") {
        if (s1 > s2){
            s1 = s1 % s2;
        }
        else {
            s2 = s2 % s1;
        }
    }
    return (s1 + s2);
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

// erase a '-' sign and return int value as reversed string
std::string int_to_val_reversed(long int x){         
    long int a = (x >= 0) ? x : -x;
    std::string s = std::to_string(a);
    std::reverse(s.begin(), s.end());
    return s;
}


bool Rational_number::is_number(const std::string& s) const{
    if (s[0] == '-'){  // if negative value
        return !s.empty() && std::find_if(s.begin() + 1,  s.end(), 
            [](unsigned char c) { return !std::isdigit(c); }) == s.end();
    } else {
        return !s.empty() && std::find_if(s.begin(),  s.end(), 
            [](unsigned char c) { return !std::isdigit(c); }) == s.end();
    }
}


Rational_number::Rational_number(): numerator("0"), denominator("1"),
                                    is_negative(false) {};


Rational_number::Rational_number(const std::string& m, const std::string& n){
    numerator = m;
    denominator = n;
    if (!is_number(numerator) || !is_number(denominator)) {
        throw 2;  // TODO: make special exception
    }
    is_negative = ((numerator[0] == '-') != (denominator[0] == '-')) && (numerator != "0");  // xor
    if (numerator == "0"){
        denominator = std::string("1");
        is_negative = false;
        return;
    }
    _str_to_big_number(numerator);
    _str_to_big_number(denominator);

    make_canonical();
};

Rational_number::Rational_number(const char* m, const char* n):
    Rational_number(std::string(m), std::string(n)) {}


Rational_number::Rational_number(long int m, long int n){
    is_negative = (m >= 0) != (n > 0);  // xor
    if (n == 0){
        throw 2;  // TODO: make special exception
    }
    numerator = int_to_val_reversed(m);
    if (m == 0){
        denominator = std::string("1");
        is_negative = false;
        return;
    }
    denominator = int_to_val_reversed(n);

    make_canonical();
};

Rational_number::Rational_number(const Rational_number& other):
    numerator(other.numerator), denominator(other.denominator), 
    is_negative(other.is_negative) {};


void Rational_number::make_canonical(){
    std::string tmp = gcd(numerator, denominator);
    while (tmp != "1"){
        numerator = numerator / tmp;
        denominator = denominator / tmp;
        tmp = gcd(numerator, denominator);
    }
}

std::ostream& operator<<(std::ostream &os, const Rational_number& n){
    std::string tmp1 = n.numerator, tmp2 = n.denominator;
    std::reverse(tmp1.begin(), tmp1.end()); 
    std::reverse(tmp2.begin(), tmp2.end());
    return os << (n.is_negative ? "-" : "") << tmp1 << '/' << tmp2;
}

Rational_number& Rational_number::operator=(const Rational_number& other){
    numerator = other.numerator;
    denominator = other.denominator;
    is_negative = other.is_negative;
    return *this;
}

Rational_number& Rational_number::operator+=(const Rational_number& v){
    return *this = (*this + v);
}

Rational_number& Rational_number::operator-=(const Rational_number& v){
    return *this = (*this - v);
}

Rational_number& Rational_number::operator*=(const Rational_number& v){
    return *this = (*this * v);
}

Rational_number& Rational_number::operator/=(const Rational_number& v){
    return *this = (*this / v);
}

bool operator==(const Rational_number& lhs, const Rational_number& rhs){
    return (lhs.is_negative == rhs.is_negative) && 
           (lhs.numerator * rhs.denominator == lhs.denominator * rhs.numerator);
}

bool operator<(const Rational_number& lhs, const Rational_number& rhs){
    //std::cout << "In operator< for Rational_number, rhs: " << rhs << std::endl;
    if (lhs.is_negative < rhs.is_negative) return false;
    if (lhs.is_negative > rhs.is_negative) return true;
    if (lhs.is_negative == false){
        return (lhs.numerator * rhs.denominator < lhs.denominator * rhs.numerator);
    } else {
        return !(lhs.numerator * rhs.denominator < lhs.denominator * rhs.numerator);
    }
}

bool operator!=(const Rational_number& left, const Rational_number& right){
    return !(left == right);
}

bool operator<=(const Rational_number& left, const Rational_number& right){
    return (left < right || left == right);
}

bool operator>(const Rational_number& left, const Rational_number& right){
    return !(left <= right);
}

bool operator>=(const Rational_number& left, const Rational_number& right){
    return !(left < right);
}

Rational_number Rational_number::operator+() const{
    return Rational_number(*this);
}

Rational_number Rational_number::operator-() const{
    Rational_number copy(*this);
    copy.is_negative = !copy.is_negative;
    return copy;
}

Rational_number Rational_number::operator++(){
    return (*this += "1");
}

Rational_number Rational_number::operator++(int){
    *this += "1";
	return *this - Rational_number("1");
}

Rational_number Rational_number::operator--(){
    return *this -= 1;
}

Rational_number Rational_number::operator--(int){
    *this -= 1;
	return *this + Rational_number("1");
}

Rational_number operator+(const Rational_number& lhs, const Rational_number& rhs){
    Rational_number res;
    if (lhs.is_negative == rhs.is_negative){
        res.numerator = lhs.numerator * rhs.denominator + rhs.numerator * lhs.denominator;
        res.denominator = lhs.denominator * rhs.denominator;
        res.is_negative = lhs.is_negative;
    } else {
        bool l_greater_r_abs = lhs.numerator * rhs.denominator > rhs.numerator * lhs.denominator;
        if (l_greater_r_abs){
            res.numerator =  lhs.numerator * rhs.denominator - rhs.numerator * lhs.denominator;
            res.denominator = (res.numerator == "0") ? "1" : lhs.denominator * rhs.denominator;
            res.is_negative = lhs.is_negative && res.numerator != "0";
        } else {
            res.numerator = rhs.numerator * lhs.denominator - lhs.numerator * rhs.denominator;
            res.denominator = (res.numerator == "0") ? "1" : lhs.denominator * rhs.denominator;
            res.is_negative = rhs.is_negative && res.numerator != "0";
        }
    }

    res.make_canonical();
    return res;
}

Rational_number operator-(const Rational_number& lhs, const Rational_number& rhs){
    return lhs + (-rhs);
}

Rational_number operator*(const Rational_number& lhs, const Rational_number& rhs){
    Rational_number res;
    res.numerator = lhs.numerator * rhs.numerator;
    res.denominator = (res.numerator == "0") ? "1" : lhs.denominator * rhs.denominator;
    res.is_negative = (rhs.is_negative != lhs.is_negative) && (res.numerator != "0");
    res.make_canonical();
    return res;
}

Rational_number operator/(const Rational_number& lhs, const Rational_number& rhs){
    if (rhs == "0") throw 1;   // TODO: make special exception
    Rational_number res;
    res.numerator = lhs.numerator * rhs.denominator;
    res.denominator = rhs.numerator * lhs.denominator;
    res.is_negative = (rhs.is_negative != lhs.is_negative) && (res.numerator != "0");
    res.make_canonical();
    return res;
}


// for reversed strings
template<class T>
T string_to_val(const std::string& reversed_str){
    T res = 0;
    int i = 0;
    for (auto number = reversed_str.begin(); number != reversed_str.end(); number++){
        res += ((*number) - '0') * pow<T>(10, i);
        i += 1;
    }
    return res;
}

// checks if reversed str fits in T bounds
template<class T>
bool check_bound(const std::string& reversed_str, bool is_negative){
    std::string tmp = std::to_string(std::numeric_limits<T>::max());
    std::reverse(tmp.begin(), tmp.end());
    if ((is_negative == false && reversed_str > tmp) ||
        (is_negative == true && (reversed_str-"1") >= tmp))
        return false;
    else return true;
}

template<>
bool check_bound<long>(const std::string& reversed_str, bool is_negative){
    std::string tmp = std::to_string(LONG_MAX);
    std::reverse(tmp.begin(), tmp.end());
    if ((is_negative == false && reversed_str > tmp) ||
        (is_negative == true && (reversed_str-"1") >= tmp))
        return false;
    else return true;
}

Rational_number::operator int() const{
    if (denominator != "1") throw 1;    // TODO: make special exception
    if (!check_bound<int>(numerator, is_negative))
        throw 2;        // TODO: make special exception
    int res = string_to_val<int>(this->numerator);
    return is_negative ? -res : res;
}

Rational_number::operator long() const{
    if (denominator != "1") throw 1;    // TODO: make special exception
    if (!check_bound<long>(numerator, is_negative))
        throw 2;        // TODO: make special exception
    long res = string_to_val<long>(this->numerator);
    return is_negative ? -res : res;
}

Rational_number::operator short() const{
    if (denominator != "1") throw 1;    // TODO: make special exception
    if (!check_bound<short>(numerator, is_negative))
        throw 2;        // TODO: make special exception
    short res = string_to_val<short>(this->numerator);
    return is_negative ? -res : res;
}

long long Rational_number::floor() const{
    std::string tmp_res = numerator / denominator;
    if (!check_bound<long long>(tmp_res, is_negative))
        throw 2;        // TODO: make special exception
    long long res = string_to_val<long long>(tmp_res);
    return is_negative ? -res : res;
}

long long Rational_number::round() const{
    std::string tmp_res =  numerator / denominator, remainder = numerator - tmp_res * denominator;
    bool is_shift = remainder * "2" < denominator;
    if (is_shift) tmp_res = tmp_res + std::string("1");
    if (!check_bound<long long>(tmp_res, is_negative))
        throw 2;        // TODO: make special exception
    long long res = string_to_val<long long>(tmp_res);
    return is_negative ? -res : res;
}

Rational_number abs(const Rational_number& obj){
    Rational_number res(obj);
    res.is_negative = false;
    return res;
}

std::string Rational_number::to_string() const{
    std::string tmp1 = numerator, tmp2 = denominator;
    std::reverse(tmp1.begin(), tmp1.end()); 
    std::reverse(tmp2.begin(), tmp2.end());
    std::string res("<");
    res = res.append(is_negative ? "-" : "").append(tmp1).append("/").append(tmp2).append(">");
    return res;
}


// reversed 10^pow as string 
std::string power_10(size_t pow){
    std::string res;
    res.reserve(pow + 2);   // zeros + 1 + '\0'
    for (size_t i = 0;  i < pow; i ++){
        res.push_back('0');
    }
    res.push_back('1');
    return res;
}


Rational_number Rational_number::from_double(double x){
    char s1[30];    // size for string (14 decimal places before '.' and 15 decimal places after)
    sprintf(s1,"%.15f", x);
    std::string str_val(s1);
    size_t dot_pos = str_val.find('.');
    size_t denom_place = str_val.size() - dot_pos - 1;  // must be 15

    std::string int_part = std::move(str_val.substr(0, dot_pos));
    std::string fract_part = std::move(str_val.substr(dot_pos+1));

    Rational_number res;
    res.is_negative = int_part[0] == '-';

    std::string tmp_denom = power_10(denom_place);

    _str_to_big_number(int_part);
    _str_to_big_number(fract_part);

    res.numerator = std::move(int_part * tmp_denom + fract_part);
    res.denominator = std::move(tmp_denom);
    res.make_canonical();

    return res;
}
