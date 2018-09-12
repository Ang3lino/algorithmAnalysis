#ifndef ALGORITMOS_H
#define ALGORITMOS_H
#include <fstream>
#include <iostream>
#include <cassert>
#include <string>
#include <vector>
#include "BinarySearchTree.hpp"

std::vector<int> read_from_file (const char * path, const int n);

void create_subfile (const char *path, const char *name, size_t lines);

void swap (int &a, int &b);

void bubblesort (std::vector<int> &v);

void bubblesort_optimized (std::vector<int> &v);

void selectionsort (std::vector<int> &v);

void insertionsort (std::vector<int> &v);

void shellsort (std::vector<int> &v);

void treesort (std::vector<int> &v);

#endif