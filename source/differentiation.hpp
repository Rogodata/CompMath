#pragma once

#include <array>
#include <iostream>
#include <eigen3/Eigen/Dense>

template <unsigned int N> struct Factorial {
    enum { value = N * Factorial<N - 1>::value };
};

template <> struct Factorial<0> {
    enum { value = 1 };
};

template <typename RealType, unsigned int N>
struct DerivativeCoef
{
    RealType centralCoef;
    std::array<RealType, N> otherCoefs;
};

template <typename RealType, unsigned int N, unsigned int L>
DerivativeCoef<RealType, N> calcDerivativeCoef(const std::array<RealType, N> &points) noexcept
{
    // матрица такой слау метода неопределённых коэффициентов необычная, первая строка вся из единиц, а первый столбец,
    // кроме первого члена стостоит из нулей. Решим слау без первой строки и первого столбца, а центральный коэффициент
    // рассчитаем отдельно. В отличие от посика первой производной в столбце свободных коэффициентов теперь факториал
    // порядка производной, чтобы не делать лишние преобразования с матрицей
    Eigen::Matrix<RealType, Eigen::Dynamic, Eigen::Dynamic> matrix(N, N);
    Eigen::Vector<RealType, Eigen::Dynamic> coeff(N), eigAns(N);
    for (std::size_t i = 1; i < N; i++)
    {
        coeff(i) = 0;
    }
    coeff(L - 1) = Factorial<L>::value;
    for (std::size_t i = 0; i < N; i++)
    {
        matrix(0, i) = points[i];
    }
    for (std::size_t i = 1; i < N; i++)
    {
        for (std::size_t j = 0; j < N; j++)
        {
            // Нет смысла делить строки матрицы на факториал
            matrix(i, j) = matrix(i - 1, j) * (points[j]);
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
