/**
 * @file ComplNumbersOperatorsTest.cpp
 * @brief Tests for Complex_number methods and operators
 */

#include "../../complex/ClassComplex.h"
#include "../../exceptions/ComplNumbersExceptions.hpp"
#include "../../exceptions/CommonExceptions.hpp"
#include "gtest/gtest.h"

TEST(ComplNumberMethodsTest, Arithmetics){
    Complex_number<int, int> lhs(15.0, 4.0), rhs(6.0, -2);

    EXPECT_EQ((lhs + rhs).get_real(), 21.0);
    EXPECT_EQ((lhs + rhs).get_imag(), 2.0);

    EXPECT_EQ((lhs - rhs).get_real(), 9.0);
    EXPECT_EQ((lhs - rhs).get_imag(), 6.0);

    EXPECT_EQ((lhs * rhs).get_real(), 98.0);
    EXPECT_EQ((lhs * rhs).get_imag(), -6.0);

    EXPECT_EQ((lhs / rhs).get_real(), 2);   // since int values
    EXPECT_EQ((lhs / rhs).get_imag(), 1);   // since int values

    Complex_number<int, int> chs (-7, 8);
    EXPECT_EQ((lhs + chs - rhs).get_real(), 2);
    EXPECT_EQ((lhs + chs - rhs).get_imag(), 14);
}

TEST(ComplNumberMethodsTest, Comrarison){
    Complex_number<double> r1(30, -10), r2(2, 1);   // in terms of module
    EXPECT_FALSE(r1 == r2);
    EXPECT_TRUE(r1 != r2);
    EXPECT_FALSE(r1 < r2);
    EXPECT_TRUE(r1 >= r2);
    EXPECT_FALSE(r1 <= r2);
    EXPECT_TRUE(r1 > r2);
}

TEST(ComplNumberMethodsTest, Assignment){
    Complex_number<int> a(34, 35), b;
    b = a;
    EXPECT_EQ(b, a);
    Complex_number<int> c(17, 5);
    b += c;
    EXPECT_EQ(b, Complex_number<int>(51, 40));
    b -= c;
    EXPECT_EQ(b, Complex_number<int>(34, 35));
    b *= c;
    EXPECT_EQ(b, Complex_number<int>(403, 765));
    b /= c;
    EXPECT_EQ(b, Complex_number<int>(34, 35));
}


TEST(ComplNumberMethodsTest, Others){
    Complex_number<Rational_number> c(Rational_number(5, 3));
    EXPECT_EQ(c.module_square(), Rational_number(5, 3) * Rational_number(5, 3));
}