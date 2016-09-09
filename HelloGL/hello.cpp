#include "../myGL.hpp"
#include "Geometry.hpp"
#include <GLFW/glfw3.h>


/* Constants. */
static const GLint width = 600;
static const GLint height = 480;

/* triangle data */
static const std::vector<GLfloat> vertex_position_data = regularPolygon(0.8f, 3);

const std::string vertex_shader_file = "plane.vert";
const std::string fragment_shader_file = "solid_color.frag";


/* Handle potential glfw errors. */
static void error_callback(int error, const char* description);

/* Events callbacks */
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void window_size_callback(GLFWwindow* window, int w, int h);


int main(int argc, char* argv[])
{
    GLFWwindow* window;

    glfwSetErrorCallback(error_callback);


    /* Initialize GLFW */
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW.\n" << std::endl;
        return EXIT_FAILURE;
    }

    /* GLFW window hints */
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE); // Window related hint; others: GLFW_VISIBLE, GLFW_FOCUS
    glfwWindowHint(GLFW_SAMPLES, 4); // Framebuffer related hint; set multi-sampling/anti-alias
    glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE); //Framebuffer related hint; set double buffer

    /* Below OpenGL API configurations are essential for specified platforms / graphic hardware */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // set required major OpenGL versions (3.x)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // set required minimum OpenGL versions (3.x)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Mac only supports core profile for 3.x
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Mac only supports forward-compatible contexts for 3.x


    /* Creating a window and rendering context */
    window = glfwCreateWindow(
            width, // width
            height, // height
            "Hello GL", // title
            NULL, // monitor
            NULL // share resources to other window
    );

    /* Assert window creation succeeds */
    if(!window){
        std::cerr << "Failed to open GLFW window.\n" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    /* Making the OpenGL context current */
    glfwMakeContextCurrent(window);
    /* Waiting interval between swapping buffers; the default value 1 recommended */
    glfwSwapInterval(1);

    /* Designate Key action event callback */
    glfwSetKeyCallback(window, key_callback);

    /* Window refresh callback; mainly to force redraw when resizing */
    glfwSetWindowSizeCallback(window, window_size_callback);


    /* Background color. */
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    /* create VBO object */
    GLuint vertex_buffer; {
        glGenBuffers(1, &vertex_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

        // bind data
        glBufferData(
                GL_ARRAY_BUFFER,
                sizeof(decltype(vertex_position_data.front())) * vertex_position_data.size(),
                vertex_position_data.data(),
                GL_STATIC_DRAW
        );
    }

    /* create VAO object */
    GLuint vertex_array; {
        glGenVertexArrays(1, &vertex_array);
        glBindVertexArray(vertex_array);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(
                0, // counterpart of layout in the shader
                3, // components per vertex
                GL_FLOAT, // data type
                GL_FALSE, // whether normalized
                sizeof(decltype(vertex_position_data.front())) * 3, // stride
                (GLvoid*)0 // offset
        );
    }


    /* create and compile shaders */
    GLuint program_id = compileShaders(vertex_shader_file, fragment_shader_file);

    /* Apply the shader */
    glUseProgram(program_id);


    /* Main loop */
    while(!glfwWindowShouldClose(window)) {

        /* Clear screen */
        glClear( GL_COLOR_BUFFER_BIT );

        /* Viewport */
        int _width, _height;
        glfwGetFramebufferSize(window, &_width, &_height);
            // use frame buffer size instead of windows size for retina monitor adjustment
        _width >= _height ?
            glViewport((_width - _height) / 2, 0, _height, _height) :
            glViewport(0, (_height - _width) / 2, _width, _width); // Aspect ratio always 1


        /* Render work */
        glDrawArrays(
                GL_TRIANGLES, // surfel type
                0, // starting index
                3 // indices to be rendered
        );

        /* Swap buffers */
        glfwSwapBuffers(window);

        /* Processing action callbacks */
        glfwPollEvents();

    }


    /* Destroy gl objects */
    glDeleteBuffers(1, &vertex_buffer);
    glDeleteVertexArrays(1, &vertex_array);
    glDeleteProgram(program_id);


    /* Destroy GLFW window */
    glfwDestroyWindow(window);

    /* Terminate GLFW */
    glfwTerminate();


    return EXIT_SUCCESS;
}


void static error_callback(int error, const char* description) {
    (void)error;

    std::cerr << description << std::endl;
}

void static key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    (void)scancode, (void)mods;

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

void window_size_callback(GLFWwindow *window, int w, int h) {
    (void) w, (void)h;

    glfwSwapBuffers(window);
}
