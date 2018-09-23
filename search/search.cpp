
#include <iostream>
#include <fstream> // permite el manejo de archivos
#include <string>
#include <vector>
#include <array> // biblioteca para c++ 11
#include <iterator> // definicion de ostream_iterator
#include <algorithm> // funcion generica sort
#include <chrono> // medicion de tiempo
#include <typeinfo> // obtencion del tipo de dato de una variable
#include <initializer_list> // argumentos variables de c++
#include <iomanip> // funciones para manipular el flujo de entrada salida

// bibliotecas necesarias para la programacion concurrente
#include <thread>
#include <mutex>
#include <atomic>
#include <memory>
#include <future>

// Funciones mejoradas de C
#include <cassert> // aserciones de C
#include <cstdio>

// bibliotecas personales
#include "bst.h"
#include "files.hpp"
#include "LagrangePolynomial.hpp"
#include "matplotlibcpp.h"

const size_t NTHREADS = 4;

namespace plt = matplotlibcpp;
using namespace std;

typedef pair<int, int> int_pair;
typedef pair<double, double> double_pair;

/**
 * Funcion que sobrecarga el operador de flujo de salida con el fin de que cualquier 
 * vector de tipo t pueda ser impreso usando cout.
 * 
 * Nota: C++ de alguna manera tiene que saber como imprimir al tipo t.
 */
template <typename T>
std::ostream& operator<< (std::ostream& out, const std::vector<T>& v) {
  if ( !v.empty() ) {
    out << '[';
    std::copy (v.begin(), v.end(), std::ostream_iterator<T>(out, ", "));
    out << "\b\b]";
  }
  return out;
}

/**
 * Retornamos intervalos los cuales representan los indices de comienzo y fin de un
 * subvector donde n es la longitud del vector y k las particiones a hacer del mismo, por
 * ejemplo:
 *
 * 50 // 4 = 12
 * (0, 11), (12, 23), (24, 35), (36, 47 + 2)
 *
 * por supuesto, se debe tener n > k.
 */
vector<pair<int, int>> intervals(const int n, const int k) {
    int quotient = n / k, j = 0;
    vector<int_pair> pairs;
    pairs.reserve(k);

    // es mas rapida la funcion miembro emplace_back que push_back, esta manda a llamar 
    // al contructor definido en el operador diamante
    // pairs.push_back(make_pair(j, j + quotient - 1));
    for (int i = 0; i < k - 1; i++, j += quotient) {
        pairs.emplace_back(j, j + quotient - 1);
    }
    pairs.emplace_back(j, n - 1);
    return pairs;
}

/**
 * Determina si x pertenece a un vector retornando un booleano.
 * Esta es la forma mas trivial y lenta de buscar elementos en una coleccion, tiene
 * complejidad O(n).
 */
bool linear_search(const vector<int> &v, const int &x, int_pair p = make_pair(-1, -1)) {
    int a = p.first, b = p.second;
    if (p == make_pair(-1, -1)) // se procedera a buscar sobre todo el vector
        a = 0, b = v.size() - 1;
    for (int i = a; i <= b; i++)
        if (v[i] == x) return true;
    return false;
}
/**
 * Determina si un valor pertenece al vector.
 * 
 * Esta funcion es una mejora a la busqueda lineal pues se hace el uso de hilos.
 */
bool concurrent_linear_search(const vector<int> &v, const int &value) {
  vector<future<bool>> futures; // vector de futuros que retornan bool
  futures.reserve(NTHREADS); // reservamos memoria en el vector como optimizacion
  vector<int_pair> pairs = intervals(v.size(), NTHREADS); // obtenemos los intervalos a buscar
  for (int i = 0; i < NTHREADS; ++i) {
    // inicializamos los hilos, tan pronto se llame async se corren en otro plano
    futures.push_back( async(linear_search, v, value, pairs[i]) );
  }
  for (int i = 0; i < NTHREADS; ++i) 
      if (futures[i].get()) // obtenemos el valor calculado en otro plano, si no nos esperamos
        return true;
  return false; // nunca se encontro si llegamos a este punto
}

/**
 * Determina si x pertenece a un vector previamente ordenado. 
 * 
 * Busca a la mitad de la secuencia, si es menor busca en la subsecuencia de la izquierda,
 * sino busca en la derecha. Si la posicion izquierda es mayor a la derecha no se encontro
 * el valor.
 * 
 * Se reinventa la rueda pues ya existe binary_search en la STL de C++. 
 * 
 * Observaciones:
 * Para calcular la mitad del rango es mejor calcular l+(r-l)/2 a comparacion de (l+r)/2
 * pues l+r puede albergar un entero muy grande.
 * 
 * @params vector<int> v debe estar ordenado y al principio left = 0 y right = n - 1
 */
bool bin_search_helper(const vector<int> &v, const int &x, int left, int right) {
    if (left > right) return false;
    int i = left + (right - left) / 2; 
    //cout << v[i] << endl;
    if (v[i] == x) return true;
    if (x < v[i]) return bin_search_helper(v, x, left, --i);
    return bin_search_helper(v, x, ++i, right);
}

/**
 * Funcion de apoyo que manda a llamar a la misma sobrecargada. La complejidad del
 * algoritmo es O(n lg n).
 * 
 * Para que el algoritmo funcione v tiene que estar ordenado.
 */
bool bin_search(const vector<int> &v, const int findable, const int_pair lims = make_pair(-1, -1)) {
    int a = lims.first, b  = lims.second;
    if (a == -1 && b == -1) // se usa la funcion sin hilos
        a = 0, b = v.size() - 1;
    return bin_search_helper(v, findable, a, b);
}

/*
bool conc_bin_search_helper(const vector<int> &v, const int &value, 
                            const vector<int_pair> &pairs) {
    bool found = false;
    vector<future<bool>> futures;
    futures.reserve(NTHREADS);
    for (int i = 0; i < NTHREADS; i++) 
        futures.push_back( async(bin_search, v, value, pairs[i]) );
    for (int i = 0; i < NTHREADS; i++) 
        found = found or futures[i].get();
    return found;
}

bool concurrent_binary_search(const vector<int> v, const int &value) {
    vector<int_pair> pairs = intervals(v.size(), NTHREADS);
    for (auto p: pairs)  // ordenamos por tramos
        sort(v.begin() + p.first, v.begin() + p.second + 1);
    return conc_bin_search_helper(v, value, pairs);
}
*/


/**
 * - Funcion lambda tipo wrapper para poder medir el tiempo en ejecucion de cualquier funcion.
 * - Puede que el tiempo en ejecucion incrementa ligeramente por el uso de este wrapper.
 * - No puede haber parametros por defecto para la funcion en este wrapper, si se requiere
 *   hay que ponerlos explicitamente.
 * 
 * @params: un functor y sus parametros
 * @return tuple<bool, real>, el primer valor determina si se encontro el valor y el 
 *  segundo el tiempo que llevo en ejecutarse.
 */
auto timeFuncInvocation = [](auto&& func, auto&&... params) {
    bool found; // variable a regresar
    cout << endl;
    chrono::steady_clock::time_point t1 = chrono::steady_clock::now();

    // aplicamos la funcion con los parametros dados
    found = std::forward<decltype(func)>(func)(std::forward<decltype(params)>(params)...); 

    chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
    double duration = double(chrono::duration_cast<chrono::nanoseconds>( t2 - t1 ).count());

    return make_tuple(found, duration / 1e3); // found, time(us)
};



// comandazo
// g++ minimal.cpp -std=c++11 -I C:\Python37\include\ -I C:\Python37\Lib\site-packages\numpy\core\include -L C:\Python37  -lpython37
// las banderas de compilacion de g++ no conmutan

int main(int argc, char const *argv[]) {
    
    // inicializamos los valores
    vector<int> findables = { 
        322486, 14700764, 3128036, 6337399, 61396, 10393545, 2147445644, 1295390003, 
        450057883, 187645041, 1980098116, 152503, 5000, 1493283650, 214826, 1843349527, 
        1360839354, 2109248666 , 2147470852, 0 
    };

    vector<int> nvec = { 
        100, 1000, 5000, 10000, 50000, 100000, 200000, 400000, 600000, 800000, 1000000, 
        2000000, 3000000, 4000000, 5000000, 6000000, 7000000, 8000000, 9000000, 10000000 
    };

    // cargamos v de un archivo de datos ordenados
    vector<int> v = read_from_file("sortedNums.txt", 10e6);
    vector<double> avgs(nvec.size(), 0); // inicializamos size() elementos todos ceros
    bst<int> tree;
    double duration; // variable supuesta a guardar el tiempo en us
    bool found;
    //LagrangePolynomial lagpol;
    
    // obtenemos un fuctor de la funcion amiga contains de la instancia de bst
    auto fun_contains = [&](int value) { return tree.contains(value); };

    tree.sorted_to_bst(v); // construimos un arbol con v ordenado

    const int spaces = 10;
    cout << argv[1] << endl;

    switch(argv[1][0]) {
        case '0':
        cout << "Busqueda lineal" << endl;
        cout << "Numero a buscar" << "|" <<  setw(spaces) << "n" << "|" ;
        cout << setw(spaces) <<  "tiempo " << setw(spaces) << "|" << "encontrado" << endl;
        for (int findable: findables) {
            for (int j = 0; j < nvec.size(); ++j) {
                tie(found, duration) = timeFuncInvocation(linear_search, v, findable, make_pair(0, nvec[j]));
                avgs[j] += duration;
                cout << findable << " | " << setw(spaces) << nvec[j] << " | " << setw(spaces) << duration;
                cout << " | " << setw(spaces) << (found ? "simon" : "nel") << endl;
                //cout << "------------------------------------------------------\n";
            }
        }

        break;
        case '1':
        cout << "Busqueda binaria" << endl;
        cout << "Numero a buscar" << "|" <<  setw(spaces) << "n" << "|" ;
        cout << setw(spaces) <<  "tiempo " << setw(spaces) << "|" << "encontrado" << endl;
        for (int findable: findables) {
            for (int j = 0; j < nvec.size(); ++j) {
                tie(found, duration) = timeFuncInvocation(bin_search, v, findable, make_pair(0, nvec[j]));
                avgs[j] += duration;
                cout << findable << " | " << setw(spaces) << nvec[j] << " | " << setw(spaces) << duration;
                cout << " | " << setw(spaces) << (found ? "simon" : "nel") << endl;
                cout << "------------------------------------------------------\n";
            }
        }

        break;
        case '2':
        cout << "Busqueda por arbol" << endl;
        cout << "Numero a buscar" << "|" <<  setw(spaces) << "n" << "|" ;
        cout << setw(spaces) <<  "tiempo " << setw(spaces) << "|" << "encontrado" << endl;
        for (int findable: findables) {
            for (int n: nvec) {
                tie(found, duration) = timeFuncInvocation(fun_contains, findable);
                cout << findable << " | " << setw(spaces) << n << " | " << setw(spaces) << duration;
                cout << " | " << setw(spaces) << (found ? "simon" : "nel") << endl;
                cout << "------------------------------------------------------\n";
            }
        }

        case '3':
        cout << "Busqueda lineal usando " << NTHREADS << " hilos." << endl;
        cout << "Numero a buscar" << "|" <<  setw(spaces) << "n" << "|" ;
        cout << setw(spaces) <<  "tiempo " << setw(spaces) << "|" << "encontrado" << endl;
        for (int findable: findables) {
            for (int j = 0; j < nvec.size(); ++j) {
                tie(found, duration) = timeFuncInvocation(concurrent_linear_search, v, findable);
                avgs[j] += duration;
                cout << findable << " | " << setw(spaces) << nvec[j] << " | " << setw(spaces) << duration;
                cout << " | " << setw(spaces) << (found ? "simon" : "nel") << endl;
                cout << "------------------------------------------------------\n";
            }
        }

        default: 
            cout << "Opcion invalida";
            return -1;
    }

    cout << nvec << endl;
    cout << avgs << endl;

    // divide cada componente del vector entre su longitud
    transform(avgs.begin(), avgs.end(), avgs.begin(), [&](double &x) { return x / avgs.size(); });
    transform(nvec.begin(), nvec.end(), nvec.begin(), [&](int &x) { return x / 100; });

    cout << nvec << endl;
    cout << avgs << endl;

    plt::plot(nvec, avgs);
    plt::show();

    cout << endl;
    return 0;
}
