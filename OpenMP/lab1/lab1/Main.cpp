#include <omp.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>

int get_sevens_in_num(int num)
{
    int count = 0;
    while (num > 0)
    {
        if (num % 10 == 7)
        {
            count++;
        }
        num /= 10;
    }
    return count;
}

int get_num_pairwise_sum_with_seven_in_line(std::vector<int> line)
{
    int num = 0;
    for (int i = 0; i < line.size()-1; i++)
    {
        for (int j = i + 1; j < line.size(); j++)
        {
            num += get_sevens_in_num(line[i] + line[j]);
        }
    }
    return num;
}

void get_num_pairwise_sum_with_seven_in_array(std::vector<std::vector<int>> arr)
{
    for (int i = 0; i < arr.size(); i++)
    {
        int num = get_num_pairwise_sum_with_seven_in_line(arr[i]);
        //std::cout << "Строка №" << i << " Количество семерок: " << num << std::endl;
    }
}

std::vector<std::vector<int>> rand_arr(std::vector<std::vector<int>> arr, int m, int n)
{
    for (int i = 0; i < m; i++)
    {
        std::vector<int> row;
        for (int j = 0; j < n; j++)
        {
            row.push_back(rand() % 10);
        }
        arr.push_back(row);
    }
    return arr;
}

void print_arr(std::vector<std::vector<int>> arr)
{
    for (int i = 0; i < arr.size(); i++)
    {
        for (int j = 0; j < arr[i].size(); j++)
            std::cout << arr[i][j] << " ";
        std::cout << std::endl;
    }
}


int main()
{
    setlocale(LC_ALL, "Rus");
    srand(time(NULL));
    int m = 0, n = 0;
    std::cout << "Матрица MxN: " << std::endl;
    std::cout << "Напишите M: ";
    std::cin >> m;
    std::cout << "Напишите N: ";
    std::cin >> n;

    std::vector<std::vector<int>> arr;
    arr = rand_arr(arr,m,n);
    //print_arr(arr);
    
    int num = 0;
    double time = 0;

    time = omp_get_wtime();
    for (int i = 0; i < arr.size(); i++)
    {
        int num = get_num_pairwise_sum_with_seven_in_line(arr[i]);
        //std::cout << "Строка №" << i << " Количество семерок: " << num << std::endl;
    }
    time = omp_get_wtime() - time;
    std::cout << "Затраченное время: " << time << " Количество потоков: " << "1"<< std::endl;

    int num_threads = 0;
    time = omp_get_wtime();
#pragma omp parallel shared(arr) num_threads(6)
    {
        num_threads = omp_get_num_threads();
#pragma omp for
            for (int i = 0; i < arr.size(); i++)
            {
                int num = get_num_pairwise_sum_with_seven_in_line(arr[i]);
                //std::cout << "Строка №" << i << " Количество семерок: " << num << std::endl;
        }
    }
    time = omp_get_wtime() - time;
    std::cout << "Затраченное время: " << time <<" Количество потоков: "<< num_threads<< std::endl;
    return 0;
}

