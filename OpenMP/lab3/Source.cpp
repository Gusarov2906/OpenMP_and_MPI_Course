#include <iostream>
#include <vector>
#include <ctime>
#include <omp.h>

struct result
{
    std::vector<int> arr;
    int sum;
};

std::vector<int> rand_arr(std::vector<int> arr, int n)
{

    for (int j = 0; j < n; j++)
    {
        arr.push_back(rand() % 10);
    }

    return arr;
}

void print_arr(std::vector<int> arr)
{
    for (int i = 0; i < arr.size(); i++)
    {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}

result task1_with_paller(std::vector<int> A, std::vector<int> B)
{
    int sum = 0;
    int i;

    int num_threads = 0;
    double time = 0;

    //print_arr(A);
    //print_arr(B);
    std::cout << "Параллельное вычисление: " << std::endl;
    time = omp_get_wtime();
    #pragma omp parallel shared(A,B)// num_threads(A.size())
    {
        num_threads = omp_get_num_threads();
    #pragma omp for private(i) reduction(+:sum)
        for (int i = 0; i < A.size(); i++)
        {
            if (A[i] < B[i])
            {
                A[i] = B[i];
            }
            sum += A[i];
        }
    } 
    time = omp_get_wtime() - time;
    std::cout << "Количество созданных потоков: " << num_threads << std::endl;
    std::cout << "Затраченное время: " << time << std::endl;
    result res;
    res.arr = A;
    res.sum = sum;
    return res;
}

result task1_without_paller(std::vector<int> A, std::vector<int> B)
{
    int sum = 0;
    int i;

    int num_threads = 0;
    double time = 0;

    //print_arr(A);
    //print_arr(B);
    std::cout << "Однопоточное вычисление: " << std::endl;
    time = omp_get_wtime();

    for (int i = 0; i < A.size(); i++)
    {
        if (A[i] < B[i])
        {
            A[i] = B[i];
        }
        sum += A[i];
    }
    
    time = omp_get_wtime() - time;
    std::cout << "Затраченное время: " << time << std::endl;
    result res;
    res.arr = A;
    res.sum = sum;
    return res;
}

void main()
{
    setlocale(LC_ALL, "Rus");
    srand(time(NULL));
    int n = 0;
  
    std::cout << "Введите размерность массивов: ";
    std::cin >> n;

    std::vector<int> A = rand_arr(A, n);
    std::vector<int> B = rand_arr(B, n);
    
    result res = task1_with_paller(A, B);
    //std::cout << "Массив: ";  print_arr(res.arr); 
    std::cout << "Сумма: " << res.sum << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    res = task1_without_paller(A, B);
    //std::cout << "Массив: ";  print_arr(res.arr); 
    std::cout << "Сумма: " << res.sum << std::endl;
}