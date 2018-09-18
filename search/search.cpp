#include <iostream>
#include <fstream> // permite el manejo de archivos
#include <string>
#include <vector>
#include <array> // biblioteca para c++ 11
#include <iterator> // definicion de ostream_iterator
#include <algorithm> // funcion generica sort

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

const size_t NTHREADS = 32;

using namespace std;

typedef pair<int, int> int_pair;

/**
 * Determina si x pertenece a un vector retornando un booleano.
 * Esta es la forma mas trivial y lenta de buscar elementos en una coleccion, tiene
 * complejidad O(n).
 */
bool linear_search(const vector<int> &v, const int &x, pair<int, int> p = make_pair(-1, -1)) {
    int a = p.first, b = p.second;
    if (p == make_pair(-1, -1)) // se procedera a buscar sobre todo el vector
        a = 0, b = v.size() - 1;
    for (int i = a; i <= b; i++)
        if (v[i] == x) return true;
    return false;
}

/**
 * Determina si x pertenece a un vector previamente ordenado. Se reinventa la rueda pues
 * ya existe binary_search en la STL de C++.
 * Para calcular la mitad del rango es mejor calcular l+(r-l)/2 a comparacion de (l+r)/2
 * ya que l+r puede albergar un entero muy grande.
 */
bool bin_search(const vector<int> &v, const int &x, int left, int right) {
    if (left > right) return false;
    int i = left + (right - left) / 2;
    if (v[i] == x) return true;
    if (x < v[i]) return bin_search(v, x, left, --i);
    return bin_search(v, x, ++i, right);
}

/**
 * Funcion de apoyo que manda a llamar a la misma sobrecargada. La complejidad del
 * algoritmo es O(n lg n).
 */
bool _bin_search(const vector<int> &v, const int findable, 
        const int_pair lims = make_pair(-1, -1)) {
    int a = lims.first, b  = lims.second;
    if (a == -1 && b == -1)
        a = 0, b = v.size() - 1;
    return bin_search(v, findable, a, b);
}


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
 * por supuesto, n > k.
 */
vector<pair<int, int>> intervals(const int n, const int k) {
    int quotient = n / k, j = 0;
    vector<pair<int, int>> pairs;
    pairs.reserve(k);
    for (int i = 0; i < k - 1; i++, j += quotient) {
        pairs.push_back(make_pair(j, j + quotient - 1));
    }
    pairs.push_back(make_pair(j, n - 1));
    return pairs;
}

bool concurrent_linear_search(const vector<int> &v, const int &value) {
  bool found = false;
  vector<future<bool>> futures;
  futures.reserve(NTHREADS);
  vector<int_pair> pairs = intervals(v.size(), NTHREADS);
  for (int i = 0; i < NTHREADS; i++) {
    futures.push_back( async(linear_search, v, value, pairs[i]) );
  }
  for (int i = 0; i < NTHREADS; i++) {
      found = found or futures[i].get();
  }
  return found;
}

/*
vector<vector<int>> partitionate(vector<int> &src, int npartitions) {
    auto pairs = intervals(src.size(), npartitions);
    vector<vector<int>> partitioned;
    partitioned.reserve(npartitions);
    for (int i = 0; i < npartitions; ++i) {
        partitioned[i].emplace_back((src.cbegin() + pairs[i].f(rst, src.cbegin() + pairs[i].second));
    }
    return partitioned;
}
*/

bool concurrent_binary_search(const vector<int> &v, const int &value) {
    bool found = false;
    vector<future<bool>> futures;
    futures.reserve(NTHREADS);
    vector<int_pair> pairs = intervals(v.size(), NTHREADS);
    for (int i = 0; i < NTHREADS; i++) 
        futures.push_back( async(_bin_search, v, value, pairs[i]) );
    for (int i = 0; i < NTHREADS; i++) 
        found = found or futures[i].get();
    return found;
}

int main(int argc, char const *argv[]) {
    vector<int> v = read_from_file("numeros10millones.txt", 10e6);
    //vector<int> v = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    //vector<int> v = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
    //vector<int> v; 
	//cout << v << endl;
    vector<int_pair> pairs = intervals(v.size(), NTHREADS);
    for (auto p: pairs)  // ordenamos por tramos
        sort(v.begin() + p.first, v.begin() + p.second + 1);
    cout << concurrent_binary_search(v, 322486) << endl;
    cout << endl;
    return 0;
}
