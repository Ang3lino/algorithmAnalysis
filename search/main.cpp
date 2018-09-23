
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


// Funciones mejoradas de C
#include <cassert> // aserciones de C
#include <cstdio>

// bibliotecas personales
#include "files.hpp"
#include "LagrangePolynomial.hpp"
#include "search.hpp"
#include "matplotlibcpp.h"
#include "bst.h"

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
    tree.sorted_to_bst(v);
    //LagrangePolynomial lagpol;
    
    // obtenemos un fuctor de la funcion amiga contains de la instancia de bst
    auto fun_contains = [&](int value) { return tree.contains(value); };

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
            for (int j = 0; j < nvec.size(); ++j) {
                vector<double> subv(v.begin(), v.begin() + nvec[j]);
                tie(found, duration) = timeFuncInvocation(fun_contains, findable);
                avgs[j] += duration;
                cout << findable << " | " << setw(spaces) << nvec[j] << " | " << setw(spaces) << duration;
                cout << " | " << setw(spaces) << (found ? "simon" : "nel") << endl;
                cout << "------------------------------------------------------\n";
            }
        }

        break;
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
        break;
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