#include <gtest/gtest.h>
#include <iostream>
#include <cmath>
#include <time.h>
#include <array>
#include <stdio.h>
#include <stdlib.h>
#include "../source/interpolation.hpp"

TEST(interpolationTest, Test_1)
{
    const std::size_t N = 10;
    srand((unsigned)time(NULL));
    std::array<double, N> yArr;
    std::array<double, N> xArr;
    xArr[0] = static_cast<double>(std::rand()) / RAND_MAX * 4;
    yArr[0] = static_cast<double>(std::rand()) / RAND_MAX * 40;
    for (int i = 1; i < N; i++)
    {
        xArr[i] = xArr[i - 1] + static_cast<double>(std::rand()) / RAND_MAX * 4;
        yArr[i] = yArr[i - 1] + static_cast<double>(std::rand()) / RAND_MAX * 40;
    }
    for (int i = 0; i < N; i++)
    {
        std::cout << xArr[i] << " " << yArr[i] << std::endl;
    }
    NewtonInterpolator<double, double, N> interpolator(xArr, yArr);
    for (size_t i = 0; i < N; ++i)
    {
        ASSERT_NEAR((yArr[i] -  interpolator.interpolate(xArr[i])) / yArr[i],0 ,  1e-12);
    }
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}