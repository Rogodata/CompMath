#include <array>
#include <cmath>
#include <type_traits>

template <typename A>
struct ArgumentGetter;

template <typename R, typename Arg>
struct ArgumentGetter<R(Arg)>
{
    using Argument = Arg;
};

template <std::size_t N>
struct GaussianKvadrature
{
    std::array<double, N> points, weights;
};

template <>
struct GaussianKvadrature<2>
{
    static constexpr std::array<double, 2> points = {-1. / sqrt(3.), 1. / sqrt(3.)}, weights = {1, 1};
};

template <>
struct GaussianKvadrature<3>
{
    static constexpr std::array<double, 3> points = {-sqrt(3. / 5.), 0, sqrt(3. / 5.)}, weights = {5. / 9, 8. / 9, 5. / 9};
};

template <>
struct GaussianKvadrature<4>
{
    static constexpr std::array<double, 4> points = {0.861136, 0.339981, 0.339981, 0.861136}, weights = {0.347855, 0.652145, 0.652145, 0.347855};
};

template <typename T>
using Dif = decltype(std::declval<T>() - std::declval<T>());

/* Функция производит интегрирование на одном отрезке */
template <typename Callable, typename RealType, std::size_t N>
decltype(auto) integrate(
    const Callable &func,                                     // Интегрируемая функция
    const typename ArgumentGetter<Callable>::Argument &start, // начало отрезка
    const typename ArgumentGetter<Callable>::Argument &end    // конец отрезка
)
{
    RealType result = 0;
    for (size_t i = 0; i < N; i++)
    {
        result += GaussianKvadrature<N>::weights[i] * func((end + start) / 2 + GaussianKvadrature<N>::points[i] * (end - start) / 2);
    }
    return result * (end - start) / 2;
};

/* Функция производит интегрирование, разбивая отрезок на подотрезки длиной не более dx */
template <typename Callable, typename RealType, std::size_t N>
decltype(auto) integrate(
    const Callable &func,                                      // Интегрируемая функция
    const typename ArgumentGetter<Callable>::Argument &start,  // начало отрезка
    const typename ArgumentGetter<Callable>::Argument &end,    // конец отрезка
    const Dif<typename ArgumentGetter<Callable>::Argument> &dx // Длина подотрезка
)
{
    RealType result = 0;
    typename ArgumentGetter<Callable>::Argument position = start;
    while (position + dx < end)
    {
        result += integrate<Callable, RealType, N>(func, position, position + dx);
        position += dx;
    }
    result += integrate<Callable, RealType, N>(func, position - dx, end);
    return result;
};