#include <vector>
#include <iostream>
#include <cmath>
#include <iomanip>
#include <fstream>
#include "../../source/spline.hpp"

const int N = 6;

int main()
{
    double h = 2.5;
    double maxErr = 0;
    double mark = 0;
    for (int i = 0; i < 1e3; i++)
    {
        maxErr = 0;
        mark = 0;
        h /= 1.0037;
        std::vector<double> xArr, yArr;
        xArr.resize(0);
        yArr.resize(0);
        for (size_t j = 0; j < 10 / h + 1; j++)
        {
            xArr.push_back(j * h);
            yArr.push_back(std::exp(j*h));
        }
        CubicSpline<double, double> Spline(xArr, yArr, std::exp(0), std::exp(10));
        for (size_t j = 0; j < 1e4; j++)
        {
            double err = std::abs(Spline.interpolate((10. / 1e4) * j) - exp((10. / 1e4) * j)) / exp((10. / 1e4) * j);
            if (err > maxErr)
                maxErr = err;
        }
        std::ofstream fout("/home/rogoda/cpp_projects/CompMath/tests/mainFiles/spline/exp_spline_otn_.txt", std::ios::app);
        fout << std::fixed << std::setprecision(16) << maxErr << " " << 10/h + 1 << std::endl;
        fout.close();
    }
    std::cout << h << std::endl;
    return 0;
}