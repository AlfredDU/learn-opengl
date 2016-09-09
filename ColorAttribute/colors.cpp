#include "../myGL.hpp"
#include "Geometry.hpp"
#include "../Context.hpp"


/* Constants. */
static const GLint width = 600;
static const GLint height = 480;

const std::string vertex_shader_file = "plane.vert";
const std::string fragment_shader_file = "smooth_color.frag";

/* triangle data */
std::vector<GLfloat> vertex_data;
/* create a header object for convenient call & cast */
auto header = coloredTriangle(0.8f, vertex_data);

/* gl context */
class Window : public GLContext {

private:
    GLuint vertex_array; /* VAO object */
    GLuint vertex_buffer; /* VBO object */
    GLuint program_id; /* shaders */

    void initialize() {
        /* create VBO object */
        glGenBuffers(1, &vertex_buffer);
        {
            glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
            glBufferData(GL_ARRAY_BUFFER, header.bufferSize(), header.bufferData(), GL_STATIC_DRAW);
        }

        /* create VAO object */
        glGenVertexArrays(1, &vertex_array);
        {
            glBindVertexArray(vertex_array);

            glEnableVertexAttribArray(0); // position data
            glEnableVertexAttribArray(1); // color data

            glVertexAttribPointer( // position data
                    0, // counterpart of layout in the shader
                    header.positionVecDimension(), // components per vertex
                    header.dataType(), // data type
                    GL_FALSE, // whether normalized
                    0, // stride
                    header.positionDataOffset() // offset
            );
            glVertexAttribPointer( // color data
                    1, // counterpart of layout in the shader
                    header.colorVecDimension(), // components per vertex
                    header.dataType(), // data type
                    GL_FALSE, // whether normalized
                    0, // stride
                    header.colorDataOffset() // offset
            );
        }

        /* create and compile shaders */
        program_id = compileShaders(vertex_shader_file, fragment_shader_file);

        /* Apply the shader */
        glUseProgram(program_id);
    };

    void draw() {
        glDrawArrays(
                GL_TRIANGLES, // surfel type
                0, // starting index
                header.verticesCount() // indices to be rendered
        );
    };

    void destroy() {
        /* Destroy gl objects */
        glDeleteBuffers(1, &vertex_buffer);
        glDeleteVertexArrays(1, &vertex_array);
        glDeleteProgram(program_id);
    };
};


int main(int argc, char* argv[]) {
    Window w;
    w.setEnvironment();
    w.createWindow(width, height, "Colorful triangle.");

    w.mainloop();

    return EXIT_SUCCESS;
}