#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    double start;
    double end;
    double result;
} Data;

void* thread_function(void* arg) {
    Data* data = (Data*) arg;
    data->result = (data->start + data->end) / 2.0;
    pthread_exit(NULL);
}

int main() {
    int num_threads = 2; // Przetestowanie działania kodu dla co najmniej 2 wątków
    double a = 0.0, b = 10.0; // Przedział (a, b)
    pthread_t threads[num_threads];
    Data data[num_threads];

    // Podzielenie przedziału (a, b) na num_threads fragmentów
    double segment_length = (b - a) / num_threads;
    for (int i = 0; i < num_threads; i++) {
        data[i].start = a + i * segment_length;
        data[i].end = data[i].start + segment_length;
        data[i].result = 0.0;

        // Tworzenie wątku
        if (pthread_create(&threads[i], NULL, thread_function, &data[i]) != 0) {
            perror("Błąd tworzenia wątku");
            exit(EXIT_FAILURE);
        }
    }

    // Oczekiwanie na zakończenie pracy wątków
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    // Sumowanie wyników
    double sum = 0.0;
    for (int i = 0; i < num_threads; i++) {
        sum += data[i].result;
    }

    // Wypisanie wyniku
    printf("Suma wyników: %f\n", sum);

    return 0;
}
