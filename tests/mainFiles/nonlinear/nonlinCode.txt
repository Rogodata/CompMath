#include <vector>
#include <iostream>
#include <cmath>
#include <iomanip>
#include <fstream>
#include "../../source/nonlinear.hpp"

const int N = 6;

int main()
{   
    double E_true = 1.5853138613542082;
    std::size_t maxIter = 200;
    double e = 0.8;
    double m = M_PI / 4;
    double prev = m, curr;
    for (std::size_t i = 1; i < maxIter; i++)
    {
        curr = prev - (prev - e * std::sin(prev) - m) / (1 - e * std::cos(prev));
        prev = curr;
        std::ofstream fout("/home/rogoda/cpp_projects/CompMath/tests/mainFiles/nonlinear/kepler8.txt", std::ios::app);
        fout << std::fixed << std::setprecision(16) << curr - E_true  << " " << i << std::endl;
        fout.close();
    }
    
    return 0;
}