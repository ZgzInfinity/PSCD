#include <iostream>
#include <thread>
#include <cmath>
#include <cstdlib>

using namespace std;

const int N = 256;
const int numProcesos = 16;
const int COTA_INF = 1;
const int COTA_SUP = 10;
typedef double IM[N][N];
int trozo = N/sqrt(numProcesos);
int contador = 0;


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


void submatrix (IM m, int i1, int j1, int i2, int j2, double& suma){
	double total=0.0;
	for (int i= i1 * trozo; i < i2 * trozo; i++){
		for (int j = j1 * trozo; j < j2 * trozo; j++){
			total = total + m[i][j];
		}
	}
	suma = total;
	contador++;
}


void informador (double sumParcial[]){
	double suma=0.0;
	for (int i=0; i< numProcesos; i++){
		suma = suma + sumParcial[i];
		}
	cout <<"La media es... : "<<suma/(N*N) << endl;
}

int main(){
	IM matrix;
	generarMatrix(matrix);
	
	// Invocar función mostrarMatriz
	
	double sumasParciales[numProcesos]; //vector donde almaceno las sumas parciales 
	int k = 0;
	thread P[numProcesos+1];
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
		P[k]=thread(&submatrix,matrix,i,j,(i + 1),(j + 1),ref(sumasParciales[k]));
		k++;}
	}
 
	P[numProcesos] = thread(&informador,ref(sumasParciales));

	for (int i=0; i<=numProcesos; i++){
	P[i].join();
	}
	
	cout << " Fin del programa " << endl;
	return 0;
}


