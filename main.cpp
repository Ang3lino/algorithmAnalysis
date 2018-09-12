//*****************************************************************
//LIBRERIAS INCLUIDAS
//*****************************************************************
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <iostream>

#include "tiempo.h"
#include "Algoritmos.hpp"

using namespace std;

//*****************************************************************
//PROGRAMA PRINCIPAL 
//*****************************************************************
int main (int argc, char **argv) {	
	//******************************************************************	
	//Variables del main
	//******************************************************************	
	double utime0, stime0, wtime0, utime1, stime1, wtime1; //Variables para medición de tiempos
	int n; //n determina el tamaño del algorito dado por argumento al ejecutar
	int i; //Variables para loops
	int algoritmo; //Variable que identifica el algoritmo a usar
	vector<int> v; //Vector de números a ordenar

	void (*algoritmoOrdenamiento[6])(vector<int> &v);
	algoritmoOrdenamiento[0] = bubblesort;
	algoritmoOrdenamiento[1] = bubblesort_optimized;
	algoritmoOrdenamiento[2] = selectionsort;
	algoritmoOrdenamiento[3] = insertionsort;
	algoritmoOrdenamiento[4] = shellsort;
	algoritmoOrdenamiento[5] = treesort;
	//******************************************************************	
	//Recepción y decodificación de argumentos
	//******************************************************************	

	//Se deben introducir 3 argumentos para que el programa se ejecute.
	if (argc != 3) {
		//printf("\nIndique el tamanio del algoritmo - Ejemplo: [user@equipo]$ %s 100\n",argv[0]);
		cout << "Forma de usar el programa:" << endl;
		cout << "./main <algoritmo> <n>" << endl;
		cout << "Donde n es la cantidad de números a ordenar del archivo 10 millones." << endl;
		cout << "Y algoritmo = {bubble(1), bubble optimizado(2), insertion(3), selection(4), shell(5), bts(6)}." << endl;
		exit(1);
	} 
	//Tomar el segundo argumento como nombre del algoritmo y el tercero como el tamaño del problema.
	else
	{
		algoritmo = atoi(argv[1]);
		n = atoi(argv[2]);
    	v = read_from_file("numeros10millones.txt", n);
	}
	
	//******************************************************************	
	//Iniciar el conteo del tiempo para las evaluaciones de rendimiento
	//******************************************************************	
	uswtime(&utime0, &stime0, &wtime0);
	//******************************************************************
	
	//******************************************************************	
	//Algoritmo
	//******************************************************************	

	algoritmoOrdenamiento[algoritmo - 1](v);
	//******************************************************************

	//******************************************************************	
	//Evaluar los tiempos de ejecución 
	//******************************************************************
	uswtime(&utime1, &stime1, &wtime1);
	
	//Cálculo del tiempo de ejecución del programa
	printf("\n");
	printf("real (Tiempo total)  %.10f s\n",  wtime1 - wtime0);
	printf("user (Tiempo de procesamiento en CPU) %.10f s\n",  utime1 - utime0);
	printf("sys (Tiempo en acciónes de E/S)  %.10f s\n",  stime1 - stime0);
	printf("CPU/Wall   %.10f %% \n",100.0 * (utime1 - utime0 + stime1 - stime0) / (wtime1 - wtime0));
	printf("\n");
	
	//Mostrar los tiempos en formato exponecial
	printf("\n");
	printf("real (Tiempo total)  %.10e s\n",  wtime1 - wtime0);
	printf("user (Tiempo de procesamiento en CPU) %.10e s\n",  utime1 - utime0);
	printf("sys (Tiempo en acciónes de E/S)  %.10e s\n",  stime1 - stime0);
	printf("CPU/Wall   %.10f %% \n",100.0 * (utime1 - utime0 + stime1 - stime0) / (wtime1 - wtime0));
	printf("\n");
	//******************************************************************

	//Terminar programa normalmente	
	exit (0);	
}