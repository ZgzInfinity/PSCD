//*****************************************************************
// File:   practica_1_V3.cpp
// Author: PSCD-Unizar
// Date:   septiembre 2016
// Coms:   Parte del práctica 1 de PSCD
//         Compilar mediante
//           g++ -std=c++11 -pthread practica_1_V3.cpp -o practica_1_V3
//*****************************************************************

#include <iostream>
#include "saludador.h"
#include <thread>
#include <string>
#include <chrono>

using namespace std;


// Implementación de las funciones de los objetos de la clase


Saludador::Saludador(string mens, int retardo, int veces) {
    this->mens = mens; //mens: parámetro de la función
                       //this->mens: variable propia
    this->retardo = retardo;
    this->veces = veces;
};


Saludador::Saludador() {
    mens = "";          //mens: parámetro de la función
                       //this->mens: variable propia
    retardo = 0;
    veces = 0;
};

void Saludador::run() {
    for(int i=1; i<=veces; i++) {
        cout << mens +"\n";
        this_thread::sleep_for(chrono::milliseconds(retardo));
    }
};