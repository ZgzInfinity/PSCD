
#include <iostream>
#include <cstdlib>
#include <string>
#include <chrono>
#include "saludador.h"
#include <ctime>

const int DIM = 10;
const int COTA_INF_VECES = 5;
const int COTA_MAX_VECES = 15;
const int COTA_INF_RETARDO = 100;
const int COTA_MAX_RETARDO = 300;


/* Funciones auxiliares */
int randomRetardo (int& retardo){
	srand(time(NULL));
	retardo = COTA_INF_RETARDO + rand() % (COTA_MAX_RETARDO + 1 - COTA_INF_RETARDO);
	return retardo;
}

int randomVeces (int& veces){
	srand(time(NULL));
	veces = COTA_INF_VECES + rand() % (COTA_MAX_VECES + 1 - COTA_INF_VECES);
	return veces;
}


int main() {
    
    thread P[DIM];
	string mensaje, tab, escribir;
	string soy= "Soy ";
	

    for (int i = 0; i < DIM; i++){
		  mensaje = to_string(i+1);
		  int retardo = randomRetardo(retardo);
          escribir = tab + soy + mensaje;
		  tab = tab +"\t";
          int veces = randomVeces(veces);
          Saludador S = {escribir,retardo,veces};
		  P[i] = thread(&Saludador::run,S);
		  
	}
    
	for ( int i = 0; i < DIM; i++ ){
		P[i].join();
	}
 
 
    cout << "Fin\n";
    return 0;
}