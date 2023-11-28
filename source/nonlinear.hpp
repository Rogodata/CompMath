#pragma once

#include<iostream>
#include <type_traits>
#include <cmath>
#include <exception>

/**
    Решает уравнение Кеплера методом Ньютона
    * ecc - эксцентриситет, принадлежит (0, 1)
    * meanAnomaly - средняя аномалия, М (радианы)
    * maxIter - максимальное количество итераций
    * tol - точность, с которой нужно отыскать решение

    Рекомендуемое поведение. Если решение не нашлось за maxIter итераций - выбрасывать исключение.
    Если приближения к решению между итерациями меняются не более, чем на tol, то решение достигнуто.
**/
double keplerSolver(double ecc, double meanAnomaly, unsigned int maxIter, double tol)
{
    // возьмём среднюю аномалию в качестве начального приближения так как при орбитах,
    // близких к круговым средняя и эксцентрическая близкиz
    double prev = meanAnomaly, curr;
    for (std::size_t i = 0; i < maxIter; i++)
    {
        curr = prev - (prev - ecc * std::sin(prev) - meanAnomaly) / (1 - ecc * std::cos(prev));
        if (std::abs(curr - prev) < tol)
        {
            return curr;
        }
        prev = curr;
    }
    throw std::exception();
};

template <typename A>
struct ArgumentGetter;

template <typename R, typename Arg>
struct ArgumentGetter<R(Arg)>
{
    using Argument = Arg;
};

template <typename T>
using Dif = decltype(std::declval<T>() - std::declval<T>());

template <typename Callable, typename RealType>
decltype(auto) solve(
    const Callable &func,                                            // функция F
    const RealType tau,                                             // шаг тау
    const typename ArgumentGetter<Callable>::Argument& initialGuess, // начальное приближение
    const unsigned int nIteration                                    // количество итераций
){
    typename std::decay_t<typename ArgumentGetter<Callable>::Argument> curr= initialGuess;
    for (std::size_t i = 0; i < nIteration; i++)
    {
        curr += tau * func(curr);
    }
    return curr;
};

template <typename Callable, typename RealType>
decltype(auto) solve(
    const Callable &func,                                            // функция F
    const typename ArgumentGetter<Callable>::Argument &initialGuess, // начальное приближение
    const unsigned int nIteration                                    // количество итераций
){
    typename std::decay_t<typename ArgumentGetter<Callable>::Argument> curr= initialGuess;
    for (std::size_t i = 0; i < nIteration; i++)
    {
        curr = func(curr);  
    }
    return curr;
};

