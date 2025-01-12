#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

#define WYMIAR 4

int main() {
    double a[WYMIAR][WYMIAR];
    double suma = 0.0;

    // Inicjalizacja macierzy
    for (int i = 0; i < WYMIAR; i++) {
        for (int j = 0; j < WYMIAR; j++) {
            a[i][j] = 1.02 * i + 1.01 * j;
        }
    }

    // Sekwencyjne obliczenie sumy
    for (int i = 0; i < WYMIAR; i++) {
        for (int j = 0; j < WYMIAR; j++) {
            suma += a[i][j];
        }
    }
    printf("Suma (sekwencyjnie): %lf\n", suma);
    //omp_set_nested(1);


//    // Dekomp. 1: Dekompozycja wierszowa (zrównoleglenie pętli zewnętrznej)
    printf("\n--- Dekomp. 1: Wierszowa, pętla zewnętrzna ---\n");
    double suma_parallel_1 = 0.0;
    #pragma omp parallel for reduction(+:suma_parallel_1) schedule(static, 2) ordered
    for (int i = 0; i < WYMIAR; i++) {
        for (int j = 0; j < WYMIAR; j++) {
            suma_parallel_1 += a[i][j];
            #pragma omp ordered
            printf("W1 Wątek %d obsługuje element (%d, %d)\n", omp_get_thread_num(), i, j);
        }
    }
    printf("Suma (dekomp. 1): %lf\n", suma_parallel_1);

    // Dekomp. 2: Dekompozycja kolumnowa (zrównoleglenie pętli wewnętrznej)
    printf("\n--- Dekomp. 2: Kolumnowa, pętla wewnętrzna ---\n");
    double suma_parallel_2 = 0.0;
    for (int i = 0; i < WYMIAR; i++) {
        #pragma omp parallel for reduction(+:suma_parallel_2) schedule(dynamic) ordered
        for (int j = 0; j < WYMIAR; j++) {
            suma_parallel_2 += a[i][j];
            #pragma omp ordered
            printf("W2 Wątek %d obsługuje element (%d, %d)\n", omp_get_thread_num(), i, j);
        }
    }
    printf("Suma (dekomp. 2): %lf\n", suma_parallel_2);

    // Dekomp. 3: Dekompozycja kolumnowa (pętla zewnętrzna iteruje po kolumnach)
    printf("\n--- Dekomp. 3: Kolumnowa, pętla zewnętrzna ---\n");
    double suma_parallel_3 = 0.0;
    #pragma omp parallel
    {
        double lokalna_suma = 0.0;
        #pragma omp for schedule(dynamic)
        for (int j = 0; j < WYMIAR; j++) {
            for (int i = 0; i < WYMIAR; i++) {
                lokalna_suma += a[i][j];
                printf("W3 Wątek %d obsługuje element (%d, %d)\n", omp_get_thread_num(), i, j);
            }
        }
        #pragma omp critical
        {
            suma_parallel_3 += lokalna_suma;
        }
    }
    printf("Suma (dekomp. 3): %lf\n", suma_parallel_3);

    // Dekomp. 4: Dekompozycja wierszowa, pętla zewnętrzna, ręczne sterowanie
    printf("\n--- Dekomp. 4: Wierszowa, ręczne sterowanie ---\n");
    double suma_parallel_4 = 0.0;
    #pragma omp parallel
    {
        double lokalna_suma = 0.0;
        #pragma omp for schedule(static, 2)
        for (int i = 0; i < WYMIAR; i++) {
            for (int j = 0; j < WYMIAR; j++) {
                lokalna_suma += a[i][j];
                printf("W4 Wątek %d obsługuje element (%d, %d)\n", omp_get_thread_num(), i, j);
            }
        }
        #pragma omp critical
        {
            suma_parallel_4 += lokalna_suma;
        }
    }
    printf("Suma (dekomp. 4): %lf\n", suma_parallel_4);

      // Dekomp. 5: Dekompozycja 2D (zagnieżdżona równoległość) na 4.5
    printf("\n--- Dekomp. 5: 2D zagnieżdżona ---\n");
    double suma_parallel_5 = 0.0;
    omp_set_nested(1);
    #pragma omp parallel for reduction(+:suma_parallel_5) schedule(static, 2) num_threads(3)
    for (int i = 0; i < WYMIAR; i++) {
        #pragma omp parallel for reduction(+:suma_parallel_5) schedule(static, 2) num_threads(2) firstprivate(i)
        for (int j = 0; j < WYMIAR; j++) {
            printf("W5 Wątek (%d,%d) obsługuje element (%d, %d)\n", omp_get_thread_num(), i, j, omp_get_thread_num());
            //int id_w = omp_get_thread_num();
            //printf("(%1d,%1d)-W_%1d,%1d ",i,j,id_w,omp_get_thread_num());
            suma_parallel_5 += a[i][j];
        }
    }
    printf("Suma (dekomp. 5): %lf\n", suma_parallel_5);


    return 0;
}