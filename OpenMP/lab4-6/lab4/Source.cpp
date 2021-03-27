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

int get_max(int A, int B)
{
    if (A < B)
    {
        A = B;
    }
    return A;
}

result task_with_atomic(std::vector<int> A, std::vector<int> B)
{
    int sum = 0;
    int i;

    int num_threads = 0;
    double time = 0;

    //print_arr(A);
    //print_arr(B);
    std::cout << "Параллельное вычисление с атомарными операциями: " << std::endl;
    time = omp_get_wtime();
#pragma omp parallel shared(A,B)// num_threads(A.size())
    {
        num_threads = omp_get_num_threads();
#pragma omp for private(i) //reduction(+:sum)
        for (int i = 0; i < A.size(); i++)
        {
            A[i] = get_max(A[i], B[i]);
            #pragma omp atomic
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

result task_with_critical_section(std::vector<int> A, std::vector<int> B)
{
    int sum = 0;
    int i;

    int num_threads = 0;
    double time = 0;

    //print_arr(A);
    //print_arr(B);
    std::cout << "Параллельное вычисление с критическими секциями: " << std::endl;
    time = omp_get_wtime();
#pragma omp parallel shared(A,B)// num_threads(A.size())
    {
        num_threads = omp_get_num_threads();
#pragma omp for private(i) //reduction(+:sum)
        for (int i = 0; i < A.size(); i++)
        {
            A[i] = get_max(A[i], B[i]);
            #pragma omp critical
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

result task_without_parallel(std::vector<int> A, std::vector<int> B)
{
    int sum = 0;
    int i;

    int num_threads = 0;
    double time = 0;

    //print_arr(A);
    //print_arr(B);
    std::cout << "Однопоточное вычисление: " << std::endl;
    time = omp_get_wtime();
    num_threads = omp_get_num_threads();
    for (int i = 0; i < A.size(); i++)
    {
        A[i] = get_max(A[i], B[i]);
        sum += A[i];
    }

    time = omp_get_wtime() - time;
    std::cout << "Количество созданных потоков: " << num_threads << std::endl;
    std::cout << "Затраченное время: " << time << std::endl;
    result res;
    res.arr = A;
    res.sum = sum;
    return res;
}



result task_two_sections_parallel(std::vector<int> A, std::vector<int> B)
{
    int threads_count;

    int n = A.size();

    int sum, currentResult, i;
    int current_results[2];

    double time_parallel_two_sections;

    for (int i = 0; i < 2; i++)
    {
        current_results[i] = 0;
    }
    double start = omp_get_wtime();
#pragma omp parallel sections private(i) shared(A, B)
    {
        threads_count = omp_get_num_threads();
#pragma omp section
        {
            for (i = 0; i < n / 2; i++)
            {
                current_results[0] += get_max(A[i], B[i]);
            }
        }
#pragma omp section
        {
            for (i = n / 2; i < n; i++)
            {
                current_results[1] += get_max(A[i], B[i]);
            }
        }
    }
    for (i = 0, sum = 0; i < 2; i++)
    {
        sum += current_results[i];
    }
    time_parallel_two_sections = omp_get_wtime() - start;
    std::cout << "Вычисление в двух секциях: " << std::endl;
    std::cout << "Затраченное время: " << time_parallel_two_sections << std::endl;
    result res;
    res.arr = A;
    res.sum = sum;
    return res;
}

result task_three_sections_parallel(std::vector<int> A, std::vector<int> B)
{
    int threads_count;

    int n = A.size();

    int sum, currentResult, i;
    int current_results[3];

    double time_parallel_two_sections;

    for (int i = 0; i < 3; i++)
    {
        current_results[i] = 0;
    }
    double start = omp_get_wtime();
#pragma omp parallel sections private(i) shared(A, B)
    {
        threads_count = omp_get_num_threads();
#pragma omp section
        {
            for (i = 0; i < n / 3; i++)
            {
                current_results[0] += get_max(A[i], B[i]);
            }
        }
#pragma omp section
        {
            for (i = n / 3; i < 2 * n / 3; i++)
            {
                current_results[1] += get_max(A[i], B[i]);
            }
        }
#pragma omp section
        {
            for (i = 2 * n / 3; i < n; i++)
            {
                current_results[2] += get_max(A[i], B[i]);
            }
        }
    }
    for (i = 0, sum = 0; i < 3; i++)
    {
        sum += current_results[i];
    }
    time_parallel_two_sections = omp_get_wtime() - start;
    std::cout << "Вычисление в трех секциях: " << std::endl;
    std::cout << "Затраченное время: " << time_parallel_two_sections << std::endl;
    result res;
    res.arr = A;
    res.sum = sum;
    return res;
}

result task_four_sections_parallel(std::vector<int> A, std::vector<int> B)
{
    int threads_count;

    int n = A.size();

    int sum, currentResult, i;
    int current_results[4];

    double time_parallel_two_sections;

    for (int i = 0; i < 4; i++)
    {
        current_results[i] = 0;
    }
    double start = omp_get_wtime();
#pragma omp parallel sections private(i) shared(A, B)
    {
        threads_count = omp_get_num_threads();
#pragma omp section
        {
            for (i = 0; i < n / 4; i++)
            {
                current_results[0] += get_max(A[i], B[i]);
            }
        }
#pragma omp section
        {
            for (i = n / 4; i < 2 * n / 4; i++)
            {
                current_results[1] += get_max(A[i], B[i]);
            }
        }
#pragma omp section
        {
            for (i = 2 * n / 4; i < 3 * n / 4; i++)
            {
                current_results[2] += get_max(A[i], B[i]);
            }
        }
#pragma omp section
        {
            for (i = 3 * n / 4; i < n; i++)
            {
                current_results[3] += get_max(A[i], B[i]);
            }
        }
    }
    for (i = 0, sum = 0; i < 4; i++)
    {
        sum += current_results[i];
    }
    time_parallel_two_sections = omp_get_wtime() - start;
    std::cout << "Вычисление в четырех секциях: " << std::endl;
    std::cout << "Затраченное время: " << time_parallel_two_sections << std::endl;
    result res;
    res.arr = A;
    res.sum = sum;
    return res;
}

result task_six_sections_parallel(std::vector<int> A, std::vector<int> B)
{
    int threads_count;

    int n = A.size();

    int sum, currentResult, i;
    int current_results[6];

    double time_parallel_two_sections;

    for (int i = 0; i < 6; i++)
    {
        current_results[i] = 0;
    }
    double start = omp_get_wtime();
#pragma omp parallel sections private(i) shared(A, B)
    {
        threads_count = omp_get_num_threads();
#pragma omp section
        for (i = 0; i < n / 6; i++)
        {
            current_results[0] += get_max(A[i], B[i]);
        }
#pragma omp section
        for (i = n / 6; i < 2 * n / 6; i++)
        {
            current_results[1] += get_max(A[i], B[i]);
        }
#pragma omp section
        for (i = 2 * n / 6; i < 3 * n / 6; i++)
        {
            current_results[2] += get_max(A[i], B[i]);
        }
#pragma omp section
        for (i = 3 * n / 6; i < 4 * n / 6; i++)
        {
            current_results[3] += get_max(A[i], B[i]);
        }
#pragma omp section
        for (i = 4 * n / 6; i < 5 * n / 6; i++)
        {
            current_results[4] += get_max(A[i], B[i]);
        }
#pragma omp section
        for (i = 5 * n / 6; i < n; i++)
        {
            current_results[5] += get_max(A[i], B[i]);
        }
    }
    for (i = 0, sum = 0; i < 6; i++)
    {
        sum += current_results[i];
    }
    time_parallel_two_sections = omp_get_wtime() - start;
    std::cout << "Вычисление в шести секциях: " << std::endl;
    std::cout << "Затраченное время: " << time_parallel_two_sections << std::endl;
    result res;
    res.arr = A;
    res.sum = sum;
    return res;
}


result task_twelve_sections_parallel(std::vector<int> A, std::vector<int> B)
{
    int threads_count;

    int n = A.size();

    int sum, current_result, i;
    int current_results[12];

    double time_parallel_two_sections;

    for (int i = 0; i < 12; i++)
    {
        current_results[i] = 0;
    }
    double start = omp_get_wtime();
#pragma omp parallel sections private(i) shared(A, B)
    {
        threads_count = omp_get_num_threads();
#pragma omp section
        for (i = 0; i < n / 12; i++)
        {
            current_results[0] += get_max(A[i], B[i]);
        }
#pragma omp section
        for (i = n / 12; i < 2 * n / 12; i++)
        {
            current_results[1] += get_max(A[i], B[i]);
        }
#pragma omp section
        for (i = 2 * n / 12; i < 3 * n / 12; i++)
        {
            current_results[2] += get_max(A[i], B[i]);
        }
#pragma omp section
        for (i = 3 * n / 12; i < 4 * n / 12; i++)
        {
            current_results[3] += get_max(A[i], B[i]);
        }
#pragma omp section
        for (i = 4 * n / 12; i < 5 * n / 12; i++)
        {
            current_results[4] += get_max(A[i], B[i]);
        }
#pragma omp section
        for (i = 5 * n / 12; i < 6 * n / 12; i++)
        {
            current_results[5] += get_max(A[i], B[i]);
        }
#pragma omp section
        for (i = 6 * n / 12; i < 7 * n / 12; i++)
        {
            current_results[6] += get_max(A[i], B[i]);
        }
#pragma omp section
        for (i = 7 * n / 12; i < 8 * n / 12; i++)
        {
            current_results[7] += get_max(A[i], B[i]);
        }
#pragma omp section
        for (i = 8 * n / 12; i < 9 * n / 12; i++)
        {
            current_results[8] += get_max(A[i], B[i]);
        }
#pragma omp section
        for (i = 9 * n / 12; i < 10 * n / 12; i++)
        {
            current_results[9] += get_max(A[i], B[i]);
        }
#pragma omp section
        for (i = 10 * n / 12; i < 11 * n / 12; i++)
        {
            current_results[10] += get_max(A[i], B[i]);
        }
#pragma omp section
        for (i = 11 * n / 12; i < n; i++)
        {
            current_results[11] += get_max(A[i], B[i]);
        }
    }
    for (i = 0, sum = 0; i < 12; i++)
    {
        sum += current_results[i];
    }
    time_parallel_two_sections = omp_get_wtime() - start;
    std::cout << "Вычисление в двенадцати секциях: " << std::endl;
    std::cout << "Затраченное время: " << time_parallel_two_sections << std::endl;
    result res;
    res.arr = A;
    res.sum = sum;
    return res;
}

result task_with_locks(std::vector<int> A, std::vector<int> B)
{
    int sum = 0;
    int i;
    omp_lock_t lock;
 
    int num_threads = 0;
    double time = 0;

    //print_arr(A);
    //print_arr(B);
    std::cout << "Параллельное вычисление с применением замков: " << std::endl;
    time = omp_get_wtime();
    omp_init_lock(&lock);
#pragma omp parallel shared(A,B)// num_threads(A.size())
    {
        num_threads = omp_get_num_threads();
#pragma omp for private(i) //reduction(+:sum)
        for (int i = 0; i < A.size(); i++)
        {
            A[i] = get_max(A[i], B[i]);
            omp_set_lock(&lock);
            sum += A[i];
            omp_unset_lock(&lock);
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

int get_res(int current_results[])
{
    int res = 0;
    for (int i = 0; i < 8; i++)
    {
        res += current_results[i];
    }
    return res;
}

result task_with_barrier(std::vector<int> A, std::vector<int> B)
{
    int sum = 0;
    int i;
    int current_results[8];
    for (int i = 0; i < 8; i++)
    {
        current_results[i] = 0;
    }

    int num_threads = 0;
    double time = 0;

    //print_arr(A);
    //print_arr(B);
    std::cout << "Параллельное вычисление с применением барьера: " << std::endl;
    time = omp_get_wtime();
#pragma omp parallel shared(A,B)// num_threads(A.size())
    {
        num_threads = omp_get_num_threads();
#pragma omp for private(i) //reduction(+:sum)
        for (int i = 0; i < A.size(); i++)
        {
            current_results[omp_get_thread_num()] += get_max(A[i], B[i]);
        }
#pragma omp barrier
        sum = get_res(current_results);
    }
    time = omp_get_wtime() - time;
    std::cout << "Количество созданных потоков: " << num_threads << std::endl;
    std::cout << "Затраченное время: " << time << std::endl;
    result res;
    res.arr = A;
    res.sum = sum;
    return res;
}

void main()
{
    setlocale(LC_ALL, "Rus");
    //srand(time(NULL));
    int n = 0;

    
    std::cout << "Введите размерность массивов: ";
    std::cin >> n;
    std::cout << std::endl;
    std::cout << std::endl;

    std::vector<int> A = rand_arr(A, n);
    std::vector<int> B = rand_arr(B, n);
 
    
    result res = task_with_atomic(A, B);
    //std::cout << "Массив: ";  print_arr(res.arr); 
    std::cout << "Сумма: " << res.sum << std::endl;
    std::cout << std::endl;

    res = task_with_critical_section(A, B);
    //std::cout << "Массив: ";  print_arr(res.arr); 
    std::cout << "Сумма: " << res.sum << std::endl;
    std::cout << std::endl;

    res = task_without_parallel(A, B);
    //std::cout << "Массив: ";  print_arr(res.arr); 
    std::cout << "Сумма: " << res.sum << std::endl;
    std::cout << std::endl;
;
    res = task_two_sections_parallel(A, B);
    //std::cout << "Массив: ";  print_arr(res.arr); 
    std::cout << "Сумма: " << res.sum << std::endl;
    std::cout << std::endl;

    res = task_three_sections_parallel(A, B);
    //std::cout << "Массив: ";  print_arr(res.arr); 
    std::cout << "Сумма: " << res.sum << std::endl;
    std::cout << std::endl;

    res = task_four_sections_parallel(A, B);
    //std::cout << "Массив: ";  print_arr(res.arr); 
    std::cout << "Сумма: " << res.sum << std::endl;
    std::cout << std::endl;

    res = task_six_sections_parallel(A, B);
    //std::cout << "Массив: ";  print_arr(res.arr); 
    std::cout << "Сумма: " << res.sum << std::endl;
    std::cout << std::endl;

    res = task_twelve_sections_parallel(A, B);
    //std::cout << "Массив: ";  print_arr(res.arr); 
    std::cout << "Сумма: " << res.sum << std::endl;
    std::cout << std::endl;
    
    res = task_with_locks(A, B);
    //std::cout << "Массив: ";  print_arr(res.arr); 
    std::cout << "Сумма: " << res.sum << std::endl;
    std::cout << std::endl;

    res = task_with_barrier(A, B);
    //std::cout << "Массив: ";  print_arr(res.arr); 
    std::cout << "Сумма: " << res.sum << std::endl;
    std::cout << std::endl;
    
}