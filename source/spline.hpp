#pragma once

#include <vector>
#include <type_traits>

template <typename RealType>
struct Row
{
    Row()
    {
        // default constructor
        a = 0;
        b = 0;
        c = 0;
    };

    Row(RealType a, RealType b, RealType c)
    {
        // constructor with parameters
        this->a = a;
        this->b = b;
        this->c = c;
    };

    RealType a, b, c;
};

/** класс для работы с трехдиагональной матрицей **/
template <typename Type>
class ThreeDiagonalMatrix
{
    /*** Здесь какие-то поля и методы ***/
private:
    std::vector<Row<Type>> rows;

public:
    ThreeDiagonalMatrix(std::vector<Type> &a, std::vector<Type> &b, std::vector<Type> &c)
    {
        rows.resize(0);
        rows.push_back({0, b[0], c[0]});
        for (size_t i = 0; i < b.size() - 2; i++)
        {
            rows.push_back({a[i], b[i + 1], c[i + 1]});
        }
        rows.push_back({a[b.size() - 2], b[b.size() - 1], 0});
    }

    Row<Type> operator[](const std::size_t &i_coord) const { return rows[i_coord]; }

    std::size_t get_n() const
    {
        return rows.size();
    }
};

template<typename numeratorType, typename denominatorType>
using DivisType = decltype(std::declval<numeratorType>() / std::declval<denominatorType>());

template<typename Type>
using DiffType = decltype(std::declval<Type>() - std::declval<Type>());

/** Функция для решения методм  прогонки **/
template <typename mType, typename cType>
std::vector<DivisType<cType, mType>> solve(const ThreeDiagonalMatrix<mType> &matrix, const std::vector<cType> &column)
{
    std::vector<mType> q, p;
    q.resize(matrix.get_n());
    p.resize(matrix.get_n());
    // прямой ход
    p[0] = -matrix[0].c / matrix[0].b;
    q[0] = column[0] / matrix[0].b;
    for (long i = 1; i < matrix.get_n() - 1; ++i)
    {
        p[i] = -((matrix[i].c) / (matrix[i].a * p[i - 1] + matrix[i].b));
        q[i] = ((column[i] - matrix[i].a * q[i - 1]) / (matrix[i].a * p[i - 1] + matrix[i].b));
    }
    // обратный ход
    q[matrix.get_n() - 1] = (column[matrix.get_n() - 1] - matrix[matrix.get_n() - 1].a * q[matrix.get_n() - 2]) / (matrix[matrix.get_n() - 1].a * p[matrix.get_n() - 2] + matrix[matrix.get_n() - 1].b);
    for (long i = matrix.get_n() - 2; i > -1; --i)
    {
        q[i] += p[i] * q[i + 1];
    }
    return q;
};

template <typename xType, typename yType>
class CubicSpline
{
private:
    std::vector<yType> b, c, d, a;
    std::vector<xType> intPoints;

    using DeltaXType = DiffType<xType>;
    using DerivType = DivisType<DiffType<yType>, DeltaXType>;
    using Deriv2Type = DivisType<DiffType<DerivType>, DeltaXType>;

public:
    /*сплайн с заданными значениями вторых производных*/
    CubicSpline( const std::vector<xType> &points,  // Значения x
                        const std::vector<yType>& values,  // значения y
                        const Deriv2Type& first,  // значение для левой второй производной
                        const Deriv2Type& second  // значение для правой второй производной
                        ): intPoints{points}, a(values)
    {   
        std::vector<DiffType<xType>> hValues;
        std::vector<DiffType<yType>> uValues;
        b.resize(points.size() - 2);
        c.resize(points.size() - 2 , 2);
        d.resize(points.size() - 2);
        hValues.resize(points.size() - 1);
        uValues.resize(points.size() - 1);
        for (size_t i = 0; i < points.size() - 1; i++)
        {
            hValues[i] = points[i+1] - points[i];
        }
        for (size_t i = 0; i < points.size() - 1; i++)
        {
            uValues[i] = values[i+1] - values[i];
        }
        //points.size() это n+1
        for (std::size_t i = 0; i < points.size() - 3 ; i++)
        {
            d[i] = hValues[i + 1] / (points[i + 2] - points[i]);
            b[i] = hValues[i] / (points[i + 2] - points[i]);
        }
        ThreeDiagonalMatrix<yType> matrix(b, c, d);
        for (std::size_t i = 0; i < points.size() - 2; i++)
        {
            d[i] = 6 * (uValues[i+1] / hValues[i+1] - uValues[i] / hValues[i]) / (points[i + 2] - points[i]);
        }
        c = solve<yType, yType>(matrix, d);
        c.push_back(second);
        // c собрали, соберём b
        b.resize(points.size() - 1);
        b[0] = c[0] * hValues[0] / 3 + uValues[0] / hValues[0];
        for (std::size_t i = 1; i < points.size() - 1 ; i++)
        {
            b[i] = c[i] * hValues[i] / 3 + uValues[i] / hValues[i] + c[i - 1] * hValues[i] / 6;
        }
        // теперь d
        d.resize(points.size() - 1);
        d[0] = c[0] / hValues[0] + first;
        for (std::size_t i = 1; i < points.size() - 1; i++)
        {
            d[i] = (c[i] - c[i - 1]) / hValues[i];
        }
    };

    /*Естественный сплайн*/
    CubicSpline(const std::vector<xType> &points, // Значения x
                const std::vector<yType> &values )// значения y
        : CubicSpline(points, values, 0, 0) {};


    yType interpolate(const xType &x) const noexcept {
        for (std::size_t i = 1; i < b.size(); i++)
        {
            if(x <= intPoints[i])
            {
                yType result = d[i - 1] / 6;
                result = std::fma(result, (x - intPoints[i]), c[i - 1] / 2);
                result = std::fma(result, (x - intPoints[i]), b[i - 1]);
                result = std::fma(result, (x - intPoints[i]), a[i]);
                return result;
            }
        }
        yType result = d[b.size() - 1] / 6;
        result = std::fma(result, (x - intPoints[b.size()]), c[b.size() - 1] / 2);
        result = std::fma(result, (x - intPoints[b.size()]), b[b.size() - 1]);
        result = std::fma(result, (x - intPoints[b.size()]), a[b.size()]);
        return result;
    }; 
};
