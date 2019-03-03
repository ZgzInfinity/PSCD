#include <iostream>
#include <thread>
#include <cmath>
#include <cstdlib>
#include <iostream>

using namespace std;

const int N = 256;
const int numProcesos = 16;
const int COTA_INF = 1;
const int COTA_SUP = 10;
typedef double IM[N][N];
int trozo = N/sqrt(numProcesos);

int contador = 0;
double sumaTotal = 0;
atomic_flag lock_stream = ATOMIC_FLAG_INIT;

void generarMatrix(IM m){
	srand (time(NULL));
	for (int i = 0; i < N; i++){
		for (int j = 0; j < N; j++){
			m[i][j] =(rand() % COTA_SUP) / COTA_INF;
		}
	}
}

void mostrarMatriz (IM m){
	for (int i = 0; i < N; i++){
		for (int j = 0; j < N; j++){
			cout << m[i][j] << " ";
		}
		cout << endl;
	}
}


void submatrix (IM m, int i1, int j1, int i2, int j2){
	double total=0.0;
	for (int i= i1 * trozo; i < i2 * trozo; i++){
		for (int j = j1 * trozo; j < j2 * trozo; j++){
			total+= m[i][j];
		}
	}
	while (lock_stream.test_and_set()){
		
	}
	contador++;
	sumaTotal += total;
	lock_stream.clear();
	if (contador == numProcesos){
		cout << " La media de la matriz es " << sumaTotal / (N*N) << endl;
		cout << " Fin del programa " << endl;
	}
}




int main(){
	IM matrix;
	generarMatrix(matrix);
	
	// Invocar función mostrarMatriz
	
	int k = 0;
	thread P[numProcesos];
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
		P[k]=thread(&submatrix,matrix,i,j,i + 1,j + 1);
		k++;
		}
	}
 
	for (int i=0; i<=numProcesos - 1; i++){
	P[i].join();
	}
	
	return 0;
}


