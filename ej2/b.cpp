#include <iostream>

using namespace std;


void six(int n) {
	for (int i = 10; i < 5 * n; i *= 2)
		cout << "\"Algoritmos\"\n";
}

void seven(int n) {
	for (int j = n; j > 1; j /= 2) {
		if (j < n / 2) {
			for (int i = 0; i < n; i += 2)
				cout << "\"Algoritmos\"\n";
		}
	}
}

void eight(int n) {
	int i = n;
	while (i >= 0) {
		for (int j = n; i < j; i -= 2, j /= 2)
			cout << "\"Algoritmos\"\n";
	}
}

int main(void) {
	cout << "n: ";
	int n;
	cin >> n;
	cout << "6.- " << endl;
	six(n);
	//cout << "7.- " << endl;
	//seven(n);
	//cout << "8.- " << endl;
	//eight(n);
		return 0;
}
