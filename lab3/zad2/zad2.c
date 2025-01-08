#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *thread_function(void *arg) {
    int thread_id = *(int *)arg;
    printf("Wątek o ID: %lu, przekazany identyfikator: %d\n", pthread_self(), thread_id);
    return NULL;
}

int main() {
    int liczba_wątków = 5;
    pthread_t wątki[liczba_wątków];
    int identyfikatory[liczba_wątków];

    for (int i = 0; i < liczba_wątków; i++) {
        identyfikatory[i] = i;
        pthread_create(&wątki[i], NULL, thread_function, &identyfikatory[i]);
    }
    for (int i = 0; i < liczba_wątków; i++) {
        pthread_join(wątki[i], NULL);
    }

    return 0;
}
