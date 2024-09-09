// ЛАБОРАТОРНАЯ РАБОТА № 6
// Вариант: 5
// ФИО: Яровой Вадим Дмитриевич
// Группа: 5140904/40102


// Постановка задачи:

// Дана последовательность натуральных чисел {a0...an–1}. Создать
// OpenMP-приложение для поиска всех ai, являющихся простыми числами.
// Количество потоков является входным параметром программы, потоки про-
// водят вычисления независимо друг от друга, количество символов в строке
// может быть не кратно количеству потоков.

#include <iostream>
#include <vector>
#include <omp.h>

// Функция для проверки, является ли число простым
bool is_prime(int n) {
    if (n <= 1) return false;
    for (int i = 2; i * i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
}

int main(int argc, char* argv[]) {
    // Для обработки ошибок
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <num_threads> <num1> <num2> ... <numN>" << std::endl;
        return 1;
    }

    // Получаем кол-во потоков первым аргументом
    int num_threads = std::stoi(argv[1]);
    std::vector<int> numbers;

    // Считываем последовательность чисел из аргументов командной строки
    for (int i = 2; i < argc; ++i) {
        numbers.push_back(std::stoi(argv[i]));
    }

    // Параллельный цикл с использованием прагмы
    #pragma omp parallel for num_threads(num_threads)
    for (int i = 0; i < numbers.size(); ++i) {
        if (is_prime(numbers[i])) {
            // критическая секция, чтобы терминал не рвался
            #pragma omp critical
            {
                std::cout << numbers[i] << " is prime" << std::endl;
            }
        }
    }

    return 0;
}
// clang++ -fopenmp -o bin/lab_6 lab_6.cpp && ./bin/lab_6 4 2 3 4 5 6 7 8 9 10 11 12 13 14 15
