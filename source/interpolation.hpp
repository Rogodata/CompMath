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
    // точки, последнюю точку хранить не нужно
    std::array<xType, N - 1> intPoints;
    // значения разделённых разностей
    std::array<yType, N> coeffs;

public:
    NewtonInterpolator(const std::array<xType, N> &points, const std::array<yType, N> &values) noexcept
    {
        // конструктор подготовит коэффициенты и далее уже зная их рассчитаем значение
        for (std::size_t i = 0; i < N - 1; i++)
        {
            intPoints[i] = points[i];
        }
        std::copy(values.begin(), values.end(), coeffs.begin());
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
        //суммируем схемой Горнера
        /*yType result = coeffs[N - 1] * (x - intPoints[N-2]);
        for (std::size_t i = N - 2; i > 0; i--)
        {
            result = (result + coeffs[i]) * (x - intPoints[i - 1]);
        }
        result += coeffs[0];*/
        // Но ввот так выходит точнее, ошибка на рандомных числах порядка 1е-11
        yType result = coeffs[N - 1];
        for (std::size_t i = N - 1; i > 0; i--)
        {
            result = fma(result, (x - intPoints[i - 1]), coeffs[i - 1]);
        }
        return result;
    };
};
