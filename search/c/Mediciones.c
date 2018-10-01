/*
"FUNCIONES NECESARIAS DE MEDICIÓN DE TIEMPO REAL"

*DESCRIPCIÓN: CONTIENE UNA FUNCIÓN NECESARIA ARA LA MEDICIÓN EN EL
PROGRAMA PRINCIPAL DE LA PRÁCTICA 2 DE ANÁLISIS DE ALGORITMOS.
    ---FUNCIÓN ORIGINAL DESARROLLADA POR: Prof. Edgardo Adrián Franco Martínez.

*FECHA: 30 DE SEPTIEMBRE DE 2018
*VERSIÓN: 1.0
*AUTORES:
  -Fuentes García Alan Kevin
  -López Manríquez Ángel
  -Ruiz López Luis Carlos
*/

#include "Mediciones.h"

/*
DESCRIPCIÓN: obtiene el tiempo transcurrido desde que se lanzó el programa hasta el llamado de la función.

PARÁMETROS: ninguno.

RETORNO: el tiempo real en un tipo de dato double.
*/
double tiempo_real(void) {
    double mega = 1.0e-6;
    struct rusage buffer;
    struct timeval tp;
    struct timezone tzp;

    getrusage(RUSAGE_SELF, &buffer);
    gettimeofday(&tp, &tzp);
    double usertime = (double) buffer.ru_utime.tv_sec + 1.0e-6 * buffer.ru_utime.tv_usec;
    double systime  = (double) buffer.ru_stime.tv_sec + 1.0e-6 * buffer.ru_stime.tv_usec;
    double walltime = (double) tp.tv_sec + 1.0e-6 * tp.tv_usec;

    return walltime;
}
