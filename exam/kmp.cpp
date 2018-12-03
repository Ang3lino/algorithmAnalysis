
#include <string>
#include <iostream>
#include <vector>

using namespace std;

vector<int> computeTable(string src) {
    int i = 1, j = 0, n = src.size();
    vector<int> pi(n);
    pi[0] = 0;
    while (i < n) {
        if (src[i] == src[j]) {
            pi[i] = j + 1;
            ++j;
            ++i;
        } else {
            if (j != 0) {
                j = pi[j - 1];
            } else {
                pi[i] =0;
                ++i;
            }
        }
    }
    return pi;
}

bool kmp(string text, string pattern) {
    vector<int> pi = computeTable(pattern);

    for (int k = 0; k < pi.size(); ++k) cout << pattern[k];
    cout << endl; 
    for (int k = 0; k < pi.size(); ++k) cout << pi[k];
    cout << endl;
    for (int k = 0; k < pi.size(); ++k) cout << k ;
    cout << endl;

    int i = 0, j = 0;
    while (i < text.size() && j < pattern.size()) {
        if (text[i] == pattern[j]) {
            ++i;
            ++j;
        } else {
            if (j != 0) 
                j = pi[j - 1];
            else 
                ++i;
        }
    } 
    return j == pattern.size();
}

int main(void) {
    //string str = "abcxabcdabcdabcy";
    //string subString = "abcdabcy";
    string str ;
    string subString ;
    cout << "String: ";
    cin >> str;
    cout << "Pattern: ";
    cin >> subString;

    bool result = kmp(str, subString);
    cout << endl << (result) << endl;
    return 0;
}