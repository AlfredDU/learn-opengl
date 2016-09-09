#ifndef _GEOMETRY_HPP
#define _GEOMETRY_HPP

#include <vector>
#include <complex>
#include <cmath>

/* return the vertices of a regular polygon */
std::vector<float> regularPolygon(float radius, int angles) {
    std::vector<float> vertices;
    std::complex<float> unit_root(
            std::cos(2.0f * float(M_PI) / angles),
            std::sin(2.0f * float(M_PI) / angles)
    );

    std::complex<float> n_unit_root;
    for (int i = 0; i <= angles - 1; ++i) {
        n_unit_root = std::pow(unit_root, i);
        vertices.push_back(radius * n_unit_root.real());
        vertices.push_back(radius * n_unit_root.imag());
        vertices.push_back(0.0f);
    }

    return vertices;
};

#endif