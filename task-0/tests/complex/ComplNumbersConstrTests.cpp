/**
 * @file ComplNumbersConstrTests.cpp
 * @brief Tests for Complex_number constructors
 */

#include "../../complex/ClassComplex.h"
#include "../../exceptions/ComplNumbersExceptions.hpp"
#include "../../exceptions/CommonExceptions.hpp"
#include "gtest/gtest.h"


TEST(ComplNumberConstrTest, DefaultConstr){
    Complex_number<> a;
    EXPECT_EQ(a.to_string(), "(0.000000, 0.000000)") << "Default value expected to be zero, got: " << a.to_string();

    Complex_number<Rational_number> b;
    EXPECT_EQ(b.to_string(), "(<0/1>, <0/1>)") << "Default value expected to be zero, got: " << b.to_string();

    Complex_number<int, bool> c;
    EXPECT_EQ(c.to_string(), "(0, 0)") << "Default value expected to be zero, got: " << c.to_string();
}


TEST(ComplNumberConstrTest, ConstrFromNumbers){
    Complex_number<int> a(15);
    EXPECT_EQ(a.get_real(), 15);
    EXPECT_EQ(a.get_imag(), 0);

    Complex_number<double> b(24.5);
    EXPECT_DOUBLE_EQ(b.get_real(), 24.5);
    EXPECT_DOUBLE_EQ(b.get_imag(), 0.0);

    Complex_number<double, bool> c(12.12, false);
    EXPECT_DOUBLE_EQ(c.get_real(), 12.12);
    EXPECT_EQ(!c.get_imag(), true);
}

TEST(ComplNumberConstrTest, ConstrCopy){
    Complex_number<> a(Complex_number<>(15.9, 24.5));
    EXPECT_DOUBLE_EQ(a.get_real(), 15.9);
    EXPECT_DOUBLE_EQ(a.get_imag(), 24.5);

    Complex_number<int, int> number = Complex_number<long double, bool>(321.123, true);
    auto result(number);
    EXPECT_EQ(result.get_real(), 321);
    EXPECT_EQ(result.get_imag(), 1);
}