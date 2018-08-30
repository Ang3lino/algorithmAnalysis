// aqui esta krnal v:

#include <iostream>
#include <vector>

using namespace std;

// auto es un tipo de dato dinamico
void print_vector(vector<auto> v) {
	cout << "[ ";
	for (int i = 0; i < v.size(); i++)
		cout << v[i] ;
	cout << " ]" << endl;
}

// al poner & en el parametro lo pasamos por referencia
void base(long m, const long b, vector<long> &solution) {
	long r = m % b;
	long n = (m - r) / b; // n < m
	if (n > 0) base(n, b, solution);
	solution.push_back(r); // ponemos el valor al final del vector
}

vector<long> base(long m, const long b) {
	vector<long> sol;
	base(m, b, sol);
	return sol;
}


int main() {
	long b = 2;
	long n;
	cout << "n: ";
	cin >> n;
	std::vector<long> v = base(n, b);
	print_vector(v);
	return 0;
}
