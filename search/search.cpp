#include <iostream>
#include <fstream> // permite el manejo de archivos
#include <string>
#include <vector>
#include <array> // biblioteca para c++ 11
#include <iterator> // definicion de ostream_iterator 

// bibliotecas necesarias para la programacion concurrente
#include <thread>
#include <mutex>
#include <atomic>
#include <memory>

// Funciones mejoradas de C
#include <cassert> // aserciones de C
#include <cstdio>

#include "bst.h"
#include "files.hpp"

const size_t NTHREADS = 4;
const int NELEMS = 0;

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
inline bool binary_search(const vector<int> &v, const int findable) {
    int r = v.size() - 1;
    return bin_search(v, findable, 0, r);
} 

void bin_search() {
    vector<int> v = read_from_file("10millones.txt", 1e5);
    for (int x: v) 
        cout << ((binary_search(v, x)) ? "found": "not found") << endl;
    cout << endl;
    cout << ((binary_search(v, 12)) ? "found": "not found") << endl;
    cout << ((binary_search(v, -1)) ? "found": "not found") << endl;
    cout << endl;
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

inline void conc_lin_search_helper(const vector<int> &v, const int &value, 
        shared_ptr<bool> &found, const int_pair p) {
    *found = linear_search(v, value, p);
}

bool concurrent_linear_search(const vector<int> &v, const int &value) {
    array<thread, NTHREADS> threads;
    shared_ptr<bool> found;
    auto pairs = intervals(v.size(), NTHREADS);
    for (int i = 0; i < NTHREADS; ++i) {
        threads[i] = thread(conc_lin_search_helper, v, value, found, pairs[i]);
        threads[i].join();
    }
    return found.get();
}

/*
vector<vector<int>> partitionate(vector<int> &src, int npartitions) {
    auto pairs = intervals(src.size(), npartitions);
    vector<vector<int>> partitioned;
    partitioned.reserve(npartitions);
    for (int i = 0; i < npartitions; ++i) {
        partitioned[i].emplace_back((src.cbegin() + pairs[i].first, src.cbegin() + pairs[i].second));
    }
    return partitioned;
}
*/

int main(int argc, char const *argv[]) {
    //vector<int> v = read_from_file("10e3.txt", 10e1);
    vector<int> v = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    //vector<int> w(v.begin() + 1, v.begin() + 4);
    //auto matrix = partitionate(v, 4);
    //for (auto x: matrix) cout << x << endl;
    cout << linear_search(v, 3, make_pair(0, 2)); 
    cout << endl;
    return 0;
}
