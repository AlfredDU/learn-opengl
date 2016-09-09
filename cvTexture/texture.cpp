#include "../myGL.hpp"
#include "../Context.hpp"


/* Constants. */
static const GLint width = 600;
static const GLint height = 480;

const std::string vertex_shader_file_color = "color.vert";
const std::string fragment_shader_file_color = "color.frag";
const std::string vertex_shader_file_texture = "texture.vert";
const std::string fragment_shader_file_texture = "texture.frag";
const std::string texture_image = "Lenna.png";

/* two triangles, one with color data and the other with texture coordinates */
std::vector<GLfloat> vertex_data = {
    // position             // texture        // color
    0.5f, -0.5f, 0.0f,      1.0f, 0.0f,       1.0f, 0.0f, 0.0f,
    0.5, 0.5, 0.0f,         1.0f, 1.0f,       0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, 0.0f,      0.0f, 1.0f,       0.0f, 0.0f, 1.0f,
    -0.5f, 0.5f, 0.0f,      0.0f, 1.0f,       1.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, 0.0f,     0.0f, 0.0f,       0.5f, 0.5f, 0.5f,
    0.5f, -0.5f, 0.0f,      1.0f, 0.0f,       0.0f, 0.0f, 0.0f
};
/* create a header object for convenient call & cast */
auto header = VertexBufferHeader<GLfloat>(vertex_data, 6, 3, 0, 2, 3);

/* gl context */
class Window : public GLContext {

private:
    GLuint vertex_buffer; /* VBO object */
    GLuint vertex_array_color; /* VAO objects */
    GLuint vertex_array_texture;
    GLuint program_id_color; /* shaders */
    GLuint program_id_texture;
    GLuint texture; /* texture */

    void initialize() {
        /* create VBO object */
        glGenBuffers(1, &vertex_buffer);
        {
            glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
            glBufferData(GL_ARRAY_BUFFER, header.bufferSize(), header.bufferData(), GL_STATIC_DRAW);
        }

        /* two VAO objects; one with color but no texture; the other with texture but no color */
        glGenVertexArrays(1, &vertex_array_texture);
        {
            glBindVertexArray(vertex_array_texture);

            glEnableVertexAttribArray(0); // position data
            glEnableVertexAttribArray(1); // texture coordinates data

            glVertexAttribPointer( // position data
                    0, // counterpart of layout in the shader
                    header.positionVecDimension(), // components per vertex
                    header.dataType(), // data type
                    GL_FALSE, // whether normalized
                    header.stride(), // stride
                    header.positionDataOffset() // offset
            );
            glVertexAttribPointer( // texture coordinates data
                    1, // counterpart of layout in the shader
                    header.uvVecDimension(), // components per vertex
                    header.dataType(), // data type
                    GL_FALSE, // whether normalized
                    header.stride(), // stride
                    header.uvDataOffset() // offset
            );

            glBindVertexArray(0);
        }

        glGenVertexArrays(1, &vertex_array_color);
        {
            glBindVertexArray(vertex_array_color);

            glEnableVertexAttribArray(0); // position data
            glEnableVertexAttribArray(1); // color data

            glVertexAttribPointer( // position data
                    0, // counterpart of layout in the shader
                    header.positionVecDimension(), // components per vertex
                    header.dataType(), // data type
                    GL_FALSE, // whether normalized
                    header.stride(), // stride
                    header.positionDataOffset() // offset
            );
            glVertexAttribPointer( // color data
                    1, // counterpart of layout in the shader
                    header.colorVecDimension(), // components per vertex
                    header.dataType(), // data type
                    GL_FALSE, // whether normalized
                    header.stride(), // stride
                    header.colorDataOffset() // offset
            );

            glBindVertexArray(0);
        }

        /* create and compile shaders */
        program_id_color = compileShaders(vertex_shader_file_color, fragment_shader_file_color);
        program_id_texture = compileShaders(vertex_shader_file_texture, fragment_shader_file_texture);

        /* create texture */
        texture = loadRgbTexture(texture_image);

        /* Apply the shader */
        glUseProgram(program_id_color);
    };

    void draw() {
        glBindVertexArray(vertex_array_color);
        glUseProgram(program_id_color);
        glDrawArrays(
                GL_TRIANGLES, // surfel type
                0, // starting index
                header.verticesCount() // indices to be rendered
        );

        glBindVertexArray(vertex_array_texture);
        glUseProgram(program_id_texture);
        glDrawArrays(
                GL_TRIANGLES, // surfel type
                header.verticesCount() / 2, // starting index
                header.verticesCount() // indices to be rendered
        );
    };

    void destroy() {
        /* Destroy gl objects */
        glDeleteBuffers(1, &vertex_buffer);
        glDeleteVertexArrays(1, &vertex_array_color);
        glDeleteVertexArrays(1, &vertex_array_texture);
        glDeleteProgram(program_id_color);
        glDeleteProgram(program_id_texture);

        glDeleteTextures(1, &texture);
    };
};


int main(int argc, char* argv[]) {
    Window w;
    w.setEnvironment();
    w.createWindow(width, height, "Color & Texture");

    w.mainloop();

    return EXIT_SUCCESS;
}