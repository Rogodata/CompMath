#include <array>
#include <iostream>
#include <cmath>
#include <iomanip>
#include <fstream>
#include "../../source/integration.hpp"

double mysin(double x){
    //return std::sin(x);
    return std::sin(100 * x) * std::exp(-x * x) * std::cos(2 * x);
};

const int N = 3;

int main()
{
    double h = 3;
    double maxErr = 0;
    double mark = 0;
    for (int i = 0; i < 3.5e3; i++)
    {
        h /= 1.0035;
        double testAnswer = integrate<decltype(mysin), double, N>(mysin, 0, 3, h);
        std::ofstream fout("/home/rogoda/cpp_projects/CompMath/tests/mainFiles/integrating/func_3.txt", std::ios::app);
        fout << std::fixed << std::setprecision(16) << testAnswer  << " " << h << std::endl;
        //fout << std::fixed << std::setprecision(16) << (testAnswer - 1.8390715290764524522) / 1.8390715290764524522  << " " << h << std::endl;
        fout.close();
    }
    std::cout << h << std::endl;
    return 0;
}