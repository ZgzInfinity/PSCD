
// Bibliotecas del programa

#include "Monitor.hpp"
#include <iomanip>
#include <iostream>
#include <thread>
#include <fstream>
#include <queue>

using namespace std;

// Declaracion de constantes


/*
 * Implementación del monitor gasolinera 
 */
 
// Función constructor
controlGasolinera::controlGasolinera() {
	// Constructor del monitor
	vehiculosEnEspera=0;
	hanEsperado=0;
	surtidoresEnUso=0;
	
	/* Inicializa las propiedades de cada surtidor:
	 *	- Esta vacio
	 *	- Se ha usado 0 veces
	 *  - Se ha durado su repostaje en él 0 segundos 
	 */
	for (int i = 0; i < MAX_SURTIDORES; i++){
		empleoSurtidor[i]=0;
		tiempoSurtidor[i]=0;
		surtidoresLibres[i]=true;
	}
};

// Gestión de los vehiculos para entrar a repostar
void controlGasolinera::iniciarRepostaje(int vehiculo, int& elSurtidorLibre, int vecesRepostado, int duracion, ofstream& f){
	bool puedoEntrar=false;
	
	int primerVehiculo;
	unique_lock<mutex> lck (mtx);
	
	/* Cuando un vehiculo quiere repostar entra a la cola */
	filaCoches.push(vehiculo);
	vehiculosEnEspera++;
	
	/* -- Control de la operación bloqueante --
	 * Solo entran cuando el coche es el primero de la cola y 
	 * hay al menos un surtidor libre */
	while (surtidoresEnUso>=MAX_SURTIDORES || filaCoches.front()!=vehiculo){
		hanEsperado++;
		cout << "!El coche " << vehiculo+1 << " quiere repostar y hay " << vehiculosEnEspera << " coches esperando." << endl;
		f << "!El coche " << vehiculo+1 << " quiere repostar y hay " << vehiculosEnEspera << " coches esperando." << endl;
		// Bloqueo del vehiculo
		repostaje.wait(lck);
		
	}
	// El vehiculo puede repostar
	vehiculosEnEspera--;
	primerVehiculo=filaCoches.front();
	filaCoches.pop();

	// Busca cual es el primer surtidor libre
	bool encontrado = false;
	int j = 0;
	while (!encontrado && j < MAX_SURTIDORES){
		if (surtidoresLibres[j]){
			encontrado = true;
			surtidoresLibres[j] = false;
			elSurtidorLibre = j;
			}
		j++;
	}
	
	// Se modifica el estado del surtidor donde reposta el coche
	tiempoSurtidor[elSurtidorLibre] = tiempoSurtidor[elSurtidorLibre] + duracion;
	empleoSurtidor[elSurtidorLibre]++;
	surtidoresEnUso++;
	cout << "Va a repostar el vehiculo " << primerVehiculo+1 << " en surtidor libre " << elSurtidorLibre+1<< " y ha repostado " << vecesRepostado+1 << " veces." << endl;
	
}



// gestión de los vehiculos al salir del surtidor
void controlGasolinera::finalizarRepostaje(int vehiculo, int elSurtidorLibre, ofstream& f){
	unique_lock<mutex> lck (mtx);
	
	// El surtidor queda libre
	surtidoresLibres[elSurtidorLibre]=true;
	surtidoresEnUso--;
	cout << "Vehiculo " << vehiculo+1 << " ha repostado en surtidor " << elSurtidorLibre+1 << endl;
	f << "Vehiculo " << vehiculo+1 << " ha repostado en surtidor " << elSurtidorLibre+1 << endl;
	
	// Se avisan a todos los coches que han quedado bloqueados 
	repostaje.notify_all();
}

// gestión del proceso estadistico
void controlGasolinera::iniciarEstadistico(ofstream& f){
	unique_lock<mutex> lck (mtx);
	// Almacena en el fichero como evoluciona la situación de la gasolinera
	f << endl << endl;
	f << "**** ESTADISTICO INFORMANDO ****" << endl;
	f << "********************************" << endl;
	for (int i = 0; i < MAX_SURTIDORES; i++){
	    f << "El surtidor " << i + 1 << " se ha usado " << empleoSurtidor[i] << " veces." << endl;
	    f << "La duracion media del repostaje en el surtidor " << i + 1 << " ha sido de "<< tiempoSurtidor[i] / empleoSurtidor[i] << endl;
	}
	f << "Coches bloqueados hasta el momento: " << hanEsperado << endl;
	f << "*************************************" << endl;
	f << "******** FIN DEL COMUNICADO *********" << endl;
	f << endl << endl;
}
