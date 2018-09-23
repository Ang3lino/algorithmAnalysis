
#ifndef __SEARCH_HPP__
#define __SEARCH_HPP__

#include <vector>

// bibliotecas necesarias para la programacion concurrente
#include <thread>
#include <mutex>
#include <atomic>
#include <memory>
#include <future>

using std::vector;
using std::make_pair;

typedef std::pair<int, int> int_pair;

const int NTHREADS = 4;
vector<int_pair> intervals(const int n, const int k);

bool linear_search(const vector<int> &v, const int &x, int_pair p = make_pair(-1, -1)); 
bool concurrent_linear_search(const vector<int> &v, const int &value);
bool bin_search_helper(const vector<int> &v, const int &x, int left, int right);
bool bin_search(const vector<int> &v, const int findable, int_pair lims = make_pair(-1, -1));

/*
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
*/



#endif // !__SEARCH_HPP__
