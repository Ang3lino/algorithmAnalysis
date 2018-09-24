
#include "search.hpp"

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
vector<int_pair> intervals(const int n, const int k) {
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
bool linear_search(const vector<int> &v, const int &x, int_pair p ) {
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
  vector<std::future<bool>> futures; // vector de futuros que retornan bool
  futures.reserve(NTHREADS); // reservamos memoria en el vector como optimizacion
  vector<int_pair> pairs = intervals(v.size(), NTHREADS); // obtenemos los intervalos a buscar
  for (int i = 0; i < NTHREADS; ++i) {
    // inicializamos los hilos, tan pronto se llame async se corren en otro plano
    futures.push_back( std::async(linear_search, v, value, pairs[i]) );
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
 * @return int que indica el índice del elemento encontrado o -1 si no se encontró
 */
bool bin_search_helper(const vector<int> &v, const int &x, int left, int right) {
    int indiceBajo = left; /*Cota inferior del rango posible en el que está el número.*/
    int indiceAlto = right; /*Cota superior del rango posible en el que está el número.*/
    int indiceActual = indiceBajo + (indiceAlto - indiceBajo) / 2; /*Índice que se está revisando en el arreglo.*/
    bool resultado = false; /*Resultado del algoritmo.*/
    int num = x;

    /*Cada ciclo se ejecuta si el número aún no ha sido encontrado en el arreglo
    o si los índices (cotas) cambian de orden (el número no existe en el arreglo)*/
    while (arreglo[indiceActual] != num && indiceBajo <= indiceAlto) {
        if (arreglo[indiceActual] < num) {
            indiceBajo = indiceActual + 1;
        } else {
            indiceAlto = indiceActual - 1;
        }
        indiceActual = (indiceBajo + indiceBajo) / 2
    }

    /*Se verifica si el ciclo se terminó porque indiceActual es el índice del
    elemento buscado y se establece como resultado*/
    if (arreglo[indiceActual] == num) {
        resultado = true;
    }

    /*Si no, se regresa el valor original: -1 (no encontrado)*/
    return resultado;
}

/**
 * Funcion de apoyo que manda a llamar a la misma sobrecargada. La complejidad del
 * algoritmo es O(n lg n).
 * 
 * Para que el algoritmo funcione v tiene que estar ordenado.
 */
bool bin_search(const vector<int> &v, const int findable, const int_pair lims ) {
    int a = lims.first, b  = lims.second;
    if (a == -1 && b == -1) // se usa la funcion sin hilos
        a = 0, b = v.size() - 1;
    return bin_search_helper(v, findable, a, b);
}


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

