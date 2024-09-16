// ЛАБОРАТОРНАЯ РАБОТА № 9
// Вариант: 5
// ФИО: Яровой Вадим Дмитриевич
// Группа: 5140904/40102

// Постановка задачи:

// Криптономикон-2. Дружественный секретный институт No 721, рас-
// положенный где-то на Урале разработал новейшую систему дешифрования
// новейшей системы шифрования, разработанной в секретном институте No
// 127, находящемся где-то в Сибири. Решить задачу, обратную задаче 4, напи-
// сав программу-дешифровщик. Взяв в качестве входного сообщения шифро-
// ванный текст и ключ, она должна восстанавливать исходный текст.

// Дополнительная информация

// 4. Криптономикон. В секретном институте No 127, находящемся где-то
// в Сибири, разработана новейшая система шифровки данных для малого биз-
// неса. Система представляет собой ряд модулей, каждый из которых прини-
// мает на вход часть сообщения и шифрует его по определенной схеме. Какую
// именно часть текста шифрует каждый из модулей, задается специальным
// ключом, который меняется каждый день. Написать программу, моделирую-
// щую работу шифровальной системы. В качестве шифров использовать пря-
// мые подстановки (вид шифра, когда каждому символу исходного текста ста-
// вится в соответствие какой-то символ шифрованного текста, причем всегда
// один и тот же). В качестве ключа использовать подстановку, нижняя строка
// которой задает номер модуля, текст делится на равные части, весь остаток
// получает тот модуль, который шифрует последнюю часть. Использовать ме-
// тод передачи информации «точка-точка».

#include <iostream>
#include <mpi.h>
#include <string>
#include <vector>

using namespace std;

// Функция для дешифровки части сообщения
string decrypt_part(const string& part, const string& key) {
    string decrypted_part;
    for (char c : part) {
        // Проверяем, является ли символ буквой
        if (isalpha(c)) {
            // Ищем позицию в ключе
            size_t pos = key.find(c);
            // Если нашли - делаем подстановку
            if (pos != string::npos) {
                decrypted_part += 'a' + pos;  // Восстанавливаем букву
            } else {
                decrypted_part += c;  // Оставляем без изменений
            }
        } else {
            // Оставляем все, что не знаем как заменить (пробелы, знаки)
            decrypted_part += c;
        }
    }
    return decrypted_part;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    string encrypted_text;
    string key = "zyxwvutsrqponmlkjihgfedcba"; // Наш ключ
    int part_size;
    string local_encrypted_part;
    string decrypted_part;

    if (rank == 0) {
        // Процесс 0: Ввод шифрованного сообщения

        // Используем getline для поддержки пробелов
        getline(cin, encrypted_text);

        // Вычисляем размер частей для каждого процесса
        part_size = encrypted_text.length() / (size - 1);
        int remainder = encrypted_text.length() % (size - 1);

        // Отправляем части текста другим процессам
        for (int i = 1; i < size; ++i) {
            int start_idx = (i - 1) * part_size + min((i - 1), remainder);
            int end_idx = start_idx + part_size + ((i - 1) < remainder ? 1 : 0);
            local_encrypted_part = encrypted_text.substr(start_idx, end_idx - start_idx);
            MPI_Send(local_encrypted_part.c_str(), local_encrypted_part.size(), MPI_CHAR, i, 0, MPI_COMM_WORLD);
        }
    } else {
        // Остальные процессы получают свою часть сообщения
        MPI_Status status;

        // Некоторое ограничение
        char buffer[100];
        MPI_Recv(buffer, 100, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
        int count;
        MPI_Get_count(&status, MPI_CHAR, &count);
        local_encrypted_part = string(buffer, count);
    }

    // Все процессы выполняют дешифровку своей части, кроме нулевого
    if (rank != 0) {
        decrypted_part = decrypt_part(local_encrypted_part, key);
    }

    // Процесс 0 собирает результаты
    if (rank == 0) {
        string decrypted_text = decrypted_part;
        for (int i = 1; i < size; ++i) {
            char buffer[100];
            MPI_Status status;
            MPI_Recv(buffer, 100, MPI_CHAR, i, 0, MPI_COMM_WORLD, &status);
            int count;
            MPI_Get_count(&status, MPI_CHAR, &count);
            decrypted_text += string(buffer, count);
        }
        cout << "Decrypted text: " << decrypted_text << endl;
    } else {
        // Остальные процессы отправляют свои дешифрованные части процессу 0
        MPI_Send(decrypted_part.c_str(), decrypted_part.size(), MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}

// Пример использования

// 1.ЗАПУСК
// mpic++ -o bin/lab_9 lab_9.cpp && mpirun -np 4 ./bin/lab_9

// 2. ВАЖНО
// аргумент "-np" должен быть не меньше 2,
// так как процесс который раздает задачи не учавствует в их
// обработке и будет ошибка деления на 0

// 3.ПРИМЕР ВВОДА
// svool dliow (оно же "hello world")