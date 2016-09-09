#ifndef _GEOMETRY_HPP
#define _GEOMETRY_HPP

#include <cassert>
#include <vector>
#include <complex>
#include <cmath>

#include "../myGL.hpp"

/* vertex buffer data header; itself doesn't contain data */
template <typename T> class ContinuousVertexBuffer{
public:
    const std::vector<T>& stl_container;

private:
    GLsizei m_verticesCount;
    GLint m_positionVecDimension;
    const typename std::vector<T>::size_type m_vertices_begin = 0;

    GLint m_colorVecDimension;
    const typename std::vector<T>::size_type m_colors_begin;

public:
    ContinuousVertexBuffer<T>(
            const typename std::vector<T>& container,
            int vertices_count, int position_vector_dim, int color_vector_dim,
            typename std::vector<T>::size_type colors_begin
    ) :
            stl_container(container),
            m_verticesCount(vertices_count),
            m_positionVecDimension(position_vector_dim),
            m_colorVecDimension(color_vector_dim),
            m_colors_begin(colors_begin) {
    };

    ~ContinuousVertexBuffer(){};

public:
    GLsizei verticesCount() const {
        return this->m_verticesCount;
    };

    GLint positionVecDimension() const {
        return this->m_positionVecDimension;
    };

    GLsizeiptr bufferSize() const {
        return sizeof(T) * this->stl_container.size();
    };

    const typename std::vector<T>::value_type* bufferData() const {
        return this->stl_container.data();
    };

    GLenum dataType() const {
        if (sizeof(GLfloat) <=  sizeof(T) && sizeof(T) < sizeof(GLdouble)) {
            return GL_FLOAT;
        } else { // if (sizeof(GLdouble) <=  sizeof(T))
            return GL_DOUBLE;
        }
    };

    const void* positionDataOffset() const {
        return (void*)(this->m_vertices_begin * sizeof(T));
    };

    GLint colorVecDimension() const {
        return this->m_colorVecDimension;
    };

    const void* colorDataOffset() const {
        return (void*)(this->m_colors_begin * sizeof(T));
    };
};



/* generate the vertices of a regular triangle; color the three vertices with Red, Green and Blue */
/* pass an empty stl container ref to hold the data; if needed catch the header returns */
template <typename T> ContinuousVertexBuffer<T> coloredTriangle(T radius, typename std::vector<T>& vertices) {
    int angles = 3;

    assert(vertices.size() == 0);
    // vertices = std::vector<float>(); // All elements existed are eliminated

    std::complex<T> unit_root(
            std::cos(2.0 * T(M_PI) / angles),
            std::sin(2.0 * T(M_PI) / angles)
    );

    std::complex<T> n_unit_root;
    for (int i = 0; i <= angles - 1; ++i) {
        n_unit_root = std::pow(unit_root, i);
        /* append vertices data */
        std::vector<T> coordinates = {radius * n_unit_root.real(), radius * n_unit_root.imag(), 0.0};
        vertices.insert(vertices.end(), coordinates.begin(), coordinates.end());
    }

    auto color_data_begin = vertices.size(); // position data ends and color data begins

    for (int i = 0; i <= angles - 1; ++i) {
        /* append color rgb data */
        std::vector<T> rgb = {0.0, 0.0, 0.0};
        rgb[i % 3] = 1.0;
        vertices.insert(vertices.end(), rgb.begin(), rgb.end());
    }

    return ContinuousVertexBuffer<T>(vertices, 3, 3, 3, color_data_begin);
};

#endif