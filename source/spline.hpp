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

template <typename numeratorType, typename denominatorType>
using DivisType = decltype(std::declval<numeratorType>() / std::declval<denominatorType>());

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

public:
    CubicSpline(const std::vector<xType> &points, // Значения x
                const std::vector<yType> &values )// значения y
    : intPoints{points}, a(values)
    {
        std::vector<yType> aCoeff, bCoeff, cCoeff;
        aCoeff.resize(points.size() - 2);
        bCoeff.resize(points.size() - 2 , 2);
        cCoeff.resize(points.size() - 2);
        b.resize(points.size() - 1);
        c.resize(points.size() - 1);
        d.resize(points.size() - 1);
        //points.size() это n+1
        for (std::size_t i = 0; i < points.size() - 3 ; i++)
        {
            cCoeff[i] = (points[i + 2] - points[i + 1]) / (points[i + 2] - points[i]);
            aCoeff[i] = (points[i + 1] - points[i]) / (points[i + 2] - points[i]);
        }
        ThreeDiagonalMatrix<yType> matrix(aCoeff, bCoeff, cCoeff);
        for (std::size_t i = 0; i < points.size() - 2; i++)
        {
            cCoeff[i] = 6 * ((values[i + 2] - values[i + 1]) / (points[i + 2] - points[i + 1]) - (values[i + 1] - values[i]) / (points[i + 1] - points[i])) / (points[i + 2] - points[i]);
        }
        bCoeff = solve<yType, yType>(matrix, cCoeff);
        bCoeff.push_back(0);
        c = bCoeff;
        // c собрали, соберём b
        b[0] = c[0] * (points[1] - points[0]) / 3 + (values[1] - values[0]) / (points[1] - points[0]);
        for (std::size_t i = 1; i < points.size() - 1 ; i++)
        {
            b[i] = c[i] * (points[i + 1] - points[i]) / 3 + (values[i + 1] - values[i]) / (points[i + 1] - points[i]) + c[i - 1] * (points[i + 1] - points[i]) / 6;
        }
        // теперь d
        d[0] = c[0] / (points[1] - points[0]);
        for (std::size_t i = 1; i < points.size() - 1; i++)
        {
            d[i] = (c[i] - c[i - 1]) / (points[i + 1] - points[i]);
        }
        //теперь оно собирает векторы правильно
    };

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
