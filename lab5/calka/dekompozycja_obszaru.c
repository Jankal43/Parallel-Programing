#include<stdio.h>
#include<pthread.h>
#include<math.h>

double funkcja ( double x );

double calka_dekompozycja_obszaru(double a, double b, double dx, int l_w);

void* calka_podobszar_w(void* arg_wsk);

typedef struct {
    double a_local;
    double b_local;
    double dx;
    double wynik;
} Obszar;

double calka_dekompozycja_obszaru(double a, double b, double dx, int l_w){

  //printf("a %lf, b %lf, dx %lf\n", a, b, dx);

  double calka_suma_local = 0.0;
	pthread_t watki[l_w];
    Obszar obszary[l_w];
    double szerokosc = (b - a) / l_w;
  // tworzenie struktur danych do obsługi wielowątkowości

 for (int i = 0; i < l_w; i++) {
        obszary[i].a_local = a + i * szerokosc;
        obszary[i].b_local = a + (i + 1) * szerokosc;
        obszary[i].dx = dx;
        pthread_create(&watki[i], NULL, calka_podobszar_w, (void*)&obszary[i]);
    }
    for (int i = 0; i < l_w; i++) {
        pthread_join(watki[i], NULL);
        calka_suma_local += obszary[i].wynik;
    }
  // tworzenie wątków

  // oczekiwanie na zakończenie pracy wątków

  return(calka_suma_local);
}



void* calka_podobszar_w(void* arg_wsk){

  double a_local, b_local, dx;
  // rozpakowanie danych przesłanych do wątku
  Obszar* obszar = (Obszar*)arg_wsk;



  int my_id; // skąd pobierany?
  printf("\nWątek %d: a_local %lf, b_local %lf, dx %lf\n", 
	 my_id, a_local, b_local, dx);
  // wywołanie zadania do wykonania: całkowanie w zadanym przedziale 
  // calka = calka_sekw(a_local, b_local, dx);
  // (trzeba także dodać prototyp na początku pliku)
  
// zadanie w funkcji calka_sekw jest równoważne obliczeniom:
//  int N_local = ceil((b_local-a_local)/dx);
   int N_local = ceil((obszar->b_local - obszar->a_local) / obszar->dx);
//  double dx_adjust_local = (b_local-a_local)/N_local;
    double dx_adjust_local = (obszar->b_local - obszar->a_local) / N_local;
//  printf("a_local %lf, b_local %lf, dx_adjust_local %lf, n_local %d\n", 
//	 a_local, b_local, dx_adjust_local, N_local);
//  int i;
//  double calka = 0.0;
    for (int i = 0; i < N_local; i++) {
       double x1 = obszar->a_local + i * dx_adjust_local;
       obszar->wynik += 0.5 * dx_adjust_local * (funkcja(x1) + funkcja(x1 + dx_adjust_local));
       }
       pthread_exit(NULL);
//  for(i=0; i<N_local; i++){
//    double x1 = a_local + i*dx_adjust_local;
//    calka += 0.5*dx_adjust_local*(funkcja(x1)+funkcja(x1+dx_adjust_local));
    //printf("i %d, x1 %lf, funkcja(x1) %lf, całka = %.15lf\n", 
    //	   i, x1, funkcja(x1), calka);
//  }

}
