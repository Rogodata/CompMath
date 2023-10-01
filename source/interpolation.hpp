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
        coeffs = values;
        
        for (std::size_t i = 0; i < N - 1; i++)
        {
            for (std::size_t j = N - 1; j > i; j--)
            {
                coeffs[j] = (coeffs[j] -  coeffs[j - 1]) / (points[j] - points[j - 1 - i]);
                
            }
        }
        
        
    };

    yType interpolate(const xType &x) const noexcept
    {
        yType result = coeffs[0];
        xType mnozh = 1;
        for (std::size_t i = 0; i < N - 1; i++)
        {
            mnozh *= x - intPoints[i];
            result += mnozh * coeffs[i + 1];
        }
        return result;
    };
};
