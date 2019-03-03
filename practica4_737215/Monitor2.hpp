//*****************************************************************
// File:   Monitor.hpp
// Author: PSCD-Unizar
// Date:   octubre 2016
// Coms:   Especificación de una clase de semáforos, extendida, con operaciones
//         "wait(nat)" y "signal(nat)"
//         El valor transmitido en la inicialización ha de ser
//         no negativo. Si fuera negativo, da error y aborta la ejecución.
//         La semántica que implementa es la que en
//         M. Ben-Ari, "Principles of Concurrent and Distributed Programming", Addison-Wesley, 2006
//         denomina "Busy-wait" (ver pág. 120)
//         Para los casos en que el uso del semáforo va a ser exclusivamente como "mutex", Cpp
//         suministra la clase "std::mutex", cuya especificación se encuentra en 
//         http://en.cppreference.com/w/cpp/thread/mutex
//*****************************************************************

#ifndef Monitor2_HPP
#define Monitor2_HPP

// Bibliotecas del programa

#include <mutex>
#include <condition_variable>
#include <assert.h>
#include <queue>

const int MAX_SURTIDORES = 4;

using namespace std; 

// monitor gasolinera
class controlGasolinera {
private:

    mutex mtx;         								// Semaforo mutex
													// Variables de condicion
    condition_variable repostaje;		
	condition_variable revision;
	
	/* Declaracion de variables permanentes */
	
	bool deseoDeRevisar;							// Indicación para saber si se quiere revisar
	bool enRevision;								// Booleano que informa de si nos encontramos en revision
	int hanEsperado;								// Vehiculos que esperan en total
	int surtidoresEnUso;	  						// Surtidores ocupados
	int tiempoSurtidor[MAX_SURTIDORES];				// Duracion del repostaje
	int empleoSurtidor[MAX_SURTIDORES];				// surtidor en uso
	bool surtidoresLibres[MAX_SURTIDORES];          // vector de booleanos para controlar el uso de los surtidores
	int vehiculosEnEspera;							// Vehiculos a la espera de entrar a un surtidor 
	int totalRevisiones;							// Almacena cuantas revisiones se han realizado 

	queue<int> filaCoches;							// lista FIFO del monitor asociada a la variable condicion

public:
	// Operaciones del monitor
	controlGasolinera();
	void iniciarRepostaje(int coche, int& elSurtidor, int vecesRepostado, int elTiempo, ofstream& f);
	void finalizarRepostaje(int coche, int elSurtidor, ofstream& f);
	void iniciarEstadistico(ofstream& f);
	void iniciarRevision(int aleatorio, ofstream& f);
	void finalizarRevision(ofstream& f);
};

#endif 
