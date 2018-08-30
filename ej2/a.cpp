#include <iostream>

using namespace std;

void one(int n) {
	int a = new int[n];
	for (int i = 1; i < n; i++) 
		for (j = 0; j < n - 1; j++) {
			int tmp = a[j];
			a[j] = a[j + 1];
			a[j + 1] = tmp;
		}
}

void two(int n) {
	int a = new int[n];
	for (int i = 1; i < n; i++) 
		for (j = 0; j < n - 1; j++) {
			int tmp = a[j];
			a[j] = a[j + 1];
			a[j + 1] = tmp;
		}
}

