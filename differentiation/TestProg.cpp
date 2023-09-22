#include <iostream>
#include <iomanip>
#include "../eigen-3.4.0/Eigen/Dense"
#include <array>
#include <fstream>
#include "differentiation.hpp"

const std::size_t N = 5;

int main()
{
    std::array<double, 5> hCoeff{-1, 1, 2, 3, 4};
    double h = 10;
    DerivativeCoef<double, N> test_answer = calcDerivativeCoef<double, N>(hCoeff);
    for (long j = 0; j < 16; j++)
    {
        double diff = 0;
        h = h / 10;
        for (auto i = 0; i < N; i++)
        {
            diff += test_answer.otherCoefs[i] * exp(1 + hCoeff[i] * h) / h;
        }
        diff += test_answer.centralCoef * exp(1) / h;
        std::ofstream fout("/home/rogoda/cpp_projects/CompMath/differentiation/exp_1_5.txt", std::ios::app);
        fout << std::fixed << std::setprecision(16) <<(diff - exp(1)) / exp(1) << " " << h << std::endl;
        fout.close();
    }

    return 0;
}
