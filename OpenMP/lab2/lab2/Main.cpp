#include <omp.h>
#include <iostream>
const int NMAX[10] = { 10, 10, 500, 500, 100, 100, 2000, 2000, 15000, 15000 };
const int LIMIT[10] = { 50, 5, 800, 40, 200, 10, 450, 10, 800000, 2000 };

void main()
{
    setlocale(LC_ALL, "Rus");
    int i, j;
    float sum;
    double times[10];
    int num_threads[10];
        for (int n = 0; n < 10; n++)
        {


        float** a = new float* [NMAX[n]];
        for (i = 0; i < NMAX[n]; i++)
        {
            a[i] = new float[NMAX[n]];
            for (j = 0; j < NMAX[n]; j++)
                a[i][j] = i + j;
        }
        double start_time = omp_get_wtime();
#pragma omp parallel shared(a) if (NMAX[n]>LIMIT[n])
        {
            num_threads[n] = omp_get_num_threads();
#pragma omp for private(i,j,sum) 
            for (i = 0; i < NMAX[n]; i++)
            {
                sum = 0;
                for (j = 0; j < NMAX[n]; j++)
                    sum += a[i][j];
                printf("Сумма элементов строки %d равна %f\n", i, sum);
            }
        } /* Завершение параллельного фрагмента */
        start_time = omp_get_wtime() - start_time;
        times[n] = start_time;
    }
    for (int i = 0; i < 10; i++)
        std::cout << "NMAX = " << NMAX[i] << " LIMIT = " << LIMIT[i] << " TIME = " << times[i] << " NUM THREADS: "<< num_threads[i]<< std::endl;
}