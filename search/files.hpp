
#ifndef __files_h__
#define __files_h__

#include <vector>
#include <fstream>
#include <string>

#include <assert.h>

using namespace std;

vector<int> read_from_file(const char * path, const int n) {
    vector<int> loaded;
    ifstream stream(path); // flujo de entrada
    int temp, i = 0;
    //assert(stream.is_open());
    while (i < n && stream >> temp) {
        loaded.push_back(temp);
        i++;
    }
    stream.close();
    return loaded;
}

void create_subfile(const char *path, const char *name, size_t lines) {
    int i = 0;
    ifstream istream(path);
    ofstream ostream(name);
    string tmp;
    assert(istream.is_open());
    while (i++ < lines && istream >> tmp) ostream << tmp << endl;
    ostream.close();
    istream.close();
}

#endif // !1f __files_h__
