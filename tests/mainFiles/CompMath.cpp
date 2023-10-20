#include <eigen3/Eigen/Dense>
#include <array>
#include <iostream>
#include <cmath>
#include <iomanip>
#include <fstream>
#include "../../source/interpolation.hpp"

const int N = 6;

int main()
{
    double h = 2;
    double maxErr = 0;
    double mark = 0;
    for (int i = 0; i < 1e3; i++)
    {
        maxErr = 0;
        mark = 0;
        h /= 1.0035;
        std::array<double, N> yArr{exp(0), exp(h / 5), exp(2 * h / 5), exp(3 * h / 5), exp(4 * h / 5), exp(h)};
        std::array<double, N> xArr{0, h / 5, 2 * h / 5, 3 * h / 5, 4 * h / 5, h};

        HermiteInterpolator<double, double, N> interpolator(xArr, yArr, yArr);
        for (size_t j = 0; j < 1e4; j++)
        {
            double err = std::abs(interpolator.interpolate((h / 1e4) * j) - exp((h / 1e4) * j));
            if (err > maxErr)
                maxErr = err;
        }
        std::ofstream fout("/home/rogoda/cpp_projects/CompMath/tests/mainFiles/interpolation/exp_6_herm.txt", std::ios::app);
        fout << std::fixed << std::setprecision(16) << maxErr << " " << h << std::endl;
        fout.close();
    }
    std::cout << h << std::endl;
    return 0;
}