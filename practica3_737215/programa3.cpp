#include <queue>
#include <iostream>
#include <iomanip>
#include <Semaphore.hpp>
#include <cstdlib>
#include <thread>
#include <fstream>
#include <ctime>

using namespace std;

//Declaracion de constantes
const int MAX_SURTIDORES = 3;
const int MAX_REPOSTAJES = 400;
const int MAX_COCHES = 20;
const int MAX_LONG_FICHERO = 100;

// Cola de coches que llegan a la gasolinera
queue <int> filaDeCoches;

bool quiereRevisar=false;
//Semaphore surtidores[4]; 
Semaphore mut(1);
Semaphore turnoRepostaje[MAX_COCHES];
 

// Semaforo de control de revision
Semaphore revision(0);

/* Definicion de las variables */

// Listado de coches que esperan
int listadoCoches[MAX_COCHES];

// Surtidores de gasolina ocupados
int surtidoresEnUso=0;

// Veces que ha repostado cada coche
int numRepostaos=0;

// Tiempo de duracion de repostaje en un surtidor
int tiempoSurtidor[4];

// Booleano que informa de si los surtidores se hallan libres
bool surtidoresVacios[4];

// El surtidor x está en uso
int empleoSurtidor[4];

// Coches en espera
int cochesEnEspera=0;

// Numero de revisiones realizadas
int numRevisiones = 0;

void mostrarResultadosEstad(const char Fichero[]){
	// constante de longitud de linea
	const int MAX_LONG_LINEA = 128;
	// Abrir flujo de entrada
	ifstream f;
	char linea[MAX_LONG_LINEA];
	// Apertura del fichero
	f.open(Fichero);
	if (f.is_open()){
		// Bucle de lectura
		while (!f.eof()){
			f.getline(linea,MAX_LONG_LINEA);
			// Salida estándar
			cout << linea << endl;
		}
		f.close();
		// Cierra el flujo asociado al fichero
	}
}



void avisar(){
	int primerVehiculo;
	// Por prioridad, mirar si hay coches en la cola
	if (filaDeCoches.size() > 0){
		// Me quedo con el primer coche que llego
		primerVehiculo = filaDeCoches.front();
	}
	// Si los surtidores estan vacios y se quiere revisar
	if (surtidoresEnUso<=0 && quiereRevisar){
		quiereRevisar=false;
		revision.signal();
	}
	// Sino se revisa y hay coches con suritodes libres
	else if(surtidoresEnUso<=3 && listadoCoches[primerVehiculo] > 0 && !quiereRevisar ){
		// Retiro el primer coche de la cola
		listadoCoches[primerVehiculo]--;
		filaDeCoches.pop();
		turnoRepostaje[primerVehiculo].signal();
	}
	else{
		// No curre nada y se libera el semaforo
		mut.signal();
	}
}

// proceso estadistico
void estadistico (const char FicheroEstad[]){
	// Nombre del fichero donde se guardan las estadisticas
	// flujo de salida
	ofstream f;
	// Apertura del fichero
	f.open(FicheroEstad);
	if (f.is_open()){
		while (numRepostaos<MAX_REPOSTAJES){
			// Duracion del repostaje
			this_thread::sleep_for(chrono::milliseconds(300));
			mut.wait();
			// Se escribe en el fichero los datos necesarios
			f << " ==== ESTADISTICO INFORMA =====" << endl << endl;
			for (int i = 0; i < 4; i++){
				f << endl;
				cout << endl;
				// Recorrido de los surtidores en busca de modificaciones
				f << " INFORMACION DEL SURTIDOR " << i + 1 << endl;
				f << " El repostaje en este surtidor ha sido de " << tiempoSurtidor[i]/empleoSurtidor[i] << endl;
				f << " Se han respostado en " << empleoSurtidor[i] << " ocasiones." << endl;
			}
			f << " Vehiculos en espera : " << cochesEnEspera << endl << endl;
			f << " ==== FIN DEL COMUNICADO ESTADISTICO ====" << endl;
			f << endl;
			avisar();
		}
	}
	// Se cierra el flujo asociado al fichero
	f.close();
}

// proceso revisor
void revisor(){
	ofstream f1;
	f1.open("log2.log");	
	ofstream f2;
	f2.open("log2_estad.log");
	if (f1.is_open() && f2.is_open()){
		
	
	int chequeo=0;
	while (numRepostaos<MAX_REPOSTAJES){
		chequeo=(1000-100)*rand()/RAND_MAX+100;
		this_thread::sleep_for(chrono::milliseconds(chequeo));
		mut.wait();
		if(surtidoresEnUso>0){
			quiereRevisar=true;
			mut.signal();
			revision.wait();
		}
		// Se revisan los surtidores
		cout << endl << endl;
		cout << "=================== REVISION ===================" << endl<< endl;
		f1 << "=================== REVISION  ===================" << endl<< endl;
		f2 << "=================== REVISION  ===================" << endl<< endl;
		for (int i = 0; i < 4; i++){
			// Muestreo del estado de los surtidores
			if (surtidoresVacios[i]){
				cout << " Surtidor " << i << " vacio " << endl;
				f1 << " Surtidor " << i << " vacio " << endl;
				f2 << " Surtidor " << i << " vacio " << endl;
			}
		}
		// Muestra los datos de la inspeccion
		cout << "La revision ha durado: " << chequeo << endl;
		f1 << "La revision ha durado: " << chequeo << endl;
		cout << " Total de chequeos: " << numRevisiones << endl;
		f1 << " Total de chequeos: " << numRevisiones << endl;
		cout << "La revision ha durado: " << chequeo << endl;
		f2 << "La revision ha durado: " << chequeo << endl;
		cout << " Total de chequeos: " << numRevisiones << endl;
		f2 << " Total de chequeos: " << numRevisiones << endl;
		numRevisiones++;
		for (int i = 0; i < 4; i++){
			f2 << "El surtidor " << i+1 << " se ha utilizado " << empleoSurtidor[i] << " veces." << endl;
			f2 << "El tiempo medio de uso del surtidor " << i+1 << " ha sido de " <<  tiempoSurtidor[i]/empleoSurtidor[i] << endl;
			f2 << "Por el momento han esperado:  " << cochesEnEspera << endl;
			cout << endl << endl;
		}
		avisar();
	}
	f1.close();
	f2.close();
	}
}

// proceso vehiculo [1..20]
void vehiculo (int n, int coche, ofstream &f , ofstream &f1){
	// Duerma al proceso vehiculo[i]
	this_thread::sleep_for(chrono::milliseconds(n));
	int surtidorDisponible;
	for (int i = 0; i < MAX_COCHES; i++){
		mut.wait();
		// Si hay un coche en cada surtidor
		if (surtidoresEnUso>MAX_SURTIDORES){
			// Deben esperar
			listadoCoches[coche]++;
			cochesEnEspera++;
			cout << "El vehiculo " << coche+1 << " quiere repostar." << endl;
			f << "El vehiculo " << coche+1 << " quiere repostar." << endl;
			f1 << "El vehiculo " << coche+1 << " quiere repostar." << endl;
			filaDeCoches.push(coche);
			mut.signal();
			turnoRepostaje[coche].wait();	
		}
		// Se busca que el primero de los surtidores vacios
		bool encontrado = false;
		int j = 0;
		while (!encontrado && j < 4){
			if (surtidoresVacios[j]){
				encontrado = true;
				surtidoresVacios[j] = false;
				surtidorDisponible = j;
			}
		    j++;
		}
		// Uno de los surtidores libres ha sido cocupado
		empleoSurtidor[surtidorDisponible]++;
		tiempoSurtidor[surtidorDisponible]+=n;
		numRepostaos++;
		surtidoresEnUso++;
		
		// Informacion del repostaje del vehiculo en el surtidor por consola
		cout << "Vehiculo " << coche+1 << " repostando en surtidor " << surtidorDisponible+1 << ", ha repostado: " << i + 1 << " veces." << endl;
		// Por fichero
		f << "Vehiculo " << coche+1 << " repostando en surtidor " << surtidorDisponible+1 << ", ha repostado: " << i + 1<< " veces." << endl;
		f1 << "Vehiculo " << coche+1 << " repostando en surtidor " << surtidorDisponible+1 << ", ha repostado: " << i + 1 << " veces." << endl;

		avisar();
		this_thread::sleep_for(chrono::milliseconds(n));
		mut.wait();
		cout << "El vehiculo " << coche+1 << " ha repostado en el surtidor: " << surtidorDisponible+1 << " y esperan : " << cochesEnEspera << endl;
		f << "Sale el coche " << coche+1 << " del surtidor: " << surtidorDisponible+1 << " y esperando: " << listadoCoches[coche] << endl;
		f1 << "Sale el coche " << coche+1 << " del surtidor: " << surtidorDisponible+1 << " y esperando: " << listadoCoches[coche] << endl;
		surtidoresVacios[surtidorDisponible]=true;
		surtidoresEnUso--;
		// Escribo en fichero
		// Se libera el surtidor y se avisa
		avisar();
	}
}

int main(){
	const char nombreFichero[MAX_LONG_FICHERO]="log3.log";
	const char nombreFicheroEst[MAX_LONG_FICHERO]="log3_estad.log";
	ofstream f , f1;
	f.open(nombreFichero);
	f1.open(nombreFicheroEst);
	if (f.is_open() && f1.is_open()){
		for (int i = 0; i < 4; i++){
			surtidoresVacios[i]=true;
			tiempoSurtidor[i]=0;
			empleoSurtidor[i]=0;
		}
		for (int i = 0; i < MAX_COCHES; i++){
			turnoRepostaje[i].setInitValue(0);
			listadoCoches[i]=0;
		}
		cout << " ==== INICIO DE GESTION ==== " << endl;
		f << " ==== INICIO DE GESTION ==== " << endl;
		
		int min = 10 , max = 50;
		
		// Declaracion de procesos
		thread vehiculos[MAX_COCHES];
		thread informador (&estadistico,"log3estad.log");
		thread revisar (revisor);
		srand(time(NULL));
		
		for (int i = 0; i < MAX_COCHES; i++){
			int n=(max-min)*rand()/RAND_MAX+min;
			vehiculos[i] = thread(&vehiculo,n,i,ref(f),ref(f1));
		}
		
		// Finalizacion de los procesos
		for (int i = 0; i < MAX_COCHES; i++){
			vehiculos[i].join();
		}

	
		// Finalizacion del proceso estadistico y revisor
		informador.join();
		revisar.join();
		// Se informa por consola y fichero del final del programa
		cout << endl << endl;
		cout << " ===== FIN DE GESTION ===== " << endl;
		f << " ===== FIN DE GESTION ===== " << endl;
		f.close();
		f1.close();
	}
	
	
	// Muestreo del del fichero estadistico del revisor
	cout << endl << endl;
	cout << " CONTENIDO DEL FICHERO : log3_estad.log " <<  endl << endl;
	
	mostrarResultadosEstad("log3_estad.log");

	
    return 0;
}
