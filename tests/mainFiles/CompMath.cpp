#include <eigen3/Eigen/Dense>
#include <array>
#include <iostream>
#include <iomanip>
#include <fstream>
#include "../../source/interpolation.hpp"

const std::size_t N = 5;

int main()
{   
    double h = 2;
    double maxErr = 0;
    double mark = 0;
    for(int i = 0; i < 1e3; i ++){
        maxErr = 0;
        mark = 0;
        h /= 1.0035;
        std::array<double, 3> yArr{exp(h /2 - sqrt(3) * h / 4),exp(h / 2),exp(h/2 + sqrt(3) * h / 4)};
        std::array<double, 3> xArr{h /2 - sqrt(3) * h / 4, h / 2, h/2 + sqrt(3) * h / 4};
        NewtonInterpolator<double, double, 3> interpolator(xArr, yArr);
        for (size_t j = 0; j < 1e4; j++)
        {
            double err = interpolator.interpolate((h / 1e4) * j) - exp((h / 1e4) * j);
            if (err > maxErr)
                maxErr = err;
        }
        std::ofstream fout("/home/rogoda/cpp_projects/CompMath/tests/mainFiles/interpolation/exp_3_cheb.txt", std::ios::app);
        fout << std::fixed << std::setprecision(16) << maxErr << " " << h << std::endl;
        fout.close();
              
    }
    std::cout << h << std::endl;
    
    
    return 0;
}