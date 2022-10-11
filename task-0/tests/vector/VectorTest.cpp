/**
 * @file VectorTest.cpp
 * @brief Tests for class Vector
 */

#include "../../vector/ClassVector.hpp"
#include "../../exceptions/VectorExceptions.hpp"
#include "../../exceptions/CommonExceptions.hpp"
#include "../../exceptions/ParserExceptions.hpp"
#include <filesystem>
#include "gtest/gtest.h"

#ifndef __proj_path__
#define __proj_path__
std::filesystem::path project_path() {
    return std::filesystem::current_path().parent_path();
}
#endif //__proj_path

std::filesystem::path vector_test_path = project_path() / "tests" / "vector" / "vector_input_files";

TEST(VectorTest, ConstrTest){
    Vector<double> vec1(10);
    vec1(1) = 5;
    vec1(2) = 6;
    vec1(7) = 0.00001;
    EXPECT_EQ(vec1.get_size(), 2);

    Vector<Complex_number<>> vec2(20, {{1, Complex_number<>(2)}, {3, Complex_number<>(0, 0.0001)}});
    vec2(2) = Complex_number<>(10, 2);
    EXPECT_EQ(vec2.get_size(), 2);

    Vector<Complex_number<>> vec3(vec2);
    EXPECT_EQ(vec3.get_size(), 2);
    EXPECT_EQ(vec2.get_size(), 2);

    Vector<Complex_number<>> vec4(std::move(vec2));
    EXPECT_EQ(vec4.get_size(), 2);
    EXPECT_EQ(vec2.get_size(), 0);

    Matrix<int> f(10, 15, {{{1, 3}, 5}, {{1, 10}, 156}, {{0, 1}, 1500}} );
    Vector<int> vec5(f[Matrix_row_coord(1)]);
    EXPECT_EQ(vec5(10), 156);

    Vector<Rational_number> vec6(std::string(vector_test_path / "vector_rational.txt").c_str());
    EXPECT_EQ(vec6(5999).to_string(), "<44/1>");    // index shift by 1

    Vector<Complex_number<>> vec7(std::string(vector_test_path / "vector_complex.txt").c_str());
    EXPECT_DOUBLE_EQ(vec7(5999).get_imag(), 2);     // index shift by 1

    EXPECT_THROW(Vector<Complex_number<>>(std::string(vector_test_path / "vector_rational.txt").c_str()), Type_error);
    EXPECT_THROW(Vector<Complex_number<>>(std::string(vector_test_path / "vector_bad.txt").c_str()), Parser_error);
}


TEST(VectorTest, OperatorsTest){
    Vector<int> vec1(10, {{1, 5}, {7, 4}});
    Vector<int> vec2(10, {{2, 6}, {7, 4}});
    Vector<int> vec3(10);

    vec3 = vec1 + vec2;
    EXPECT_EQ(vec3.get_size(), 3);
    EXPECT_EQ(vec3(7), 8);

    vec3 = vec1 - vec2;
    EXPECT_EQ(vec3.get_size(), 2);
    EXPECT_EQ(vec3(7), 0);

    EXPECT_EQ((-vec1)(7), -4);

    Matrix<int> f(10, 3, {{{1, 2}, 5}, {{1, 0}, 156}, {{0, 1}, 1500}} );
    Vector<int> vec_f(10, {{0, 2}, {1, 3}, {6, 5}});
    Vector<int> res_f(vec_f * f);
    EXPECT_EQ(res_f.get_max_size(), 3);
    EXPECT_EQ(res_f(0), 468);
    EXPECT_EQ(res_f(1), 3000);
    EXPECT_EQ(res_f(2), 15);

    EXPECT_THROW((Vector<int>(9) * f), Shape_error);
    EXPECT_THROW((Vector<int>(9) + vec1), Shape_error);

    vec3 = vec1 * 2;
    EXPECT_EQ(vec3(1), 10);
}

//TEST(VectorTest, MethodsTest){
//}