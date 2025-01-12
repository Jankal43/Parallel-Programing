 #include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

#define WYMIAR 18

int main ()
{
    double a[WYMIAR];

#pragma omp  parallel for default(none) shared(a)
    for(int i=0;i<WYMIAR;i++) a[i]=1.02*i;

    // pętla sekwencyjna
    double suma=0.0;
    for(int i=0;i<WYMIAR;i++) {
        suma += a[i];
    }

    printf("Suma wyrazów tablicy: %lf\n", suma);

    // pętla do modyfikacji - docelowo równoległa w OpenMP
    double suma_parallel=0.0;
    omp_set_num_threads(4);
//#pragma omp parallel for default(none) shared(a) reduction(+:suma_parallel) ordered
    // #pragma omp parallel for default(none) shared(a) reduction(+:suma_parallel) ordered num_threads(4) schedule(static, 3)
   // #pragma omp parallel for default(none) shared(a) reduction(+:suma_parallel) ordered num_threads(4) schedule(static)
     //#pragma omp parallel for default(none) shared(a) reduction(+:suma_parallel) ordered num_threads(4) schedule(dynamic, 2)
#pragma omp parallel for default(none) shared(a) reduction(+:suma_parallel) ordered num_threads(4) schedule(dynamic)

    for(int i=0;i<WYMIAR;i++) {
        int id_w = omp_get_thread_num();
        // ...
        suma_parallel += a[i];
        // ...
#pragma omp ordered
        printf("a[%2d]->W_%1d  \n",i,id_w);
    }

    printf("\nSuma wyrazów tablicy równolegle (z klauzulą -  schedule(dynamic): %lf\n",
       suma_parallel);

}