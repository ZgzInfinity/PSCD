//*****************************************************************
// File:   practica_1_V2.cpp
// Author: PSCD-Unizar
// Date:   septiembre 2016
// Coms:   Parte del práctica 1 de PSCD
//         Compilar mediante
//           g++ -std=c++11 -pthread practica_1_V2.cpp -o practica_1_V2
//*****************************************************************

#include <iostream>
#include <thread>
#include <string>
#include <chrono>
#include <cstdlib>

using namespace std;

const int DIM = 10;
const int semilla = 20;
const int COTA_INF_VECES = 5;
const int COTA_MAX_VECES = 15;
const int COTA_INF_RETARDO = 100;
const int COTA_MAX_RETARDO = 300;

int randomRetardo (int retardo){
	srand(semilla);
	retardo = COTA_INF_RETARDO + rand() % (COTA_MAX_RETARDO + 1 - COTA_INF_RETARDO);
	return retardo;
}

int randomVeces (int veces){
	srand(semilla);
	veces = COTA_INF_VECES + rand() % (COTA_MAX_VECES + 1 - COTA_INF_VECES);
	return veces;
}



void saludo(string m, int retardo, int veces) {
	 for ( int i = 1; i <= veces; i++){
        cout << m+"\n";
        this_thread::sleep_for(chrono::milliseconds(retardo));
	 }	
    
}


int main() {
    
    thread P[DIM];
	string mensaje;
	int retardo = 0, veces = 0;
	string tab;
	string escribir;
	string soy= "Soy ";
	

    for (int j = 0; j < DIM; j++){
		  mensaje = to_string(j+1);
		  int desfase = randomRetardo(retardo);
          int cantidad = randomVeces(veces);
		  escribir=tab+ soy + mensaje;
		  tab=tab +"\t";
		  P[j] = thread(&saludo, escribir ,desfase,cantidad);
		  
	}
    
	for ( int i = 0; i < DIM; i++ ){
		P[i].join();
	}
 
 
    cout << "Fin\n";
    return 0;
}