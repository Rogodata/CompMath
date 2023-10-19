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
    ThreeDiagonalMatrix(std::vector<Type> a, std::vector<Type> b, std::vector<Type> c)
    {
        rows.resize(0);
        rows.push_back({0, b[0], c[0]});
        for (size_t i = 0; i < b.size() - 2; i++)
        {
            rows.push_back({a[i], b[i + 1], c[i + 1]});
        }
        rows.push_back({a[b.size() - 2], b[b.size() - 1], 0});
    }

    Row<Type> operator[](const std::size_t i_coord) const { return rows[i_coord]; }

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
    q[matrix.get_n() - 1] = (column[matrix.get_n() - 1] - matrix[matrix.get_n() - 1].a * q[matrix.get_n() - 2]) / (matrix[matrix.get_n() -1].a * p[matrix.get_n() - 2] + matrix[matrix.get_n() - 1].b);
    for (long i = matrix.get_n() - 2; i > -1; --i)
    {
        q[i] += p[i] * q[i + 1];
    }
    return q;
};



template <typename xType, typename yType>
class CubicSpline
{
  

public:
    CubicSpline(const std::vector<xType> &points, // Значения x
                const std::vector<yType> &values, // значения y
    ){};

    yType interpolate(const xType &x) const noexcept;
};
