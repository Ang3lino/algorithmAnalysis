#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <cassert>

#include "bst.h"
#include "files.hpp"

using namespace std;

bool linear_search(const vector<int> &v, int x) {
    for (int value: v) if (value == x) return true;
    return false;
}

/**
 * Determina si x pertenece a un vector ordenado.
 * Para calcular la mitad del rango es mejor calcular l+(r-l)/2 a comparacion de (l+r)/2
 * ya que l+r puede albergar un entero muy grande.
 */
bool bin_search(const vector<int> &v, const int x, int left, int right) {
    if (left > right) return false;
    int i = left + (right - left) / 2; 
    if (v[i] == x) return true;
    if (x < v[i]) return bin_search(v, x, left, --i); 
    return bin_search(v, x, ++i, right); 
}

inline bool binary_search(const vector<int> &v, const int findable) {
    int r = v.size() - 1;
    return bin_search(v, findable, 0, r);
} 

int main(int argc, char const *argv[]) {
    vector<int> v = read_from_file("10e3sorted.txt", 1e7);
    //vector<int> v = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    return 0;
}
