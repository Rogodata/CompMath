#include <gtest/gtest.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <array>
#include "../source/spline.hpp"

TEST(tridiagonal_test, Test_1)
{
    std::vector<double> column = {1, 2, 3}, trueAnswer = {-13. / 17, 15. / 17, 7. / 17};
    ThreeDiagonalMatrix<double> matrix({3, 2}, {1, 3, 3}, {2, 4});
    std::vector<double> testAnswer = solve(matrix, column);
    for (size_t i = 0; i < 3; i++)
    {
        ASSERT_NEAR(trueAnswer[i], testAnswer[i], 1e-12);
    }
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}