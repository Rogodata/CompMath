#pragma once

#include <array>
#include <vector>
#include <eigen3/Eigen/Dense>

/* Это таблица Бутчера для метода Рунге-Кутты 4 порядка. Я ее не заполнил */
struct RK4Table
{
    static constexpr unsigned int stages = 4;
    static constexpr std::array<std::array<double, stages>, stages> table = {std::array<double, stages>{0, 0, 0, 0}, std::array<double, stages>{1. / 2, 0, 0, 0}, std::array<double, stages>{0, 1. / 2, 0, 0}, std::array<double, stages>{0, 0, 1., 0}};
    static constexpr std::array<double, stages> cColumn = {0, 1. / 2, 1. / 2, 1.};
    static constexpr std::array<double, stages> bString = {1. / 6, 1. / 3, 1. / 3, 1. / 6};
};

// Реализация класса правой части дифференциального уравнения. То есть класс f(t, y) для y' = f(t,y).
// Здесь написан пример для уравнения осциллятора (x, v)' = (v, -x)

class Oscillator
{

public:
    static constexpr unsigned int dim = 2; // размерность задачи

    using Argument = double; // тип аргумента, тип t

    using State = Eigen::Vector<double, dim>; // состояние

    struct StateAndArg
    {
        State state;
        Argument arg;
    };

    /*** Вычисляет правую часть ДУ - функцию f***/
    Eigen::Vector<double, dim> calc(const StateAndArg &stateAndArg) const
    {
        return Eigen::Vector<double, dim>{stateAndArg.state(1), -stateAndArg.state(0)};
    }
};

//класс для первого задания y' = t^3, второе задание это осциллятор
class FirstTask
{

public:
    static constexpr unsigned int dim = 1; // размерность задачи

    using Argument = double; // тип аргумента, тип t

    using State = double; // состояние

    struct StateAndArg
    {
        State state;
        Argument arg;
    };

    /*** Вычисляет правую часть ДУ - функцию f***/
    double calc(const StateAndArg &stateAndArg) const
    {
        return stateAndArg.arg * stateAndArg.arg * stateAndArg.arg;
    }
};

// Сигнатура для метода интегрирования:

template <typename Table, typename RHS> // таблица бутчера и класс правой части f
std::vector<typename RHS::StateAndArg> integrate(
    const typename RHS::StateAndArg &initialState,
    const typename RHS::Argument &endTime,
    const Table &butcher,
    double step,
    const RHS &rhs)
{
    std::vector<typename RHS::StateAndArg> answer{initialState};
    for (std::size_t i = 0; i < endTime / step; i++)
    {
        // реализуем явный метод - будем считать, что на диагонали матрицы Бутчера нули
        // вычисляем коэффициенты к

        std::array<typename RHS::State, butcher.stages> kCoeffs;
        kCoeffs[0] = rhs.calc(answer[i]);
        //вычисляем коэффициенты с к1 по кstages-1
        for (std::size_t j = 1; j < butcher.stages; j++)
        {
            typename RHS::State arg = answer[i].state;
            for (std::size_t k = 0; k < j; k++)
            {
                arg += butcher.table[j][k] * kCoeffs[k];
            }
            kCoeffs[j] = rhs.calc(typename RHS::StateAndArg{arg, step * (i + butcher.cColumn[j])});
        }
        //Коэффициенты к посчитали, теперь собираем следующий шаг
        typename RHS::State newY = butcher.bString[0] * kCoeffs[0];
        for (std::size_t j = 1; j < butcher.stages; j++)
        {
            newY += butcher.bString[j] * kCoeffs[j];
        }
        newY *= step;
        newY += answer[i].state;
        answer.push_back({newY, answer[i].arg + step});
    }
    return answer;
};
