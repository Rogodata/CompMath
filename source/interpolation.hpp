#pragma once

#include <array>

/**
 * xType - тип аргумента x.
 * yType - тип значения функции y
 * N - количество точек для интерполяции
 *
 * Рекомедую обратить внимание. Разность (xType - xType) не обязана быть типом xType
 */
template <typename xType, typename yType, unsigned int N>
class NewtonInterpolator
{
private:
    // точки
    std::array<xType, N> intPoints;
    // значения разделённых разностей
    std::array<yType, N> coeffs;

public:
    NewtonInterpolator(const std::array<xType, N> &points, const std::array<yType, N> &values) noexcept
        : intPoints{points}, coeffs(values)
    {
        // конструктор подготовит коэффициенты и далее уже зная их рассчитаем значение
        for (std::size_t i = 0; i < N - 1; i++)
        {
            for (std::size_t j = N - 1; j > i; j--)
            {
                coeffs[j] = (coeffs[j] - coeffs[j - 1]) / (points[j] - points[j - 1 - i]);
            }
        }
    };

    yType interpolate(const xType &x) const noexcept
    {
        // суммируем схемой Горнера
        yType result = coeffs[N - 1];
        for (std::size_t i = N - 1; i > 0; i--)
        {
            result = std::fma(result, (x - intPoints[i - 1]), coeffs[i - 1]);
        }
        return result;
    };
};

/**
 * xType - тип аргумента x.
 * yType - тип значения функции y
 * N - количество точек для интерполяции
 *
 * Рекомедую обратить внимание. Разность (xType - xType) не обязана быть типом xType
 */
template <typename xType, typename yType, unsigned int N>
class HermiteInterpolator
{
private:
    // точки
    std::array<xType, 2 * N> intPoints;
    // значения разделённых разностей
    std::array<yType, 2 * N> razn;
    /*** Какие-то поля ***/
public:
    HermiteInterpolator(const std::array<xType, N> &points, const std::array<yType, N> &values, const std::array<yType, N> &deriv) noexcept
    {
        // Сделаем хитрый интерполянт Ньютона
        for (std::size_t i = 0; i < N; i++)
        {
            intPoints[2 * i] = points[i];
            intPoints[2 * i + 1] = points[i];
        }
        razn[0] = values[0];
        razn[1] = deriv[0];
        for (std::size_t i = 1; i < N; i++)
        {
            razn[2 * i] = (values[i] - values[i - 1]) / (points[i] - points[i - 1]);
            razn[2 * i + 1] = deriv[i];
        }
        // сейчас можно воспользоваться схемой из интерполятора Ньютона, где одну итерацию поиска разделённых разностей мы уже провели
        for (std::size_t i = 1; i < 2 * N - 1; i++)
        {
            for (std::size_t j = 2 * N - 1; j > i; j--)
            {
                razn[j] = (razn[j] - razn[j - 1]) / (intPoints[j] - intPoints[j - 1 - i]);
            }
        }
    };

    yType interpolate(const xType &x) const noexcept
    {
        yType result = razn[2 * N - 1];
        for (std::size_t i = 2 * N - 1; i > 0; i--)
        {
            result = std::fma(result, (x - intPoints[i - 1]), razn[i - 1]);
        }
        return result;
    };
};
