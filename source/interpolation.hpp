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
        /*yType result = coeffs[N - 1] * (x - intPoints[N-2]);
        for (std::size_t i = N - 2; i > 0; i--)
        {
            result = (result + coeffs[i]) * (x - intPoints[i - 1]);
        }
        result += coeffs[0];*/
        // Но вот так выходит точнее, относительная ошибка на рандомных числах порядка 1е-12
        yType result = coeffs[N - 1];
        for (std::size_t i = N - 1; i > 0; i--)
        {
            result = std::fma(result, (x - intPoints[i - 1]), coeffs[i - 1]);
        }
        return result;
    };
};