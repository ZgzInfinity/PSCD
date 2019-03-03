
// Bibliotecas del programa

#include "Monitor2.hpp"
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <thread>
#include <fstream>
#include <ctime>
#include <queue>

// Declaración de constantes

const int MAX_LONG_FICHERO = 100;
const int MAX_LONG_LINEA = 128;
const int MAX_COCHES = 20;
const int MAX_REPOSTAJES = 400;
const int COTA_MINIMA = 1;
const int COTA_MAXIMA = 100;


using namespace std;

/* Variables globales */


// Total de veces que se han repostado entre todos los coches
int vecesRepostados=0;

// Variable del monitor
controlGasolinera g;


/*
 * Pre: ---
 * Post: Devuelve por salida estándar
 * 		 el contenido del fichero donde se han almacenado las estadísitcas
 */
void mostrarResultadosEstadisticos (const char nombre[]){
	// Apertura del flujo de datos
	ifstream f;
	f.open(nombre);
	// Se asocia el flujo de datos al fichero << nombre >>
	if (f.is_open()){
		/* Si puede abrirse el fichero
		 * lleva a cabo una lectura línea por línea
		 * mostrando su contenido por salida estandar */
		char linea[MAX_LONG_LINEA];
		f.getline(linea,MAX_LONG_LINEA,'\n');
		while (!f.eof()){
			cout << linea << endl;
			f.getline(linea,MAX_LONG_LINEA,'\n');
		}
		// Cierre del flujo de datos asociado al fichero
		f.close();
	}
}

// gestión del proceso revisor 
void revisor(int aleatorio, ofstream& f){
	while (vecesRepostados<MAX_REPOSTAJES){
		this_thread::sleep_for(chrono::milliseconds(aleatorio)); // Tiempo que está el coche circulando
		g.iniciarRevision(aleatorio,f);
		g.finalizarRevision(f);
	}
}


// proceso estadistico
void estadistico(int retardo, ofstream& f2){
	while (vecesRepostados<MAX_REPOSTAJES){
	// El proceso se duerme tantos milisegundos como indica << retardo >>
	this_thread::sleep_for(chrono::milliseconds(retardo));
	// Escritura de las estadísticas en el fichero asociado
	g.iniciarEstadistico(f2);
	}
}

// proceso vehiculo(i) => [1..20]
void vehiculo (int vehiculo, int retardo, ofstream& f1){
	// Indicación del surtidor libre
	int elSurtidorLibre;
	for (int i = 0; i < MAX_COCHES; i++){
		// El vehiculo circula
		this_thread::sleep_for(chrono::milliseconds(retardo)); 
		// El vehiculo entra a repostar si puede hacerlo
		g.iniciarRepostaje(vehiculo,elSurtidorLibre,i,retardo,f1);
		// Tiempo en repostaje
		this_thread::sleep_for(chrono::milliseconds(retardo)); 
		// El vehiculo i sale del surtidor dejandolo libre
		g.finalizarRepostaje(vehiculo,elSurtidorLibre,f1);
		// Incremento del número de repostajes totales
		vecesRepostados++;
	}
}

int main(){
	// Ficheros donde guardar los datos de la gasolinera
	const char fichero[MAX_LONG_FICHERO] = "log2.log";
	const char fichEstadistico[MAX_LONG_FICHERO] = "log2_estad.log";
	
	// Creación de los flujos de salida asociados
	ofstream f1;
	ofstream f2;
	
	// Se abren los ficheros
	f1.open(fichero);
	
	f2.open(fichEstadistico);
	
	// Si se han podido abrir 
    if(f2.is_open() && f1.is_open()) {
		// Inicio del programa
		cout << endl;
		cout << " Inicio de la gestion " << endl;
	
		// Vector de procesos para identificar los vehiculos
		thread vehiculos[MAX_COCHES];
	
		// Variable para medir el tiempo aleatorio
		int tiempo;
	
	
		// Se lanzan los  20 procesos vehiculo
		srand(time(NULL));
		for (int i = 0; i < MAX_COCHES; i++){
			tiempo = (COTA_MAXIMA - COTA_MINIMA) * rand() / RAND_MAX + COTA_MINIMA;
			vehiculos[i] = thread(&vehiculo, i, tiempo,ref(f1));
		}
	
		// Se lanza el proceso estadistico
		thread resumirEstadisticas(&estadistico,tiempo,ref(f2));
	
		tiempo = (2000 - 1000) * rand() / RAND_MAX + 1000;
	
		// Se lanza el proceso revisor
		thread tocaRevisar(&revisor,tiempo,ref(f1));
	
	
		// Finalizan los procesos vehiculo
		for (int i = 0; i < MAX_COCHES; i++){
	    vehiculos[i].join();
		}
	
		// Terminan el proceso estadistico y el revisar
		resumirEstadisticas.join();
		tocaRevisar.join();
	
	
		// Se cierran los flujos
		f1.close();
		f2.close();

	
		cout << "Fin de la jornada con un total de " << vecesRepostados << " coches repostados." << endl;
	}
	
	cout << endl << endl;
	
	cout << "  MUESTREO DE LAS ESTADISTICAS " << endl;
	cout << " ============================== " << endl << endl;
	
	/* Muestreo por salida estandar del contenido almacenado en fichEstadistico */
	mostrarResultadosEstadisticos(fichEstadistico);

	// Fin del programa
    return 0;
}


