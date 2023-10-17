#include <array>
#include <iostream>
#include <cmath>
#include <iomanip>
#include <fstream>
#include "../../source/interpolation.hpp"

const int N = 3;

int main()
{

    std::array<double, N> yArr{-0.2, -0.5, 0.8};
    std::array<double, N> xArr{0.5 - sqrt(3.)/4., 0.5, 0.5 + sqrt(3.)/4.};
    xArr[1] = 0.5;
    for (int i =0; i < 3; i++){
        std::cout << xArr[i] << " ";
    }

    NewtonInterpolator<double, double, N> interpolator(xArr, yArr);
    //std::cout << interpolator.interpolate(0) << std::endl;
    return 0;
}