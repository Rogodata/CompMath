#include <gtest/gtest.h>
#include <iostream>
#include <cmath>
#include <array>
#include "../source/integration.hpp"

double mysin(double x){
    return std::sin(x);
};

double quadfunc(double x){
  return x * x;
}

TEST(integrationtest_test, Test_1)
{
    const std::size_t N = 5;
    const double trueAnswer = 1./3;
    double testAnswer = integrate<decltype(quadfunc), double, N>(quadfunc, 0, 1, 1e-5);
    ASSERT_NEAR(trueAnswer, testAnswer, 1e-14);
}

TEST(integrationtest_test, Test_2)
{
    const std::size_t N = 4;
    const double trueAnswer = 0;
    double testAnswer = integrate<decltype(mysin), double, N>(mysin, 0, 2 * M_PI, 0.25);
    ASSERT_NEAR(trueAnswer, testAnswer, 1e-14);
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}