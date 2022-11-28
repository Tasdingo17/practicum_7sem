#include "../src/Functions.h"
#include "../src/FuncFabric.h"
#include "gtest/gtest.h"

#include <cmath>

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
    TFactory funcFactory;
    
    auto g1 = funcFactory.CreateObject("power", {6}); // PowerFunc x^6
    EXPECT_DOUBLE_EQ((*g1)(2), 64);
    EXPECT_DOUBLE_EQ((*g1)(5), 15625);

    g1 = funcFactory.CreateObject("ident", {}); // f(x) = x
    EXPECT_DOUBLE_EQ((*g1)(10), 10);
    EXPECT_DOUBLE_EQ((*g1)(-1.5), -1.5);
    
    g1 = funcFactory.CreateObject("const", {8});
    EXPECT_DOUBLE_EQ((*g1)(-40), 8);
    EXPECT_DOUBLE_EQ((*g1)(0), (*g1)(-40));

    g1 = funcFactory.CreateObject("polynomial", {-1, -1, 2});   // Polynomial -1 - x + 2*x^2
    EXPECT_DOUBLE_EQ((*g1)(1), 0);
    EXPECT_DOUBLE_EQ((*g1)(-2), 9);

    g1 = funcFactory.CreateObject("exp", {});
    EXPECT_DOUBLE_EQ((*g1)(10), exp(10));
    EXPECT_DOUBLE_EQ((*g1)(1), exp(1));
}

TEST(FuncTest, ToString){
    TFactory funcFactory;
    auto g1 = funcFactory.CreateObject("power", {2}); // PowerFunc x^2
    EXPECT_EQ(g1->ToString(3), "f(x) = x^2.000");
    EXPECT_EQ(g1->ToString(), "f(x) = x^2.000000");

    auto g2 =
    funcFactory.CreateObject("polynomial", {7, 0, 3, 15}); // TPolynomial 7 + 3*x^2 + 15*x^3
    EXPECT_EQ(g2->ToString(1), "f(x) = 7.0*x^0 + 0.0*x^1 + 3.0*x^2 + 15.0*x^3");

    auto g3 =
    funcFactory.CreateObject("ident", {}); // f(x) = x
    EXPECT_EQ(g3->ToString(), "f(x) = x");

    auto g4 = 
    funcFactory.CreateObject("const", {10.543}); // f(x) = 10
    EXPECT_EQ(g4->ToString(4), "f(x) = 10.5430");
    EXPECT_EQ(g4->ToString(2), "f(x) = 10.54");

    auto g5 = 
    funcFactory.CreateObject("exp", {}); // f(x) = e^x
    EXPECT_EQ(g5->ToString(), "f(x) = e^x");
}

TEST(FuncTest, Arithmetics){
    TFactory funcFactory;
    auto h1 = funcFactory.CreateObject("ident", {});    // f(x) = x
    auto h2 = funcFactory.CreateObject("exp", {});  // e^x
    auto h3 = funcFactory.CreateObject("polynomial", {1, 5, 1});    // 1 + 5x + x^2
    EXPECT_NEAR((*h1 + *h2 * *h3)(2), 112.836, 0.001);

    EXPECT_THROW((*h1 + 2).ToString(), std::logic_error);
}

TEST(FuncTest, Derivative){
    TFactory funcFactory;
    auto p1 = funcFactory.CreateObject("ident", {});
    EXPECT_DOUBLE_EQ(p1->derivative(100), 1);

    auto p2 = funcFactory.CreateObject("exp", {});
    EXPECT_DOUBLE_EQ(p2->derivative(10), exp(10));

    auto p3 = funcFactory.CreateObject("polynomial", {1, 2, 1, 4});
    EXPECT_DOUBLE_EQ(p3->derivative(2), 54);

    auto p4 = funcFactory.CreateObject("const", {6});
    EXPECT_DOUBLE_EQ(p4->derivative(2), 0);

    auto p5 = funcFactory.CreateObject("power", {10});
    EXPECT_DOUBLE_EQ(p5->derivative(2), 5120);

    auto p6 = funcFactory.CreateObject("polynomial", {4, 1, 5, 1});
    auto p7 = funcFactory.CreateObject("ident", {});
    EXPECT_DOUBLE_EQ((*p6 + *p7).derivative(2), 34);
    EXPECT_DOUBLE_EQ((*p6 - *p7).derivative(2), 32);
    EXPECT_DOUBLE_EQ((*p6 * *p7).derivative(2), 100);
    EXPECT_DOUBLE_EQ((*p6 / *p7).derivative(2), 8);
}

TEST(FuncTest, RootFinder){
    TFactory funcFactory;
    auto q1 = funcFactory.CreateObject("exp", {10});
    EXPECT_LE(findRoot(*q1, 20, 1000000), -999999);

    auto q2 = funcFactory.CreateObject("polynomial", {1, 2, 1});
    EXPECT_NEAR(findRoot(*q2, -3, 100000), -1, 0.01);
    EXPECT_DOUBLE_EQ(findRoot(*q2, -1, 10000), -1);
    EXPECT_EQ(std::isnan(findRoot(*q2, -3, 100000, 1)), true);
}