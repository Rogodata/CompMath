#include <gtest/gtest.h>
#include <iostream>
#include <cmath>
#include <array>
#include "../source/differentiation.hpp"

TEST(differentiationtest_test, Test_1)
{
    const std::size_t N = 2;
    std::array<double, 2> hCoeff{-1, 1};
    const double trueCentralCoeff = 0.0;
    const std::array<double, 2> trueOtherCoeff {-0.5, 0.5};
    DerivativeCoef<double, N> test_answer = calcDerivativeCoef<double, N, 1>(hCoeff);
    for (size_t i = 0; i < N; ++i)
    {
        ASSERT_NEAR(trueOtherCoeff[i], test_answer.otherCoefs[i], 1e-13);
    }
    ASSERT_NEAR(trueCentralCoeff, test_answer.centralCoef, 1e-13);
}

TEST(differentiationtest_test, Test_2)
{
    const std::size_t N = 2;
    std::array<double, 2> hCoeff{1, 2};
    const double trueCentralCoeff = -1.5;
    const std::array<double, 2> trueOtherCoeff {2.0, -0.5};
    DerivativeCoef<double, N> test_answer = calcDerivativeCoef<double, N, 1>(hCoeff);
    for (size_t i = 0; i < N; ++i)
    {
        ASSERT_NEAR(trueOtherCoeff[i], test_answer.otherCoefs[i], 1e-13);
    }
    ASSERT_NEAR(trueCentralCoeff, test_answer.centralCoef, 1e-13);
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}