
#ifndef __test_h__
#define __test_h__

#include <iostream>
#include "search.cpp"
#include "files.hpp"

using std::vector;
using std::cout;

namespace test {
    void linear_search() {
        vector<int> v = read_from_file("10millones.txt", 1e4);
        for (int x: v) cout << x << ", ";
        cout << endl;
        cout << linear_search(v, 1234) << endl;
        cout << linear_search(v, 1226985352) << endl;
    }

    void bst_search() {
        vector<int> v = read_from_file("10millones.txt", 1e5);
        bst<int> tree(v);
        //for (int x: v) cout << x << ", ";
        cout << endl;
        cout << tree.contains(856834115) << endl;
        cout << tree.contains(966245083) << endl;
        cout << tree.contains(2045206161) << endl;
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

}

#endif // !___test_h__
