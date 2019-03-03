#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include <ctime>
#include <cmath>
#include <thread>
#include <chrono>

using namespace std;

// Constantes globales
const int n = 100;
const int retardo = 10;

// Variables globales
double mediaAr, maximo , minimo, dvtp;

/*
 * Pre: n = 100
 * Post: Muestra por pantalla la media aritmetica 
 *       de los elementos en v[0,n-1]
 */
void media (double v[], int n){
     // En suma no se ha acumulado ningun elemento del vector |v|
     double suma = 0.0;
     for (int i = 0; i < n; i ++){
         // Acumula el elemento i-ésimo del vector |v|
         suma += v[i];
     }
     // Muestra por pantalla el valor promedio de los elementos acumulados 
     mediaAr = suma / n;
}

/*
 * Pre: n = 100
 * Post: Muestra por pantalla el valor maximo y
 *       minimo de los elementos en v[0,n-1]
 */
void maximoMinimo (double v[], const int n){
     // Toma como valor maximo inicial el primer elemento del vector |v|
      double max = v[0];
      for (int i = 1; i < n; i++){
          if (v[i] > max){
              max = v[i];
          }
      }
      // max guarda el maximo de los elementos de v[0,n-1]
      // Toma como valor minimo inicial el primer elemento del vector |v|
	  double min = v[0];
      for (int i = 1; i < n; i++){
          if (v[i] < min){
              min = v[i];
          }
      }
     maximo = max;
	 minimo = min;
} 


/*
 * Pre: n = 100
 * Post: Muestra por pantalla la desviacion tipica 
 *       de los elementos en v[0,n-1]
 */
void desviacionTipica (double v[], int n){
     double suma = 0.0;
     for (int i = 0; i < n; i++){
         suma = suma + pow(v[i]- mediaAr,2);
     }
	 dvtp = sqrt(suma/(n-1));
}

/*
 * Pre: n = 100
 * Post: Rellena con elementos aleatorios el vector |v|
 *       
 */
void generarVector (double v[], const int n){
      srand(time(NULL));
      for (int i = 0; i < n; i++){
          v[i] = (rand() % 10000) / 100.0;
      }
}


int main(){
    double v[n];
	
	// Generacion del vector de numeros aleatorios
    generarVector(v,n);
    
	
    // Lanzamiento de los procesos
	
    thread th_1(&media, v , n );
	thread th_2(&maximoMinimo, v , n );
	thread th_3(&desviacionTipica, v , n);
	
    th_1.join();
    th_2.join();
    th_3.join();
    
	
	// Muestreo por pantalla de los resultados obtenidos
	cout << " #datos: " << n << endl;
	cout << " media : " << mediaAr << endl;
	cout << " max : " << maximo << endl;
	cout << " min : " << minimo << endl;
	cout << " sigma : " << dvtp << endl;
	
    return 0;
}