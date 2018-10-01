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

#include "Algoritmos.h"

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
int * obtenerDivisiones (int n, int num_hilos) {
	int num_divisiones = num_hilos + 1;
	int * divisiones = (int *) malloc(sizeof(int) * num_divisiones);
	int tam_division = n / num_hilos;
	*divisiones = 0;
	
	int i;
	for (i = 1; i < num_divisiones; i++) {
		divisiones[i] = divisiones[i - 1] + tam_division;
		if (i == num_hilos) {
			divisiones[i] += n % num_hilos;
		}
	}

	return divisiones;
}

/*
DESCRIPCIÓN: Obtiene "n" números en secuencia de un archivo que contiene 10 millones
de números. El orden depende del valor que tenga el parámetro "orden".

PARÁMETROS: 
	- n (in): tamaño del arreglo a ser generado. 
	- num_hilos (in): número de hilos con los que se buscará en el arreglo.
	- indices_hilos (out): un arreglo que contiene los rangos de los sub-arreglos

RETORNO: Un apuntador al arreglo de enteros resultante.
*/
int * obtenerArreglo (int n, int num_hilos, int ** indices_hilos) {
	/*Se abre el archivo para lectura y se verifica que no haya errores.*/
	FILE *archivo = fopen("./10millones.txt", "rt");

	if (archivo == NULL) {
		printf("Ocurrió un error al abrir el archivo.\n");
		return NULL;
	}

	if (n > 10e6) {
		printf("La cantidad de números requerida no debe pasar 10 millones.\n");
		return NULL;
	}

	/*Proceso de obtención del arreglo.*/
	int *arreglo = (int *) malloc(sizeof(int) * n);
	int i = 0;
	char actual[15];
	for (i = 0; i < n; i++) {
		fgets(actual, 15, archivo);
		arreglo[i] = atoi(actual);
	}

	fclose(archivo);

	/*Valores de salida*/
	if (indices_hilos != NULL)
		*indices_hilos = obtenerDivisiones(n, num_hilos);
	return arreglo;
}

/*
DESCRIPCIÓN: Obtiene "n" números en secuencia de un archivo que contiene 10 millones
de números. El orden depende del valor que tenga el parámetro "orden".

PARÁMETROS: 
	- n (in): tamaño del arreglo a ser generado. 
	- num_hilos (in): número de hilos con los que se buscará en el arreglo.
	- indices_hilos (out): un arreglo que contiene los rangos de los sub-arreglos

RETORNO: Un apuntador al arreglo de enteros resultante.
*/
nodo * obtenerArbol (int n, int num_hilos, nodo *** nodos_hilos) {
	/*Se abre el archivo para lectura y se verifica que no haya errores.*/
	FILE *archivo = fopen("./numeros10millones.txt", "rt");

	if (archivo == NULL) {
		printf("Ocurrió un error al abrir el archivo.\n");
		return NULL;
	}

	if (n > 10e6) {
		printf("La cantidad de números requerida no debe pasar 10 millones.\n");
		return NULL;
	}

	printf("Obteniendo arbol con %d elementos\n", n);
	/*Proceso de obtención del árbol.*/
	nodo *arbol = NULL;
	int i = 0;
	int j = 0;
	char actual[15];
	int *divisiones = obtenerDivisiones(n, num_hilos);
	nodo **nodos = (nodo **) malloc(sizeof(nodo * ) * num_hilos);
	nodo *ultimoInsertado = NULL;
	for (i = 0; i < n; i++) {
		fgets(actual, 15, archivo);
		ultimoInsertado = insertar(&arbol, atoi(actual));
		if (i == divisiones[j]) {
			*(nodos + j) = ultimoInsertado;
			j++;
		}
	}

	fclose(archivo);

	/*Valores de salida*/
	if (nodos_hilos != NULL)
		*nodos_hilos = nodos;
	return arbol;
}

/*=========================================================================
ALGORITMOS DE BÚSQUEDA.
===========================================================================*/

/*
DESCRIPCIÓN: Realiza la búsqueda binaria de un número (num) 
en un arreglo (arr) ordenado de n enteros.

RECIBE: 
	-"arreglo" de enteros ordenados donde se realizará la busqueda binaria;
	-"n" que es el tamaño del arreglo y
	-"num" que es el entero a ser encontrado en el arreglo.

VALOR DE RETORNO: un entero que indica el índice del elemento
encontrado o -1 si el elemento no se encontró.
*/
int busquedaBinaria (int *arreglo, int n, int num) {
	int indiceBajo = 0; /*Cota inferior del rango posible en el que está el número.*/
	int indiceAlto = n; /*Cota superior del rango posible en el que está el número.*/
	int indiceActual; /*Índice que se está revisando en el arreglo.*/
	int resultado = -1; /*Resultado del algoritmo.*/

	/*Cada ciclo se ejecuta si el número aún no ha sido encontrado en el arreglo
	o si los índices (cotas) cambian de orden (el número no existe en el arreglo)*/
	do {
		indiceActual = indiceBajo + (indiceAlto - indiceBajo) / 2;
		if (num > arreglo[indiceActual]) {
			indiceBajo = indiceActual + 1;
		} else if (num < arreglo[indiceActual]) {
			indiceAlto = indiceActual - 1;
		} else {
			resultado = indiceActual;
			break;
		}
	} while (indiceBajo <= indiceAlto);

	/*Si no, se regresa el valor original: -1 (no encontrado)*/
	return resultado;
}

/*
DESCRIPCIÓN: Realiza la búsqueda lineal de un número (num) 
en un arreglo (arr) ordenado de n enteros.

RECIBE: 
	-"arreglo" de enteros ordenados donde se realizará la busqueda lineal;
	-"n" que es el tamaño del arreglo y
	-"num" que es el entero a ser encontrado en el arreglo.

VALOR DE RETORNO: un entero que indica el índice del elemento
encontrado o -1 si el elemento no se encontró.
*/

int busquedaLineal (int *arreglo, int n, int num) {
	int resultado = -1;
	int i = 0;
	/*Se itera através de todo el arreglo y se compara con el número que se busca.*/
	while (arreglo[i] != num && i < n) {
		i++;
	}

	/*Se verifica si el bucle terminó con el indice dentro del rango de tamaño 
	del arreglo.*/
	if (i != n) {
		resultado = i;
	}

	/*Si no es el caso, se regresa el valor original: -1 (no encontrado)*/
	return resultado;
}

/*
DESCRIPCIÓN: Realiza la búsqueda de un número (num) 
en un ABB (arbol).

RECIBE: 
	-"arbol" que es un ABB y
	-"num" que es el entero a ser encontrado en el arreglo.

VALOR DE RETORNO: un entero cuyo valor es 1 si se encontró el número
o -1 si no.
*/
int busquedaPorABB (nodo *ABB, int num) {
	int resultado = -1;

	/*Se realiza la búsqueda por árbol binario y se verifica que el
	valor esté en el árbol.*/
	if (buscarNodo(ABB, num) != NULL) {
		resultado = 1;
	}

	/*Si el valor no está, se retorna con valor -1*/
	return resultado;
}

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
void handler (int signum) {
	pthread_exit(NULL);
}

/*
DESCRIPCIÓN: Termina los hilos del arreglo "hilos" de tamaño "n"

RECIBE: 
	-hilos: un apuntador a un arreglo de hilos.
	-num_hilos: la cantidad de hilos a ser terminados.

VALOR DE RETORNO: ninguno.
*/
void terminarHilos(pthread_t *hilos, int num_hilos) {
	int i;
	for (i = 0; i < num_hilos; i++) {
		pthread_kill(hilos[i], SIGUSR1);
	}
}

/*
DESCRIPCIÓN: Es un wrapper para la función busquedaLineal() preparada
para ser ejecutada por un hilo. Si se finaliza la búsqueda en algún hilo,
se terminan los demás.

RECIBE: 
	-args: un apuntador que contiene la dirección de una estructura
	Argumentos.

VALOR DE RETORNO: NULL.
*/
void * busquedaLinealHilo (void * args) {
	/*Indicamos al hilo que puede ser detenido con una variable estática
	que es una señal.*/
	static sigset_t mask;
	sigemptyset(&mask);
	sigaddset(&mask, SIGUSR1);
	/*Ya sabemos que args es una estructura de tipo Argumentos, entonces invocamos a
	busquedaLineal normalmente*/
	int resultado;
	Argumentos *argsLineal = (Argumentos *) args;
	resultado = busquedaLineal(argsLineal->arreglo, argsLineal->n, argsLineal->num);
	((Argumentos *) args)->resultado = resultado;
	if (resultado != -1) {
		terminarHilos(argsLineal->hilos, argsLineal->num_hilos);
	}

	pthread_exit(NULL);
}

/*
DESCRIPCIÓN: Es un wrapper para la función busquedaBinaria() preparada
para ser ejecutada por un hilo. Si se finaliza la búsqueda en algún hilo,
se terminan los demás.

RECIBE: 
	-args: un apuntador que contiene la dirección de una estructura
	Argumentos.

VALOR DE RETORNO: NULL.
*/
void * busquedaBinariaHilo (void * args) {
	/*Indicamos al hilo que puede ser detenido con una variable estática
	que es una señal.*/
	static sigset_t mask;
	sigemptyset(&mask);
	sigaddset(&mask, SIGUSR1);

	/*Ya sabemos que args es una estructura de tipo Argumentos, entonces invocamos a
	busquedaBinaria normalmente*/
	int resultado;
	Argumentos *argsBinaria = (Argumentos *) args;
	resultado = busquedaBinaria(argsBinaria->arreglo, argsBinaria->n, argsBinaria->num);
	((Argumentos *) args)->resultado = resultado;
	if (resultado != -1) {
		terminarHilos(argsBinaria->hilos, argsBinaria->num_hilos);
	}

	pthread_exit(NULL);
}

/*
DESCRIPCIÓN: Es un wrapper para la función busquedaPorABB() preparada
para ser ejecutada por un hilo. Si se finaliza la búsqueda en algún hilo,
se terminan los demás.

RECIBE: 
	-args: un apuntador que contiene la dirección de una estructura
	Argumentos.

VALOR DE RETORNO: NULL.
*/
void * busquedaPorABBHilo (void * args) {
	/*Indicamos al hilo que puede ser detenido con una variable estática
	que es una señal.*/
	static sigset_t mask;
	sigemptyset(&mask);
	sigaddset(&mask, SIGUSR1);

	/*Ya sabemos que args es una estructura de tipo Argumentos, entonces invocamos a
	busquedaPorABB normalmente*/
	int resultado;
	Argumentos *argsPorABB = (Argumentos *) args;
	resultado = busquedaPorABB(argsPorABB->arbol, argsPorABB->num);
	((Argumentos *) args)->resultado = resultado;
	if (resultado != -1) {
		terminarHilos(argsPorABB->hilos, argsPorABB->num_hilos);
	}

	pthread_exit(NULL);
}

/*
DESCRIPCIÓN: Revisa en los argumentos de salida de los hilos, un resultado distinto de -1

RECIBE: 
	-args: un apuntador que contiene la dirección de un arreglo de Argumentos.

VALOR DE RETORNO: -1 si todos los valores de salida son -1 o el valor de salida del que no lo sea.
*/
int resultadoHilos (Argumentos *args) {
	int resultado = -1;
	int i = 0;
	int lim = args->num_hilos;
	for (i = 0; i < lim; i++) {
		if (args[i].resultado != -1) {
			resultado = args[i].resultado;
			if (args[i].arbol == NULL) {
				resultado += args[i].n * i;
			}
			break;
		}
	}
	return resultado;
}
