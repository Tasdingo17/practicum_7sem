#include "../src/Functions.h"
#include "../src/FuncFabric.h"
#include "gtest/gtest.h"

TEST(FuncTest, Construction){
    TFactory funcFactory;
    auto g1 = funcFactory.CreateObject("power", {2}); // PowerFunc x^2
    PowerF g1_ = PowerF(2);
    EXPECT_EQ(typeid(*g1), typeid(g1_));
    EXPECT_THROW(funcFactory.CreateObject("power", {3, 4}), std::invalid_argument);

    auto g2 =
    funcFactory.CreateObject("polynomial", {7, 0, 3, 15}); // TPolynomial 7 + 3*x^2 + 15*x^3
    PolynomialF g2_ = PolynomialF({3, 15});
    EXPECT_EQ(typeid(*g2), typeid(g2_));

    auto g3 =
    funcFactory.CreateObject("ident", {7}); // f(x) = x 
    IdentF g3_;
    EXPECT_EQ(typeid(*g1), typeid(g1_));
    EXPECT_THROW(funcFactory.CreateObject("power", {3, 4}), std::invalid_argument);

    auto g4 = 
    funcFactory.CreateObject("const", {10}); // f(x) = 10
    ConstF g4_(5);
    EXPECT_EQ(typeid(*g4), typeid(g4_));

    auto g5 = 
    funcFactory.CreateObject("exp", {}); // f(x) = e^x
    ExpF g5_;
    EXPECT_EQ(typeid(*g5), typeid(g5_));

    auto g6 = funcFactory.CreateObject("error", {});
    EXPECT_EQ(g6, nullptr);
}

TEST(FuncTest, Values){
}

TEST(FuncTest, ToString){
}

TEST(FuncTest, Arithmetics){
}

TEST(FuncTest, Derivative){
}

TEST(FuncTest, RootFinder){
}