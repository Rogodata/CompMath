#include <vector>
#include <iostream>
#include <cmath>
#include <iomanip>
#include <fstream>
#include "../../source/spline.hpp"

int main()
{
    std::vector<double> xArr, yArr;
    xArr = {1, 4, 7, 12, 20};
    yArr = {5, 4, 10, 18, 23};
    CubicSpline<double, double> Spline(xArr, yArr);
    std::cout << Spline.interpolate(20) << std::endl;
    return 0;
}