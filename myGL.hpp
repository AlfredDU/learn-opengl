//
// Created by pallas athena on 16/8/23.
//
#ifndef _MYGL_HPP
#define _MYGL_HPP


#include <OpenGL/gl3.h>

#include <cassert>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

/* avoid warning "gl.h and gl3.h are both included" */
#ifdef __APPLE__
# define __gl_h_
# define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
#endif

/*include OpenCV libraries.*/
#include <cassert>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>


/* read shader file */
std::string readShaderFile(const std::string &file) {

    std::ifstream reader(file);
    std::stringstream source;

    if (reader.is_open()) {
        source << reader.rdbuf();
        reader.close();
    } else {
        std::cerr << "Unable to read shader file: " + file << std::endl;
        exit(EXIT_FAILURE);
    }

    return source.str();
}

/* compile shaders */
GLuint compileShaders(const std::string &vertexShaderFile, const std::string &fragmentShaderFile) {

    // Read our shaders into the appropriate buffers
    std::string vertexSource = readShaderFile(vertexShaderFile); // Get source code for vertex shader.
    std::string fragmentSource = readShaderFile(fragmentShaderFile); // Get source code for fragment shader.

    // Create an empty vertex shader handle
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // Send the vertex shader source code to GL
    // Note that std::string's .c_str is NULL character terminated.
    const GLchar *source = vertexSource.c_str();
    glShaderSource(vertexShader, 1, &source, 0);

    // Compile the vertex shader
    glCompileShader(vertexShader);

    GLint isCompiled = 0;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
    if(isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(static_cast<std::vector<GLchar>::size_type>(maxLength));
        glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

        // We don't need the shader anymore.
        glDeleteShader(vertexShader);

        // Use the infoLog as you see fit.
        std::cerr << infoLog.data() << std::endl;

        // In this simple program, we'll just leave
        exit(EXIT_FAILURE);
    }

    // Create an empty fragment shader handle
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Send the fragment shader source code to GL
    // Note that std::string's .c_str is NULL character terminated.
    source = fragmentSource.c_str();
    glShaderSource(fragmentShader, 1, &source, 0);

    // Compile the fragment shader
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
    if(isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(static_cast<std::vector<GLchar>::size_type>(maxLength));
        glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

        // We don't need the shader anymore.
        glDeleteShader(fragmentShader);
        // Either of them. Don't leak shaders.
        glDeleteShader(vertexShader);

        // Use the infoLog as you see fit.
        std::cerr << infoLog.data() << std::endl;

        // In this simple program, we'll just leave
        exit(EXIT_FAILURE);
    }

    // Vertex and fragment shaders are successfully compiled.
    // Now time to link them together into a program.
    // Get a program object.
    GLuint program = glCreateProgram();

    // Attach our shaders to our program
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    // Link our program
    glLinkProgram(program);

    // Note the different functions here: glGetProgram* instead of glGetShader*.
    GLint isLinked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
    if(isLinked == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(static_cast<std::vector<GLchar>::size_type>(maxLength));
        glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

        // We don't need the program anymore.
        glDeleteProgram(program);
        // Don't leak shaders either.
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        // Use the infoLog as you see fit.
        std::cerr << infoLog.data() << std::endl;

        // In this simple program, we'll just leave
        exit(EXIT_FAILURE);
    }

    // Always detach shaders after a successful link.
    glDetachShader(program, vertexShader);
    glDetachShader(program, fragmentShader);


    return program;
}


/* vertex buffer data header; itself doesn't contain data */
/* data are arranged as pattern of "PNTCPNTCPNTC" (position-normal-texture corrds-color) */
template <typename T> class VertexBufferHeader{
public:
    const std::vector<T>& stl_container;

private:
    GLsizei m_verticesCount;
    GLint m_positionVecDimension;
    GLint m_normalVecDimension;
    GLint m_uvVecDimension;
    GLint m_colorVecDimension;

public:
    VertexBufferHeader<T>(
            const typename std::vector<T>& container,
            int vertices_count, int position_vector_dim, int normal_vector_dim,
            int uv_vector_dim, int color_vector_dim
    ) :
            stl_container(container),
            m_verticesCount(vertices_count),
            m_positionVecDimension(position_vector_dim),
            m_normalVecDimension(normal_vector_dim),
            m_uvVecDimension(uv_vector_dim),
            m_colorVecDimension(color_vector_dim) {};

    ~VertexBufferHeader(){};

public:
    GLsizei verticesCount() const {
        return this->m_verticesCount;
    };

    GLint positionVecDimension() const {
        return this->m_positionVecDimension;
    };

    GLint normalVecDimension() const {
        return this->m_normalVecDimension;
    };

    GLint uvVecDimension() const {
        return this->m_uvVecDimension;
    };

    GLint colorVecDimension() const {
        return this->m_colorVecDimension;
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
        return (void*)0;
    };

    const void* normalDataOffset() const {
        return (void*)(sizeof(T) * this->m_positionVecDimension);
    };

    const void* uvDataOffset() const {
        return (void*)(sizeof(T) * (this->m_positionVecDimension + this->m_normalVecDimension));
    };

    const void* colorDataOffset() const {
        return (void*)(
                sizeof(T) * (this->m_positionVecDimension + this->m_normalVecDimension + this->m_uvVecDimension));
    };

    const GLsizei stride() const {
        return sizeof(T) * (this->m_positionVecDimension + this->m_normalVecDimension + this->m_uvVecDimension +
                            this->m_colorVecDimension);
    };
};


/* load & generate texture map with OpenCV libraries */
GLuint loadRgbTexture(const std::string &imageFile) {
    // since opengl deprecated GL_LUMINANCE for greyscale picture, here force to load image with RGB format;
    // load image with alpha channel pls. call another function
    cv::Mat cv_image = cv::imread(imageFile, CV_LOAD_IMAGE_COLOR);

    // assertion to avoid potential exceptions
    assert(cv_image.type() == CV_8UC3);

    // opengl default regards the bytes numbers of each row as multiple of 4; if not the value of unpack alignment
    // bytes must be set to 1; however, use 4 as possible as you can for fast processing
    const GLint GL_DEFAULT_PIXEL_ALIGNMENT = 4, GL_MIN_PIXEL_ALIGNMENT = 1;
    glPixelStorei(
            GL_UNPACK_ALIGNMENT,
            cv_image.step[0] % GL_DEFAULT_PIXEL_ALIGNMENT == 0 ? GL_DEFAULT_PIXEL_ALIGNMENT : GL_MIN_PIXEL_ALIGNMENT
    );

    // start pointer stride of each row data; may note be column numbers since opencv doesn't necessarily store row
    // data continuously.
    glPixelStorei(
            GL_UNPACK_ROW_LENGTH,
            (GLint)(cv_image.step[0] / cv_image.elemSize())
    );

    // reverse the pixel order for opencv matrix coordinates at top-left corner which opposite to opengl's behavior.
    cv::Mat cv_image_reversed(cv_image.size(), cv_image.type());
    cv::flip(cv_image, cv_image_reversed, 0);

    // generate texture object
    GLuint texture_id;
    {
        glGenTextures(1, &texture_id);
        glBindTexture(GL_TEXTURE_2D, texture_id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        glTexImage2D(
                GL_TEXTURE_2D,
                0,                          // must be 0
                GL_RGB,                     // opengl pixel format
                cv_image_reversed.cols,     // columns
                cv_image_reversed.rows,     // rows
                0,                          // borders
                GL_BGR,                     // opencv pixel format
                GL_UNSIGNED_BYTE,           // data type; corresponding to unsigned char
                cv_image_reversed.data      // pointer to data
        );
    }

    return texture_id;
};


#endif
