// ЛАБОРАТОРНАЯ РАБОТА № 7.2
// Вариант: 5
// ФИО: Яровой Вадим Дмитриевич
// Группа: 5140904/40102

// Постановка задачи:

// Определить ранг матрицы. Входные данные: целое положительное
// число n, произвольная матрица А размерности n х n. Решить задачу двумя
// способами:
// 1) количество потоков является входным параметром, при этом
//    размерность матриц может быть не кратна количеству потоков;
// 2) количество потоков заранее неизвестно и не является параметром задачи.

#include <iostream>
#include <vector>
#include <omp.h>


// Сложность O(n ^ 3)
// Приведем матрицу A с помощью элементарных преобразований к ступенчатому виду.
// Считаем кол-во оставшихся ненулевых строк
int gausMatrixRank(std::vector<std::vector<double>>& A, int n, int ths) {
    int rank = 0;

    for (int col = 0; col < n; ++col) {
        int pivotRow = -1;

        // Параллельная часть для поиска ненулевого элемента в столбце
        // !!! для задания 2 заменить на
        // !!! #pragma omp parallel for
        #pragma omp parallel for num_threads(ths)
        for (int row = rank; row < n; ++row) {
            if (A[row][col] != 0) {
                // Критическая секция, которая гарантирует,
                // что только один поток может изменять pivotRow в данный момент времени
                // Это предотвращает гонки данных.
                #pragma omp critical
                {
                    if (pivotRow == -1) {
                        // Если найден ненулевой элемент, его индекс сохраняется в pivotRow
                        pivotRow = row;

                        // Тут хорошо бы остановиться, но у нас нет такого механизма
                    }
                }
            }
        }

        // Если ненулевой элемент найден, выполняются следующие действия:
        if (pivotRow != -1) {
            // Обмен строк
            // Необходимо для того, чтобы ненулевой элемент оказался в верхней позиции.
            std::swap(A[rank], A[pivotRow]);

            // Обнуление элементов ниже главного элемента
            // !!! для задания 2 заменить на
            // !!! #pragma omp parallel for
            // Тут находиться самая эффективная часть для распараллеливания
            #pragma omp parallel for num_threads(ths)
            for (int row = rank + 1; row < n; ++row) {
                double factor = A[row][col] / A[rank][col];

                // Проходим по всем элементам строки, начиная с текущего столбца,
                // вычитаем из них соответствующее значение, умноженное на factor
                for (int j = col; j < n; ++j) {
                    A[row][j] -= factor * A[rank][j];
                }
            }

            // увеличиваем ранг на 1, так как мы успешно обработали одну строку
            rank++;
        }
    }
    return rank;
}

void printMatrix(std::vector<std::vector<double>>& A, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            std::cout << A[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

int main() {
    int n, ths;
    std::cout << "Номер тестовой матрицы n: ";
    std::cin >> n;
    std::cout << "Количество потоков: ";
    std::cin >> ths;

    // Тестовые матрицы, размерностью n x n
    std::vector<std::vector<std::vector<double>>> matrices = {
        {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}},   // (№1) все нули, значит ранг = 0
        {{1, -1, 2}, {0, 1, -1}, {1, 0, 1}}, // (№2) ранг = 2
        {{1, -1, 2}, {0, 1, -1}, {1, 1, 1}}, // (№3) ранг = 3
        {{1, -1, 2}, {0, 1, -1}, {1, 0, 1}}, // (№4) ранг = 2
    };

    // Выбор матрицы
    std::vector<std::vector<double>> A = matrices[n - 1];

    // printMatrix(A, A.size());
    // std::cout << "Используемая матрица:" << std::endl;
    // for (const auto& row : A) {
    //     for (double val : row) {
    //         std::cout << val << " ";
    //     }
    //     std::cout << std::endl;
    // }

    std::cout << std::endl << "Используемая матрица: " << std::endl;
    printMatrix(A, A.size());

    int rank = gausMatrixRank(A, A.size(), ths);

    std::cout << std::endl << "Ступенчатый вид: " << std::endl;
    printMatrix(A, A.size());

    std::cout << std::endl <<  "Ранг матрицы: " << rank << std::endl;

    return 0;
}

// clang++ -fopenmp -o bin/lab_7_2 lab_7_2.cpp && ./bin/lab_7_2
