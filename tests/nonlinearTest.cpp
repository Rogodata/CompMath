#include <gtest/gtest.h>
#include <iostream>
#include <cmath>
#include <time.h>
#include <array>
#include <stdio.h>
#include <stdlib.h>
#include "../source/nonlinear.hpp"

double myfunc(double x){
    return 1./2. - std::sin(x);
}

TEST(nonlinearTest, Test_1)
{
    double e = 0.8, m = M_PI / 4;
    double answer = keplerSolver(e, m, 100, 1e-14);
    ASSERT_NEAR(0 , answer - e * std::sin(answer) - m , 1e-15);
    //ASSERT_NEAR(0 , answer , 1e-15);
}

TEST(nonlinearTest, Test_2)
{
    double answer = solve(myfunc, 0.5, M_PI / 4, 60);
    ASSERT_NEAR(std::abs(M_PI / 6  - answer) / (M_PI/6),0, 1e-13);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}