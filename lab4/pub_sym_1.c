#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define ILE_MUSZE_WYPIC 3

void *watek_klient(void *arg);
int liczba_kufli;               // Liczba dostępnych kufli
pthread_mutex_t mutex;          // Mutex do synchronizacji

int main(void) {
    pthread_t *tab_klient;
    int *tab_klient_id;

    int l_kl = 30;   // Liczba klientów większa od liczby kufli
    int l_kf = 10;   // Liczba kufli

    liczba_kufli = l_kf;
    pthread_mutex_init(&mutex, NULL);  // Inicjalizacja mutexu

    tab_klient = (pthread_t *)malloc(l_kl * sizeof(pthread_t));
    tab_klient_id = (int *)malloc(l_kl * sizeof(int));
    for (int i = 0; i < l_kl; i++) tab_klient_id[i] = i;

    printf("\nOtwieramy pub (faulty)!\n");
    printf("\nLiczba wolnych kufli na start: %d\n", l_kf);

    for (int i = 0; i < l_kl; i++) {
        pthread_create(&tab_klient[i], NULL, watek_klient, &tab_klient_id[i]);
    }
    for (int i = 0; i < l_kl; i++) {
        pthread_join(tab_klient[i], NULL);
    }

    if (liczba_kufli != l_kf) {
        printf("\nBłędna liczba kufli ( %i, gdzie poprawnie: %i )\n", liczba_kufli, l_kf);
    } else {
        printf("\nPoprawna liczba kufli: %i\n", liczba_kufli);
    }

    printf("\nZamykamy pub!\n");

    free(tab_klient);
    free(tab_klient_id);
    pthread_mutex_destroy(&mutex);  // Zniszczenie mutexu
    return 0;
}

void *watek_klient(void *arg) {
    int moj_id = *((int *)arg);
    int ile_musze_wypic = ILE_MUSZE_WYPIC;

    for (int i = 0; i < ile_musze_wypic; i++) {
        pthread_mutex_lock(&mutex);  // Blokada sekcji krytycznej

        if (liczba_kufli > 0) {
            liczba_kufli--;  // Pobranie kufla
            printf("Klient %d: pobrano kufel, kufli pozostało: %d\n", moj_id, liczba_kufli);
        } else {
            printf("Klient %d: pobrano kufel NIEPRAWIDŁOWO, kufli pozostało: %d\n", moj_id, liczba_kufli);
        }

        pthread_mutex_unlock(&mutex);  // Odblokowanie sekcji krytycznej

        // Symulacja picia piwa
        usleep(30);

        // Oddanie kufla
        pthread_mutex_lock(&mutex);
        liczba_kufli++;
        printf("Klient %d: oddano kufel, kufli dostępnych: %d\n", moj_id, liczba_kufli);
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}
