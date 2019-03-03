//*****************************************************************
// File:   practica_1_V3.cpp
// Author: PSCD-Unizar
// Date:   septiembre 2016
// Coms:   Parte del práctica 1 de PSCD
//         Compilar mediante
//           g++ -std=c++11 -pthread practica_1_V3.cpp -o practica_1_V3
//*****************************************************************

#include <iostream>
#include <thread>
#include <string>


using namespace std;

//-----------------------------------------------------
// Parte de especificación: variables y funciones
// de los objetos de esta clase.
// Normalmente iría en el fichero "Saludador.h"
// Dejamos todo público, para centrarnos en los conceptos
// de la concurrencia. Lo adecuado es distinguir entre
// parte pública y privada, con operadores para acceder
// a los atributos, por ejemplo. Lo estudiaréis en otras
// asignaturas

class Saludador {
public:
    Saludador(string mens,  //constructor suministrando datos
              int retardo,
              int veces);
    Saludador(); //constructor por defecto
    void run();
    //más funciones, si las hubiera
    string mens;
    int retardo, veces;
};
