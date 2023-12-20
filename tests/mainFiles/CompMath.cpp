
#include <eigen3/Eigen/Dense>
#include <array>
#include <vector>
#include <iomanip>
#include <fstream>
#include <iostream>
#include "../../source/RungeKutta.hpp"

const std::size_t N = 1;

int main()
{   
    Oscillator osc1;
    FirstTask task;
    RK4Table table;
    
    std::vector<Oscillator::StateAndArg> test1 = integrate<RK4Table, Oscillator>(Oscillator::StateAndArg{Eigen::Vector<double, 2>{0, 1.}, double (0)}, 5, table, 0.002, osc1);
    for(const auto &element : test1)
    {
        std::ofstream fout("/home/rogoda/cpp_projects/CompMath/tests/mainFiles/RungeKutta/test.txt", std::ios::app);
        fout << std::fixed << std::setprecision(16) << element.state[0] << " " << element.arg << std::endl;
        fout.close();
    }
    return 0;
}