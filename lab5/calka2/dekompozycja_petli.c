#include<stdio.h>
#include<pthread.h>
#include<math.h>

double funkcja ( double x );

double calka_zrownoleglenie_petli(double a, double b, double dx, int l_w);

static int l_w_global=0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static double calka_global=0.0;
static double a_global;
static double b_global;
static double dx_global;
static int N_global;

void* calka_fragment_petli_w(void* arg_wsk);

double calka_zrownoleglenie_petli(double a, double b, double dx, int l_w){
	printf("wchodze do funkcji");
  int N = ceil((b-a)/dx);
  double dx_adjust = (b-a)/N;

   a_global = a;
    b_global = b;
    dx_global = dx_adjust;
    N_global = N;
    l_w_global = l_w;

  printf("Obliczona liczba trapezów: N = %d, dx_adjust = %lf\n", N, dx_adjust);
  //printf("a %lf, b %lf, n %d, dx %.12lf (dx_adjust %.12lf)\n", a, b, N, dx, dx_adjust);


  // tworzenie struktur danych do obsługi wielowątkowości
 	pthread_t watki[l_w];
    int watki_id[l_w];

  // tworzenie wątków
  	for (int i = 0; i < l_w; i++) {
        watki_id[i] = i;
        pthread_create(&watki[i], NULL, calka_fragment_petli_w, &watki_id[i]);
    }

  // oczekiwanie na zakończenie pracy wątków
   for (int i = 0; i < l_w; i++) {
        pthread_join(watki[i], NULL);
    }
	printf("CALKA GLOBALNA -------> %lf", calka_global );
  return(calka_global);
}

void* calka_fragment_petli_w(void* arg_wsk){

   int my_id = *(int*)arg_wsk;

  double b; // skąd pobrać dane a, b, dx, N, l_w ?
  int N, l_w;      // wariant 1 - globalne

  // a = a_global; // itd. itp. - wartości globalne nadaje calka_zrownoleglenie_petli
	  double a = a_global;
    double dx = dx_global;
  // dekompozycja cykliczna
 int my_start = my_id;
 int my_stride = l_w_global;
  int my_end = 0;


  // dekompozycja blokowa
  //int my_start = 0;
  //int my_end = 0;
  //int my_stride = 0;

  // something else ? (dekompozycja blokowo-cykliczna)

  printf("\nWątek %d: my_start %d, my_end %d, my_stride %d\n",
	 my_id, my_start, my_end, my_stride);


  int i;
  double calka_local = 0.0;
  for(i=my_start; i<my_end; i+=my_stride){

    double x1 = a + i*dx;
    calka_local += 0.5*dx*(funkcja(x1)+funkcja(x1+dx));
    //printf("i %d, x1 %lf, funkcja(x1) %lf, całka = %.15lf\n",
    //	   i, x1, funkcja(x1), calka);

  }
     pthread_mutex_lock(&mutex);
    printf("----------INKREMENTACJA CALKA GLOBALNA -------> %lf", calka_global );
     calka_global += calka_local;
    pthread_mutex_unlock(&mutex);

    return NULL;

}

