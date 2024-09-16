lab6:
    clang++ -fopenmp -o bin/lab_6 lab_6.cpp && ./bin/lab_6 4 2 3 4 5 6 7 8 9 10 11 12 13 14 15

# clang++ -fopenmp -fsanitize=thread -o bin/lab_7_1 lab_7_1.cpp - флаг для проверки гонки данных
lab71:
    clang++ -fopenmp -o bin/lab_7_1 lab_7_1.cpp && ./bin/lab_7_1
lab72:
    clang++ -fopenmp -o bin/lab_7_2 lab_7_2.cpp && ./bin/lab_7_2
lab9:
    mpic++ -o bin/lab_9 lab_9.cpp && mpirun -np 2 ./bin/lab_9
lab11:
    mpic++ -o bin/lab_10_11 lab_10_11.cpp && mpirun -np 4 ./bin/lab_10_11
lab11o:
    mpic++ -o bin/lab_10_11 lab_10_11_own.cpp && mpirun -np 4 ./bin/lab_10_11
lab11m:
    mpic++ -o bin/lab_10_11 lab_10_11_mpi.cpp && mpirun -np 4 ./bin/lab_10_11