/**
 * @file RatNumbersConstrTests.cpp
 * @brief Tests for Rational_numbers constructor
 */

#include "../../rational/ClassRationalNumber.h"
#include "../../exceptions/RatNumbersExceptions.hpp"
#include "../../exceptions/CommonExceptions.hpp"
#include "gtest/gtest.h"

TEST(RatNumberConstrTest, DefaultConstr){
    Rational_number a;
    EXPECT_EQ(a.to_string(), "<0/1>") << "Default value expected to be zero, got: " << a.to_string();
}


TEST(RatNumberConstrTest, ConstrFromStrings){
    EXPECT_EQ(Rational_number("125", "47").to_string(), "<125/47>");
    EXPECT_EQ(Rational_number("250", "94").to_string(), "<125/47>");
    EXPECT_EQ(Rational_number("-5", "4").to_string(), "<-5/4>");
    EXPECT_EQ(Rational_number("8", "-7").to_string(), "<-8/7>");
    EXPECT_EQ(Rational_number("0", "23").to_string(), "<0/1>");
    EXPECT_EQ(Rational_number("0", "-4").to_string(), "<0/1>");
    EXPECT_EQ(Rational_number("013", "-023").to_string(), "<-13/23>");

    EXPECT_EQ(Rational_number("5").to_string(), "<5/1>");
    EXPECT_EQ(Rational_number("-7").to_string(), "<-7/1>");

    EXPECT_THROW(Rational_number("1r3", "654"), Not_a_number);
    EXPECT_THROW(Rational_number("32", "2fs"), Not_a_number);
    EXPECT_THROW(Rational_number("32.5", "342"), Not_a_number);
    EXPECT_NO_THROW(Rational_number("-24", "-7"));
    EXPECT_THROW(Rational_number("311", "0"), Zero_division);
}


TEST(RatNumberConstrTest, ConstrFromInts){
    EXPECT_EQ(Rational_number(125, 47).to_string(), "<125/47>");
    EXPECT_EQ(Rational_number(250, 94).to_string(), "<125/47>");
    EXPECT_EQ(Rational_number(-5, 4).to_string(), "<-5/4>");
    EXPECT_EQ(Rational_number(8, -7).to_string(), "<-8/7>");
    EXPECT_EQ(Rational_number(0, 23).to_string(), "<0/1>");
    EXPECT_EQ(Rational_number(0, -4).to_string(), "<0/1>");

    EXPECT_EQ(Rational_number(5).to_string(), "<5/1>");
    EXPECT_EQ(Rational_number(-7).to_string(), "<-7/1>");

    EXPECT_THROW(Rational_number(311, 0), Zero_division);
}

TEST(RatNumberConstrTest, ConstrCopy){
    Rational_number a;
    EXPECT_EQ(Rational_number(a).to_string(), "<0/1>");

    Rational_number b("123", "322");
    EXPECT_EQ(Rational_number(b).to_string(), "<123/322>");

    Rational_number c("123", "-322");
    EXPECT_EQ(Rational_number(c).to_string(), "<-123/322>");
}