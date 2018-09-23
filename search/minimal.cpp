#include "matplotlibcpp.h"
#include "LagrangePolynomial.hpp"
#include <vector>

using namespace std;

namespace plt = matplotlibcpp;

int main() {
    vector<double> x = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    vector<double> y = {1, 4, 9, 16, 25, 36, 49, 64, 81};
    LagrangePolynomial lpol(x, y);
    vector<double> a, b;
    for (double i = 0; i < x.size(); i += 0.1) {
        a.push_back(i);
        b.push_back(lpol.eval(i));
    }
    plt::plot(x,y);
    plt::show();
}