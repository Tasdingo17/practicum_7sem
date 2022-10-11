/**
 * @file MatrixTest.cpp
 * @brief Tests for class Matrix
 */

#include "../../matrix/ClassMatrix.h"
#include "../../exceptions/MatrixExceptions.hpp"
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

std::filesystem::path matrix_test_path = project_path() / "tests" / "matrix" / "matrix_input_files";

TEST(MatrixTest, ConstrTest){
    Matrix<double> matr1(10, 15);
    matr1(1, 3) = 5;
    matr1(2, 10) = 6;
    matr1(7, 0) = 0.00001;
    EXPECT_EQ(matr1.get_size(), 2);

    Matrix<Complex_number<>> matr2(10, 15, {{{1, 2}, Complex_number<>(2)}, {{3, 10}, Complex_number<>(0, 0.0001)}});
    matr2(2, 9) = Complex_number<>(10, 2);
    EXPECT_EQ(matr2.get_size(), 2);

    Matrix<Complex_number<>> matr3(matr2);
    EXPECT_EQ(matr3.get_size(), 2);
    EXPECT_EQ(matr2.get_size(), 2);

    Matrix<Complex_number<>> matr4(std::move(matr2));
    EXPECT_EQ(matr4.get_size(), 2);
    EXPECT_EQ(matr2.get_size(), 0);

    Matrix<Complex_number<>> matr5(matr4[Matrix_coords({0, 1}, {1, 8})]);
    EXPECT_EQ(matr5.get_size(), 1);
    EXPECT_DOUBLE_EQ(matr5(1, 2).get_real(), 2);

    Matrix<Rational_number> matr6(std::string(matrix_test_path / "matrix_rational.txt").c_str());
    EXPECT_EQ(matr6(5999, 1).to_string(), "<23/5>");    // index shift by 1

    Matrix<Complex_number<>> matr7(std::string(matrix_test_path / "matrix_complex.txt").c_str());
    EXPECT_DOUBLE_EQ(matr7(5999, 1).get_imag(), 2);     // index shift by 1

    EXPECT_THROW(Matrix<Complex_number<>>(std::string(matrix_test_path / "matrix_rational.txt").c_str()), Type_error);
    EXPECT_THROW(Matrix<Complex_number<>>(std::string(matrix_test_path / "matrix_bad.txt").c_str()), Parser_error);
}

TEST(MatrixTest, OperatorsTest){
}

//TEST(MatrixTest, SliceTest){
//
//}

//TEST(MatrixTest, MethodsTest){
//}