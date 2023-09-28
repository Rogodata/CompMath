#include <iostream>
#include <iomanip>
#include <eigen3/Eigen/Dense>
#include <array>
#include <fstream>
#include "source/differentiation/differentiation.hpp"


const std::size_t N = 3;

int main()
{
    const std::size_t N = 2;
    std::array<double, 2> hCoeff{-1, 1};
    const double trueCentralCoeff = 0.0;
    const std::array<double, 2> trueOtherCoeff {-0.5, 0.5};
    DerivativeCoef<double, N> test_answer = calcDerivativeCoef<double, N>(hCoeff);
    for (size_t i = 0; i < N; ++i)
    {
        std::cout << trueOtherCoeff[i] << " " <<  test_answer.otherCoefs[i] << std::endl;
    }
    return 0;
}
