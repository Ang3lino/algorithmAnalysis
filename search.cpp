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

// bibliotecas necesarias para la programacion concurrente
#include <thread>
#include <mutex>
#include <atomic>
#include <memory>
#include <future>

// Funciones mejoradas de C
#include <cassert> // aserciones de C
#include <cstdio>

#include "bst.h"
#include "files.hpp"

const size_t NTHREADS = 4;

using namespace std;

typedef pair<int, int> int_pair;

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

bool concurrent_linear_search(const vector<int> &v, const int &value) {
  vector<future<bool>> futures;
  futures.reserve(NTHREADS);
  vector<int_pair> pairs = intervals(v.size(), NTHREADS);
  for (int i = 0; i < NTHREADS; ++i) {
    futures.push_back( async(linear_search, v, value, pairs[i]) );
  }
  for (int i = 0; i < NTHREADS; ++i) 
      if (futures[i].get()) return true;
  return false;
}

/**
 * Determina si x pertenece a un vector previamente ordenado. Se reinventa la rueda pues
 * ya existe binary_search en la STL de C++.
 * Para calcular la mitad del rango es mejor calcular l+(r-l)/2 a comparacion de (l+r)/2
 * ya que l+r puede albergar un entero muy grande.
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
bool bin_search(const vector<int> &v, const int findable, 
        const int_pair lims = make_pair(-1, -1)) {
    int a = lims.first, b  = lims.second;
    if (a == -1 && b == -1)
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
 * Funcion lambda tipo wrapper para poder medir el tiempo en ejecucion de cualquier funcion.
 * Puede que el tiempo en ejecucion incrementa ligeramente por el uso de este wrapper.
 * No puede haber parametros por defecto para la funcion en este wrapper, si se requiere
 * hay que ponerlos explicitamente.
 */
auto timeFuncInvocation = [](auto&& func, auto&&... params) {
    bool found; // variable a regresar
    cout << endl;
    chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();

    // aplicamos la funcion con los parametros dados
    found = std::forward<decltype(func)>(func)(std::forward<decltype(params)>(params)...); 

    chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>( t2 - t1 ).count();
    cout << "Tiempo de ejecucion en microsegundos: " << duration << endl;
    return found;
};

int main(int argc, char const *argv[]) {
    bool found;
    vector<int> v = read_from_file("sortedNums.txt", 10e6);
    //vector<int> v = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    int i = 0, n = v.size();
    bst<int> tree;
    tree.sorted_to_bst(v);
    //tree.inorder();

    //found = timeFuncInvocation(bin_search, v, 2147483603, make_pair(-1, -1)) ;
    //cout << found << endl;

    // creamos un functor con la funcion miembro contains de la clase bst
    //auto fun = [&] (int value) { return tree.contains(value); };
    //found = timeFuncInvocation(fun, 2147483603);
    //cout << found << endl;

    found = timeFuncInvocation(linear_search, v, 2045206160, make_pair(-1, -1)) ;
    cout << found << endl;

    //found = timeFuncInvocation(concurrent_linear_search, v, 2045206161) ;
    //cout << found << endl;

    cout << endl;
    return 0;
}
