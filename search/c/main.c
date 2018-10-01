/*
"PROGRAMA PRINCIPAL DE LA PRÁCTICA 2: ANÁLISIS TEMPORAL
Y NOTACIÓN DE ORDEN (ALGORITMOS DE BÚSQUEDA)"

*DESCRIPCIÓN: EJECUTA ALGUNO DE LOS 3 ALGORITMOS DE BUSQUEDA SIGUIENTES:
	-LINEAL
	-BINARIA
	-POR ABB
Y SUS VARINTES CON HILOS PARA ENCONTRAR UN NÚMERO EN UN ESPACIO DE 
BUSQUEDA DETERMINADO.

*FECHA: 30 DE SEPTIEMBRE DE 2018
*VERSIÓN: 1.0
*AUTORES:
	-Fuentes García Alan Kevin
	-López Manríquez Ángel
	-Ruiz López Luis Carlos
*/

#include "Algoritmos.h"
#include "Mediciones.h"
#include "ArbolBinarioBusqueda.h"

int main (int argc, char const **argv) {

	/*Corecicón de uso.*/
	if (argc != 3) {
		printf("Uso: %s [algoritmo] [numero]\n", argv[0]);
		printf("Donde:\n");
		printf("-algoritmo: es el número del algoritmo a ejecutar.\n");
		printf("\t1. Busqueda Lineal.\n");
		printf("\t2. Busqueda Binaria.\n");
		printf("\t3. Busqueda Por ABB.\n");
		printf("\t4. Busqueda Lineal (con hilos).\n");
		printf("\t5. Busqueda Binaria (con hilos).\n");
		printf("\t6. Busqueda Por ABB (con hilos).\n");
		printf("-numero: es el elemento a ser encontrado.\n");
		return -1;
	}

	/*Argumentos del main pasados a enteros.*/
	int algoritmo = atoi(argv[1]);
	int numero = atoi(argv[2]);

	/*Arreglo que contiene los tmaños de problema a probar.*/
	int n[20] = {
		100, 1e3, 5e3, 10e3, 50e3, 100e3, 200e3, 400e3, 600e3, 800e3,
		1e6, 2e6, 3e6, 4e6, 5e6, 6e6, 7e6, 8e6, 9e6, 10e6
	};

	/*Arreglos donde se almacenarán los tiempos y resultados para 
	cada tamaño de problema.*/
	double tiempos[20];
	int resultados[20];

	/*Definición de la señal que detiene los hilos.*/
	signal(SIGUSR1, handler);

	/*Espacios de búsqueda totales y para los hilos.*/
	const int num_hilos = 4;
	nodo *arbol = NULL;
	int *arreglo = NULL;
	nodo **nodos_hilos = NULL;
	int *indices_hilos = NULL;

	/*Variables necesarias para cada iteración (n).*/
	int i;
	double t_inicio;
	double t_fin;
	/*Variables necesarias para el uso de hilos.*/
	int j;
	pthread_t *hilos = (pthread_t *) malloc(sizeof(pthread_t) * num_hilos);
	Argumentos *args = (Argumentos *) malloc(sizeof(Argumentos) * num_hilos);
	switch (algoritmo) {
		case 1:
			arreglo = obtenerArreglo(10e6, 1, NULL);

			for (i = 0; i < 20; i++) {
				t_inicio = tiempo_real();
				resultados[i] = busquedaLineal(arreglo, n[i], numero);
				t_fin = tiempo_real();
				tiempos[i] = t_fin - t_inicio;
			}

			break;
		case 2:
			arreglo = obtenerArreglo(10e6, 1, NULL);
			
			for (i = 0; i < 20; i++) {
				t_inicio = tiempo_real();
				resultados[i] = busquedaBinaria(arreglo, n[i], numero);
				t_fin = tiempo_real();
				tiempos[i] = t_fin - t_inicio;
			}

			break;
		case 3:
			for (i = 0; i < 20; i++) {
				arbol = obtenerArbol(n[i], 1, NULL);
				t_inicio = tiempo_real();
				resultados[i] = busquedaPorABB(arbol, numero);
				t_fin = tiempo_real();
				tiempos[i] = t_fin - t_inicio;
			}

			break;
		case 4:
			arreglo = obtenerArreglo(10e6, 1, NULL);

			/*Inicialización de los argumentos del i-ésimo hilo.*/
			for (j = 0; j < num_hilos; j++) {
				args[j].num = numero;
				args[j].arbol = NULL;
				args[j].hilos = hilos;
				args[j].num_hilos = num_hilos;
				args[j].resultado = -1;
			}

			/*Ejecuciones de los hilos apra cada tamaño de problema.*/
			for (i = 0; i < 20; i++) {
				/*Preparación de los argumentos para cada hilo*/
				free(indices_hilos);
				indices_hilos = obtenerDivisiones(n[i], num_hilos);

				/*Inicialización de los argumentos faltantes.*/
				for (j = 0; j < num_hilos; j++) {
					args[j].arreglo = arreglo + indices_hilos[j];
					args[j].n = indices_hilos[j + 1] - indices_hilos[j];
				}
				/*Inicio de ejecución de los hilos y medición del tiempo.*/
				t_inicio = tiempo_real();
				for (j = 0; j < num_hilos; j++) {
					pthread_create(&hilos[j], NULL, busquedaLinealHilo, &args[j]);
				}

				/*Esperamos a que los hilos terminen de ejecutarse.*/
				for (j = 0; j < num_hilos; j++) {
					pthread_join(hilos[j], NULL);
				}

				/*Medición del tiempo final y registro de tiempo y resultado*/
				t_fin = tiempo_real();
				resultados[i] = resultadoHilos(args);
				tiempos[i] = t_fin - t_inicio;
			}
			break;
		case 5:
			arreglo = obtenerArreglo(10e6, 1, &indices_hilos);

			/*Inicialización de los argumentos del i-ésimo hilo.*/
			for (j = 0; j < num_hilos; j++) {
				args[j].num = numero;
				args[j].arbol = NULL;
				args[j].hilos = hilos;
				args[j].num_hilos = num_hilos;
				args[j].resultado = -1;
			}

			/*Ejecuciones de los hilos apra cada tamaño de problema.*/
			for (i = 0; i < 20; i++) {
				/*Preparación de los argumentos para cada hilo.*/
				indices_hilos = obtenerDivisiones(n[i], num_hilos);

				/*Inicialización de los argumentos faltantes.*/
				for (j = 0; j < num_hilos; j++) {
					args[j].arreglo = arreglo + indices_hilos[j];
					args[j].n = indices_hilos[j + 1] - indices_hilos[j];
				}

				/*Inicio de ejecución de los hilos y medición del tiempo*/
				t_inicio = tiempo_real();
				for (j = 0; j < num_hilos; j++) {
					pthread_create(hilos + j, NULL, busquedaBinariaHilo, args + j);
				}

				/*Esperamos a que los hilos terminen de ejecutarse.*/
				for (j = 0; j < num_hilos; j++) {
					pthread_join(hilos[j], NULL);
				}

				/*Medición del tiempo final y registro de tiempo y resultado*/
				t_fin = tiempo_real();
				resultados[i] = resultadoHilos(args);
				tiempos[i] = t_fin - t_inicio;
			}
			break;
		case 6:
			/*Inicialización de los argumentos del i-ésimo hilo.*/
			for (j = 0; j < num_hilos; j++) {
				args[j].num = numero;
				args[j].arreglo = NULL;
				args[j].hilos = hilos;
				args[j].num_hilos = num_hilos;
				args[j].resultado = -1;
			}

			/*Ejecuciones de los hilos apra cada tamaño de problema.*/
			for (i = 0; i < 20; i++) {
				/*Preparación de los argumentos para cada hilo.*/
				arbol = obtenerArbol(n[i], num_hilos, &nodos_hilos);

				/*Inicialización de los argumentos faltantes.*/
				for (j = 0; j < num_hilos; j++) {
					args[j].arbol = nodos_hilos[j];
				}

				/*Inicio de ejecución de los hilos y medición del tiempo*/
				t_inicio = tiempo_real();
				for (j = 0; j < num_hilos; j++) {
					pthread_create(hilos + j, NULL, busquedaPorABBHilo, args + j);
				}

				/*Esperamos a que los hilos terminen de ejecutarse.*/
				for (j = 0; j < num_hilos; j++) {
					pthread_join(hilos[j], NULL);
				}

				/*Medición del tiempo final y registro de tiempo y resultado*/
				t_fin = tiempo_real();
				resultados[i] = resultadoHilos(args);
				tiempos[i] = t_fin - t_inicio;
			}
			break;
		default:
			printf("Algoritmo no soportado.\n");
	}

	/*IMPRIMIR TIEMPOS Y RESULTADOS.*/
	printf("|\t\tN\t\t|\t\tTiempo\t\t|\t\tResultado\t\t|\n");
	for (i = 0; i < 20; i++) {
		printf("|\t%d\t|\t%.10e\t|\t\t%d\t\t|\n", n[i], tiempos[i], resultados[i]);
	}
	return 0;
}