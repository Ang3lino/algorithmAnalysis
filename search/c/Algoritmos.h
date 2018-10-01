/*
"BIBLIOTECA DE ALGORITMOS DE BÚSQUEDA NECESARIOS PARA LA PRÁCTICA 2"

*DESCRIPCIÓN: CONTIENE LOS ALGORITMOS NECESARIOS PARA EJECUTAR EL
PROGRAMA PRINCIPAL DE LA PRÁCTICA 2 DE ANÁLISIS DE ALGORITMOS.

*FECHA: 30 DE SEPTIEMBRE DE 2018
*VERSIÓN: 1.0
*AUTORES:
	-Fuentes García Alan Kevin
	-López Manríquez Ángel
	-Ruiz López Luis Carlos
*/

#ifndef _ALGORITMOS_H_
#define _ALGORITMOS_H_

/*=========================================================================
BIBLIOTECAS NECESARIAS
===========================================================================*/
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include "ArbolBinarioBusqueda.h"

/*=========================================================================
DEFINICIÓN DE LOS PARÁMETROS DE LAS FUNCIONES CON HILOS
===========================================================================*/

struct Argumentos {
	/*ENTRADA*/
	int *arreglo;
	int n;
	int num;
	nodo *arbol;
	pthread_t *hilos;
	int num_hilos;
	/*SALIDA*/
	int resultado;
};

typedef struct Argumentos Argumentos;
/*=========================================================================
FUNCIONES DE OBTENCIÓN DE ELEMENTOS
===========================================================================*/

/*
DESCRIPCIÓN: Según el tamaño de un arreglo (n) y el número de sub-arreglos
(num_hilos) que se quieran tratar en él, retorna un arreglo con los índices
de los rangos de dichos sub-arreglos.

PARÁMETROS: 
	- n (in): tamaño del arreglo a ser segmantado. 
	- num_hilos (in): número de hilos con los que se buscará en el arreglo.

RETORNO: Un apuntador al arreglo de enteros resultante que contienen los rangos
de cada sub-arreglo.
*/
int * obtenerDivisiones (int n,  int num_hilos);

/*
DESCRIPCIÓN: Obtiene "n" números en secuencia de un archivo que contiene 10 millones
de números. El orden depende del valor que tenga el parámetro "orden".

PARÁMETROS: 
	- n (in): tamaño del arreglo a ser generado. 
	- num_hilos (in): número de hilos con los que se buscará en el arreglo.
	- indices_hilos (out): un arreglo que contiene los rangos de los sub-arreglos

RETORNO: Un apuntador al arreglo de enteros resultante.
*/
int * obtenerArreglo (int n, int num_hilos, int ** indices_hilos);

/*
DESCRIPCIÓN: Obtiene "n" números en secuencia de un archivo que contiene 10 millones
de números. El orden depende del valor que tenga el parámetro "orden".

PARÁMETROS: 
	- n (in): tamaño del arreglo a ser generado. 
	- num_hilos (in): número de hilos con los que se buscará en el arreglo.
	- indices_hilos (out): un arreglo que contiene los rangos de los sub-arreglos

RETORNO: Un apuntador al arreglo de enteros resultante.
*/
nodo * obtenerArbol (int n, int num_hilos, nodo *** nodos_hilos);

/*=========================================================================
ALGORITMOS DE BÚSQUEDA.
===========================================================================*/

/*================================================================
DESCRIPCIÓN: Realiza la búsqueda binaria de un número (num) 
en un arreglo (arr) ordenado de n enteros.

RECIBE: 
	-"arreglo" de enteros ordenados donde se realizará la busqueda binaria;
	-"n" que es el tamaño del arreglo y
	-"num" que es el entero a ser encontrado en el arreglo.

VALOR DE RETORNO: un entero que indica el índice del elemento
encontrado o -1 si el elemento no se encontró.
=================================================================*/
int busquedaBinaria (int *arreglo, int n, int num);

/*================================================================
DESCRIPCIÓN: Realiza la búsqueda lineal de un número (num) 
en un arreglo (arr) ordenado de n enteros.

RECIBE: 
	-"arreglo" de enteros ordenados donde se realizará la busqueda lineal;
	-"n" que es el tamaño del arreglo y
	-"num" que es el entero a ser encontrado en el arreglo.

VALOR DE RETORNO: un entero que indica el índice del elemento
encontrado o -1 si el elemento no se encontró.
=================================================================*/
int busquedaLineal (int *arreglo, int n, int num);

/*================================================================
DESCRIPCIÓN: Realiza la búsqueda de un número (num) 
en un ABB (arbol).

RECIBE: 
	-"arbol" que es un ABB y
	-"num" que es el entero a ser encontrado en el arreglo.

VALOR DE RETORNO: un entero cuyo valor es 1 si se encontró el número
o -1 si no.
=================================================================*/
int busquedaPorABB (nodo *ABB, int num);

/*==================================================================
FUNCIONES CON HILOS
====================================================================*/

/*
DESCRIPCIÓN: Handler para efectuar correctamente la función signal() en el main
Con esta función, al llamar a pthread_kill(), se detendrá el hilo que se mande como
primer argumento, retornando un valor NULL.

RECIBE: 
	-signum: sin uso, simplemente para cumplir con el formato de la función handler.

VALOR DE RETORNO: ninguno.
*/
void handler (int signum);

/*
DESCRIPCIÓN: Termina los hilos del arreglo "hilos" de tamaño "n"

RECIBE: 
	-hilos: un apuntador a un arreglo de hilos.
	-num_hilos: la cantidad de hilos a ser terminados.

VALOR DE RETORNO: ninguno.
*/
void terminarHilos(pthread_t *hilos, int num_hilos);

/*
DESCRIPCIÓN: Es un wrapper para la función busquedaLineal() preparada
para ser ejecutada por un hilo. Si se finaliza la búsqueda en algún hilo,
se terminan los demás.

RECIBE: 
	-args: un apuntador que contiene la dirección de una estructura
	Argumentos.

VALOR DE RETORNO: NULL.
*/
void * busquedaLinealHilo (void * args);

/*
DESCRIPCIÓN: Es un wrapper para la función busquedaBinaria() preparada
para ser ejecutada por un hilo. Si se finaliza la búsqueda en algún hilo,
se terminan los demás.

RECIBE: 
	-args: un apuntador que contiene la dirección de una estructura
	Argumentos.

VALOR DE RETORNO: NULL.
*/
void * busquedaBinariaHilo (void * args);

/*
DESCRIPCIÓN: Es un wrapper para la función busquedaPorABB() preparada
para ser ejecutada por un hilo. Si se finaliza la búsqueda en algún hilo,
se terminan los demás.

RECIBE: 
	-args: un apuntador que contiene la dirección de una estructura
	Argumentos.

VALOR DE RETORNO: NULL.
*/
void * busquedaPorABBHilo (void * args);

/*
DESCRIPCIÓN: Revisa en los argumentos de salida de los hilos, un resultado distinto de -1

RECIBE: 
	-args: un apuntador que contiene la dirección de un arreglo de Argumentos.

VALOR DE RETORNO: -1 si todos los valores de salida son -1 o el valor de salida del que no lo sea.
*/
int resultadoHilos (Argumentos *args);

#endif