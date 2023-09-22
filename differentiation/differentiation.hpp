#pragma once

#include <array>
#include <iostream>
#include "../eigen-3.4.0/Eigen/Dense"

template <typename RealType, unsigned int N>
struct DerivativeCoef
{
    RealType centralCoef;
    std::array<RealType, N> otherCoefs;
};

template <typename RealType, unsigned int N>
DerivativeCoef<RealType, N> calcDerivativeCoef(const std::array<RealType, N> &points) noexcept
{
    // points - коэффициенты перед h
    // расчёт коэффициентов
    // матрица такой слау метода неопределённых коэффициентов необычная, первая строка вся из единиц, а первый столбец, 
    // кроме первого члена стостоит из нулей. Решим слау без первой строки и первого столбца, а центральный коэффициент рассчитаем отдельно
    Eigen::Matrix<RealType, Eigen::Dynamic, Eigen::Dynamic> matrix(N, N);
    Eigen::Vector<RealType, Eigen::Dynamic> coeff(N), eigAns(N);
    for (std::size_t i = 1; i < N; i++)
    {
        coeff(i) = 0;
    }
    coeff(0) = 1;
    for (std::size_t i = 0; i < N; i++)
    {
        matrix(0, i) = points[i];
    }
    for (std::size_t i = 1; i < N; i++)
    {
        for (std::size_t j = 0; j < N; j++)
        {
            matrix(i, j) = matrix(i - 1, j) * (points[j]) / (i+ 1);
        }
    }
    eigAns = matrix.colPivHouseholderQr().solve(coeff);
    RealType central = -eigAns.sum();
    std::array<RealType, N> otherArray;
    for (std::size_t i = 0; i < N; i++)
    {
        otherArray[i] = eigAns(i);
    }
    return {central, otherArray};
};
