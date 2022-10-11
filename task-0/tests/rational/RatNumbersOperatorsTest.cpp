/**
 * @file RatNumbersOperatorsTest.cpp
 * @brief Tests for Rational_number methods and operators
 */

#include "../../rational/ClassRationalNumber.h"
#include "../../exceptions/RatNumbersExceptions.hpp"
#include "../../exceptions/CommonExceptions.hpp"
#include "gtest/gtest.h"

TEST(RatNumberMethodsTest, Arithmetics){
    Rational_number r1(3, 4), r2(-5, 6);
    EXPECT_EQ((r1 + r2).to_string(), "<-1/12>");
    EXPECT_EQ((r1 - r2).to_string(), "<19/12>");
    EXPECT_EQ((r2 - r1).to_string(), "<-19/12>");
    EXPECT_EQ((r1 * r2).to_string(), "<-5/8>");
    EXPECT_EQ((r1 / r2).to_string(), "<-9/10>");
    EXPECT_EQ((r2 / r1).to_string(), "<-10/9>");
    Rational_number r3 (-7, 8);
    EXPECT_EQ((r2 * r3).to_string(), "<35/48>");
    EXPECT_EQ((r2 / r3).to_string(), "<20/21>");
    EXPECT_EQ((r1 + r2 - r3).to_string(), "<19/24>");

    EXPECT_EQ((r1 + 10).to_string(), "<43/4>");
    EXPECT_EQ((10 + r1).to_string(), "<43/4>");

    EXPECT_THROW((r1 / (long) 0), Zero_division);
    EXPECT_THROW((r1 / Rational_number((long) 0)), Zero_division);

    EXPECT_EQ((+r1).to_string(), "<3/4>");
    EXPECT_EQ((-r1).to_string(), "<-3/4>");
    EXPECT_EQ((++r1).to_string(), "<7/4>");
    EXPECT_EQ((--r1).to_string(), "<3/4>");
    
    EXPECT_EQ((r1++).to_string(), "<3/4>");
    EXPECT_EQ(r1.to_string(), "<7/4>");
    EXPECT_EQ((r1--).to_string(), "<7/4>");
    EXPECT_EQ(r1.to_string(), "<3/4>");

    Rational_number r4("131444452345", "1335"), r5("4324", "8658585");
    EXPECT_EQ((r4 + r5).to_string(), "<75874864227560291/770614065>");
    EXPECT_EQ((r4 - r5).to_string(), "<75874864226790619/770614065>");
    EXPECT_EQ((r4 * r5).to_string(), "<113673162387956/2311842195>");
    EXPECT_EQ((r4 / r5).to_string(), "<75874864227175455/384836>");
}

TEST(RatNumberMethodsTest, Comrarison){
    Rational_number r1(3, 4), r2(-5, 6);
    EXPECT_FALSE(r1 == r2);
    EXPECT_TRUE(r1 != r2);
    EXPECT_FALSE(r1 < r2);
    EXPECT_TRUE(r1 >= r2);
    EXPECT_FALSE(r1 <= r2);
    EXPECT_TRUE(r1 > r2);

    Rational_number r3(1, 2), r4(2, 4);
    EXPECT_TRUE(r3 == r4);
    EXPECT_FALSE(r3 != r4);
    EXPECT_FALSE(r3 < r4);
    EXPECT_TRUE(r3 >= r4);
    EXPECT_TRUE(r3 <= r4);
    EXPECT_FALSE(r3 > r4);
}

TEST(RatNumberMethodsTest, Assignment){
    Rational_number a(34, 35), b;
    b = a;
    EXPECT_EQ(b.to_string(), "<34/35>");
    Rational_number c(17, 5);
    b += c;
    EXPECT_EQ(b.to_string(), "<153/35>");
    b -= c;
    EXPECT_EQ(b.to_string(), "<34/35>");
    b *= c;
    EXPECT_EQ(b.to_string(), "<578/175>");
    b /= c;
    EXPECT_EQ(b.to_string(), "<34/35>");
}

