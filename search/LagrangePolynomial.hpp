
#ifndef __LAGRANGE_POL_H__
#define __LAGRANGE_POL_H__

#include <vector>
#include <algorithm> // for_each
#include <numeric> // accumulate

#include <cassert>

typedef std::pair<double, double> double_pair;

class LagrangePolynomial {
private:
    int degree;
    std::vector<double_pair> points;

    double lag_coef(const int &k, const int &x) {
        double prod = 1;
        const double xk = points[k].first;
        for (int i = 0; i < points.size(); ++i) {
            if (i != k) {
                const double xi = points[i].first;
                prod *= (x - xi) / (xk - xi);
            } 
        }
        return prod;
    }
    
public:

    LagrangePolynomial() { degree = 0; }
    
    LagrangePolynomial(std::vector<double_pair> pairs) : points(pairs), degree(pairs.size() - 1) { }

    LagrangePolynomial(std::vector<double> xvec, std::vector<double> yvec) {
        assert(xvec.size() == yvec.size());
        for (int i = 0; i < xvec.size(); i++) {
            points.emplace_back(xvec[i], yvec[i]);
        }
    }

    inline void add_point(const double &x, const double &y) {
        points.emplace_back(x, y);
        ++degree;
    }

    /** Establece los puntos para trabajar con la instancia.
     *  Si ya se han definido previamente se remplazan. */
    inline void set_points(std::vector<double_pair> pairs) {
        points = pairs;
        degree = pairs.size() - 1;
    }

    double eval(double x) {
        double sum = 0.0;
        for (int i = 0; i < points.size(); ++i) {
            double yi = points[i].second;
            sum += yi * lag_coef(i, x);
        }
        return sum;
    }

};

#endif