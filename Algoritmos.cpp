#include "Algoritmos.hpp"

std::vector<int> read_from_file (const char * path, const int n) {
    std::vector<int> loaded;
    std::ifstream stream(path); // flujo de entrada
    int temp, i = 0;
    assert(stream.is_open());
    while (i < n && stream >> temp) {
        loaded.push_back(temp);
        i++;
    }
    stream.close();
    return loaded;
}

void create_subfile (const char *path, const char *name, size_t lines) {
    int i = 0;
    std::ifstream istream(path);
    std::ofstream ostream(name);
    std::string tmp;
    assert(istream.is_open());
    while (i++ < lines && istream >> tmp) ostream << tmp << std::endl;
    ostream.close();
    istream.close();
}

void swap (int &a, int &b) {
    int tmp = a;
    a = b;
    b = tmp;
}

void bubblesort (std::vector<int> &v) {
	size_t len = v.size();
	for (int i = 0; i < len - 1; i++) {
		for (int j = 0; j < len - 1 - i; j++) {
			if (v[j] > v[j + 1]) {
				swap(v[j], v[j + 1]);
			}
		}
	}
}

void bubblesort_optimized (std::vector<int> &v) {
    size_t len = v.size();
    bool change_presented;
    for (int i = 0; i < len; i++) {
        change_presented = false;
        for (int j = 0; j < len - i - 1; j++) {
            if (v[j] > v[j + 1]) {
                swap(v[j], v[j + 1]);
                change_presented = true;
            }
        }
        if (!change_presented) return;
    }
}

void selectionsort (std::vector<int> &v) {
    size_t n = v.size();
    for (int i = 0; i < n; i++) {
        int minpos = i;
        for (int j = i + 1; j < n; j++) {
            if (v[j] < v[minpos]) {
                minpos = j;
            }
        }
        swap(v[i], v[minpos]);
    }
}

void insertionsort (std::vector<int> &v) {
    size_t n = v.size();
    for (int i = 1; i < n; i++) {
        int j = i;
        int val = v[i];
        while (j > 0 && v[j - 1] > val) {
            v[j] = v[j - 1];
            j--;
        }
        v[j] = val;
    }
}

void shellsort (std::vector<int> &v) {
    size_t n = v.size();
    int gap = 1;
    while (gap < n / 3) gap = gap * 3 + 1;
    while (gap > 0) {
        for (int i = gap; i < n; i++) {
            int tmp = v[i];
            int j = i;
            while (j > gap - 1 && v[j - gap] >= tmp) {
                v[j] = v[j - gap];
                j -= gap;
            }
            v[j] = tmp;
        }
        gap = (gap - 1) / 3;
    }
}

void treesort (std::vector<int> &v) {
    BinarySearchTree<int> tree;
    tree.sort(v);
}