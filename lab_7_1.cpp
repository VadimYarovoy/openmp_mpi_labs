// ЛАБОРАТОРНАЯ РАБОТА № 7.1
// Вариант: 5
// ФИО: Яровой Вадим Дмитриевич
// Группа: 5140904/40102

// Постановка задачи:

// ...
// double x[N], y[N];
// x[0] = 0;
//
// # pragma omp parallel for nowait
// for (i = 1; i < N; i++)
// {
//    x[i] = x[i - 1] * x[i - 1];
//    y[i] = x[i] / y[i - 1];
// }
//
// y[0] = x[N - 1];
// ...

#include <iostream>
#include <stdio.h>
#include <omp.h>

#include <cmath>

#define N 4

// Для случая, когда x[0] != 0
int powerOfTwo(int n) {
    if (n < 1) {
        std::cerr << "Ошибка: n должно быть больше или равно 1." << std::endl;
        return -1; // Возвращаем -1 в случае ошибки
    }
    return 1 << n; // Используем битовый сдвиг для получения 2^n
}

// Для случая, когда x[0] != 0
double getElementInSequence(double firstElement, int n) {
    if (n < 1) {
        std::cerr << "Ошибка: номер элемента должен быть больше или равен 1." << std::endl;
        return -1;
    }

    if (n == 1) {
        return firstElement;
    }

    double result = firstElement;

    for (int i = 1; i < n; i ++) {
        result = pow(2, powerOfTwo(i)) / result;
    }

    return result;
}

int main(int argc, char** argv)
{
    // ОШИБКА 1
    // Массив y не инициализирован перед использованием.
    // Это приводит к неопределенному поведению
    double x[N],y[N];

    // ---СЛУЧАЙ 1
    // x[0] = 0;

    // ---СЛУЧАЙ 2
    x[0] = 2;

    // ОШИБКА 1 - ИСПРАВЛЕНИЕ

    // ---СЛУЧАЙ 1
    // можно написать любое число так как оно ни на что не влияет
    // ведь x[0] = 0
    // y[0] = N;

    // ---СЛУЧАЙ 2
    y[0] = 3;

    // Вывод до обработки
    std::cout << "x: ";
    for (int i = 0; i < N; i++) {
        std::cout << x[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "y: ";
    for (int i = 0; i < N; i++) {
        std::cout << y[i] << " ";
    }
    std::cout << std::endl;

    // ОШИБКА 2
    // Тут нужно убрать "nowait" из-за ошибки компеляции
    // # pragma omp parallel for nowait
    // lab_7_1.cpp:14:31: error: unexpected OpenMP clause 'nowait' in directive '#pragma omp parallel for'
    //   14 |     # pragma omp parallel for nowait
    //      |                               ^
    # pragma omp parallel for
    // // Нету типа у переменной счетчика, добалено "int i = 1"
    for (int i = 1; i < N; i++) {
        // ОШИБКА 3
        // ---СЛУЧАЙ 1
        // x[i - 1] может быть не инициализирована к моменту обращения
        // Так как x[0] = 0, по можем заменить эти вычисления
        // x[i] = x[i - 1] * x[i - 1];
        // На следующее выражение
        // x[i] = 0;

        // ---СЛУЧАЙ 2
        // В противном случае, если x[0] != 0
        // Подключаем #include <cmath>
        x[i] = pow(x[0], powerOfTwo(i)); //Используем степени двойки 2, 4, 8, 16 ...

        // ОШИБКА 4
        // ---СЛУЧАЙ 1
        // y[i - 1] может быть не инициализирована к моменту обращения
        // y[i] = x[i] / y[i - 1];
        // а так как у нас x[0] == 0, то меняем на выражение
        // y[i] = 0;

        // ---СЛУЧАЙ 2
        // В противном случае нам нужно выносить выражение y[i] = x[i] / y[i - 1];
        // во второй простой цикл без многопоточной обработки
        y[i] = x[i] / getElementInSequence(y[0], i);
    }

    // ОШИБКА 2 - ДОПОЛНЕНИЕ
    // Еще момент, если не убрать "nowait", то этот код будет начат до завершения цикла,
    // тогда x[N - 1] с высокой вероятностью не будет инициализирован
    y[0] = x[N - 1];

    // Вывод результатов
    std::cout << "x: ";
    for (int i = 0; i < N; i++) {
        std::cout << x[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "y: ";
    for (int i = 0; i < N; i++) {
        std::cout << y[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}

// clang++ -fopenmp -o bin/lab_7_1 lab_7_1.cpp && ./bin/lab_7_1
