/*
"FUNCIONES NECESARIAS DEL ÁRBOL BINARIO DE BÚSQUEDA"

*DESCRIPCIÓN: CONTIENE LOS ALGORITMOS NECESARIOS DE UN ABB PARA EJECUTAR EL
PROGRAMA PRINCIPAL DE LA PRÁCTICA 2 DE ANÁLISIS DE ALGORITMOS.

*FECHA: 30 DE SEPTIEMBRE DE 2018
*VERSIÓN: 1.0
*AUTORES:
  -Fuentes García Alan Kevin
  -López Manríquez Ángel
  -Ruiz López Luis Carlos
*/

#ifndef _ABB_H_
#define _ABB_H_


#include <stdio.h>
#include <stdlib.h>
/*DEFINICION DE LA ESTRUCTURA DEL NODO.*/
struct nodo
{
  int dato;
  struct nodo *izq, *der;
};
typedef struct nodo nodo;
/*FUNCIONES BÁSICAS DEL ARBOL BINARIO DE BÚSQUEDA.*/

/*
DESCRIPCIÓN: crea un nodo de un ABB con el valor entero 
que se le proporcione como argumento.

PARÁMETROS: 
	- dato: un entero que será guardado en un nodo nuevo.

RETORNO: Un apuntador a un nodo de un arbol binario.
*/
nodo * crearNodo (int dato);

/*
DESCRIPCIÓN: busca en el ABB "arbol" un entero "dato" y regresa el nodo que lo contiene.

PARÁMETROS: 
	-arbol: un apuntador al nodo raiz de un ABB.
	-dato: un entero que se desea buscar.

RETORNO: Un apuntador al nodo que contiene el dato buscado o NULL si no e encontró.
*/
nodo * buscarNodo (nodo * arbol, int dato);

/*
DESCRIPCIÓN: inserta en el ABB "arbol" un entero "dato".

PARÁMETROS: 
	-arbol: un apuntador al nodo raiz de un ABB.
	-dato: un entero que se desea insertar.

RETORNO: ninguno.
*/
nodo * insertar (nodo ** arbol, int dato);

#endif