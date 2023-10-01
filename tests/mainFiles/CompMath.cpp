#include <eigen3/Eigen/Dense>
#include <array>
#include <iomanip>
#include <fstream>
#include "../../source/differentiation.hpp"

const std::size_t N = 5;

int main()
{
    std::array<double, 5> hCoeff{-1, 1, 2, 3, 4};
    double h = 1;
    DerivativeCoef<double, N> test_answer = calcDerivativeCoef<double, N, 2>(hCoeff);
    for (long j = 0; j < 1e4; j++)
    {
        double diff = 0;
        h = h / 1.00369;
        for (auto i = 0; i < N; i++)
        {
            diff += test_answer.otherCoefs[i] * exp(1 + hCoeff[i] * h) / h / h;
        }
        diff += test_answer.centralCoef * exp(1) / h / h;
        std::ofstream fout("/home/rogoda/cpp_projects/CompMath/tests/mainFiles/differentiation/exp_2_5.txt", std::ios::app);
        fout << std::fixed << std::setprecision(16) << (diff - exp(1) )/ exp(1) << " " << h << std::endl;
        fout.close();
    }
    return 0;
}