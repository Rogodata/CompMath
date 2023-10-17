#include <gtest/gtest.h>
#include <iostream>
#include <cmath>
#include <array>
#include "../source/integration.hpp"

double mysin(double x){
    return sin(x);
};

TEST(integrationtest_test, Test_1)
{
    const std::size_t N = 3;
    const double trueAnswer = 0;
    double testAnswer = integrate<decltype(mysin), double, N>(mysin, -M_PI, M_PI, 0.1);
    ASSERT_NEAR(trueAnswer, testAnswer, 1e-13);
}



int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}