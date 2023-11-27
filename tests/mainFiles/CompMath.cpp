#include <vector>
#include <iostream>
#include <cmath>
#include <iomanip>
#include <fstream>
#include "../../source/nonlinear.hpp"
#include <eigen3/Eigen/Dense>

Eigen::Matrix<double, 1, 2> myfunc(Eigen::Matrix<double, 1, 2> &pair)
{
    Eigen::Matrix<double, 1, 2> result(pair(0, 0) * pair(0, 0) + pair(0, 1) * pair(0, 1) - 1, std::tan(pair(0, 0)) - pair(0, 1));
    return result;
};

int main()
{   
    //два решения: одно решение в первой четверти, а второе - в третьей четверти координатной плоскости
    Eigen::Matrix<double, 1, 2> initial1(1. / 2, 1 / 2), initial2(-1. / 2, -1. / 2);
    Eigen::Matrix<double, 1, 2> answer1 = solve<decltype(myfunc), double>(myfunc, 1./4, initial1, 80);
    std::cout << std::setprecision(16) << answer1 << " " << std::endl;
    Eigen::Matrix<double, 1, 2> answer2 = solve<decltype(myfunc), double>(myfunc, 1./4., initial2, 80);
    std::cout << std::setprecision(16) << answer2 << " " << std::endl;
    return 0;
}