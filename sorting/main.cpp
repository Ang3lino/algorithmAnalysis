//*****************************************************************
//M. EN C. EDGARDO ADRIÁN FRANCO MARTÍNEZ 
//Curso: Análisis de algoritmos
//(C) Enero 2013
//ESCOM-IPN
//Ejemplo de medición de tiempo en C y recepción de parametros en C bajo UNIX
//Compilación: "gcc main.c tiempo.x  -o main(teimpo.c si se tiene la implementación de la libreria o tiempo.o si solo se tiene el codigo objeto)"
//Ejecución: "./main n c" (Linux y MAC OS)
//*****************************************************************

//*****************************************************************
//LIBRERIAS INCLUIDAS
//*****************************************************************
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <string>
#include <vector>
#include <stack>

#include "bst.h"
#include "tiempo.h"

#include <fstream>
#include <string>
#include <cstdlib>

#include <cassert>

using namespace std;

// dado un nombre al archivo retornamos un vector cargado, leemos hasta n lineas
// notas: si el archivo no existe se hace una asercion, n debe ser menor que 10e6
vector<int> read_from_file(const char * path, const int n) {
    vector<int> loaded;
    ifstream stream(path); // flujo de entrada
    int temp, i = 0;
    assert(stream.is_open());
    while (i < n && stream >> temp) {
        loaded.push_back(temp);
        i++;
    }
    stream.close();
    return loaded;
}

void create_subfile(const char *path, const char *name, size_t lines) {
    int i = 0;
    ifstream istream(path);
    ofstream ostream(name);
    string tmp;
    assert(istream.is_open());
    while (i++ < lines && istream >> tmp) ostream << tmp << endl;
    ostream.close();
    istream.close();
}

void swap(int &a, int &b) {
    int tmp = a;
    a = b;
    b = tmp;
}

void bubblesort (vector<int> &v) {
	size_t len = v.size();
	for (int i = 0; i < len - 1; i++) {
		for (int j = 0; j < len - 1 - i; j++) {
			if (v[j] > v[j + 1]) {
				swap(v[j], v[j + 1]);
			}
		}
	}
}

void bubblesort_optimized(vector<int> &v) {
    size_t len = v.size();
    bool change_presented;
    for (int i = 0; i < len; i++) {
        change_presented = false;
        for (int j = 0; j < len - i - 1; j++) {
            if (v[j] > v[j + 1]) {
                swap(v[j], v[j + 1]);
                change_presented = true;
            }
        }
        if (!change_presented) return;
    }
}

void selectionsort(vector<int> &v) {
    size_t n = v.size();
    for (int i = 0; i < n; i++) {
        int minpos = i;
        for (int j = i + 1; j < n; j++) {
            if (v[j] < v[minpos]) {
                minpos = j;
            }
        }
        swap(v[i], v[minpos]);
    }
}

void insertionsort(vector<int> &v) {
    size_t n = v.size();
    for (int i = 1; i < n; i++) {
        int j = i;
        int val = v[i];
        while (j > 0 && v[j - 1] > val) {
            v[j] = v[j - 1];
            j--;
        }
        v[j] = val;
    }
}

void shellsort(vector<int> &v) {
    size_t n = v.size();
    int gap = 1;
    while (gap < n / 3) gap = gap * 3 + 1;
    while (gap > 0) {
        for (int i = gap; i < n; i++) {
            int tmp = v[i];
            int j = i;
            while (j > gap - 1 && v[j - gap] >= tmp) {
                v[j] = v[j - gap];
                j -= gap;
            }
            v[j] = tmp;
        }
        gap = (gap - 1) / 3;
    }
}

void treesort(vector<int> &v) {
    bst<int> tree(v);
    tree.sort(v);

}

void delete_node_from_bst_test() {
    //vector<int> v = { 3, 8, 0, 7, 4, 5 };
    vector<int> v = read_from_file("10millones.txt", 1e4);
    bst<int> tree(v);

    cout << "antes: " << endl;
    //tree.inorder();
    cout << "\nNumero de nodos: " << tree.numberOfNodes() << endl;

    for (int x: v) {
        cout << "\nElemento actual: " << x << endl;
        tree.remove(x);
        //tree.inorder();
        cout << endl;
    }
    cout << "despues: " << endl;
    tree.inorder();
    cout << "Numero de nodos: " << tree.numberOfNodes() << endl;
}

void sorttest() {
    vector<int> v = read_from_file("10millones.txt", 1e1);
    cout << "leidos" << endl;
    treesort(v);
    cout << "ordenados" << endl;
    for (int i = 0; i < v.size(); i++) cout << i << " " << v[i] << "\n";
}


//*****************************************************************
//VARIABLES GLOBALES
//*****************************************************************

//*****************************************************************
//PROGRAMA PRINCIPAL 
//*****************************************************************
int main (int argc, char *argv[])
{	
	//******************************************************************	
	//Variables del main
	//******************************************************************	
	double utime0, stime0, wtime0, utime1, stime1, wtime1; //Variables para medición de tiempos
	int n; 	//n determina el tamaño del algorito dado por argumento al ejecutar
	int i; //Variables para loops
	char algoritmo; //Variable que identifica el algoritmo a usar
	vector<int> v; //Vector de números a ordenar


	//******************************************************************	
	//Recepción y decodificación de argumentos
	//******************************************************************	

	//Si no se introducen exactamente 2 argumentos (Cadena de ejecución y cadena=n)
	if (argc != 3) 
	{
		//printf("\nIndique el tamanio del algoritmo - Ejemplo: [user@equipo]$ %s 100\n",argv[0]);
		cout << "Forma de usar el programa:" << endl;
		cout << "./main <n> <nombre_algoritmo>" << endl;
		cout << "Donde n es la cantidad de números a ordenar del archivo 10 millones." << endl;
		cout << "Y nombre_algoritmo = {bubble(b), bubble optimizado(B), insertion(i), selection(s), shell(S), bts(t)}." << endl;
		exit(1);
	} 
	//Tomar el segundo argumento como tamaño del algoritmo
	else
	{
		n = atoi(argv[1]);
		algoritmo = argv[2][0];
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

    switch (algoritmo) {
    	case 'b':
    	{
    		bubblesort(v);
    		break;
    	}
    	case 'B':
    	{
    		bubblesort_optimized(v);
    		break;
    	}
    	case 'i':
    	{
    		insertionsort(v);
    		break;
    	}
    	case 's':
    	{
    		selectionsort(v);
    		break;
    	}
    	case 'S':
    	{
    		shellsort(v);
    		break;
    	}
    	case 't':
    	{
    		bst<int> tree(v);
    		tree.sort(v);
    		break;
    	}
    	default:
    	{
    		cout << "Algoritmo no soportado" << endl;
    	}
    }
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
	
	for (i = 0; i < v.size(); i++) 
		cout << i + 1  << ": " <<  v[i] << endl;

	//Terminar programa normalmente	
	exit (0);	
}


