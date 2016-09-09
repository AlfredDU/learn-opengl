//
// Created by pallas athena on 16/8/30.
//

#ifndef _GL_CONTEXT_H
#define _GL_CONTEXT_H

#include <exception>
#include <string>
#include <functional>
#include <iostream>
#include <GLFW/glfw3.h>


/* exceptions */
class GLFW3InitError : std::exception {
public:
    const char* what() {
        return "Failed to initialize GLFW.\n";
    };
};

class GLFW3CreateWindowError : std::exception {
public:
    const char* what() {
        return "Failed to open GLFW window.\n";
    };
};


/* default callback functions; use free function to avoid std::function targeting problems */
namespace default_callbacks{
    void error (int error, const char *description) {
        (void)error;
        std::cerr << description << std::endl;
    };

    void key (GLFWwindow *window, int key, int scancode, int action, int mods) {
        (void)scancode, (void)mods;
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);
    };

    void resize (GLFWwindow *window, int w, int h) {
        (void) w, (void)h;
        glfwSwapBuffers(window);
    };
};


/* opengl context using GLFW3, with methods encapsulated */
class GLContext{

private:
    GLFWwindow* window; // window to hold

public:
    /* callback function signature */
    typedef std::function<void(int, const char*)> ErrorCallback;
    typedef std::function<void(GLFWwindow*, int, int, int, int)> KeyCallback;
    typedef std::function<void(GLFWwindow*, int, int)> ResizeCallback;

protected:
    /* callback functions; can be overrode by subclasses; implemented by std::function. */
    ErrorCallback error_callback = default_callbacks::error; // Handle potential glfw errors.
    KeyCallback key_callback = default_callbacks::key; // Key Events callbacks
    ResizeCallback window_size_callback = default_callbacks::resize; // Resize callbacks

public:
    GLContext(){ // constructor
        glfwSetErrorCallback(*(this->error_callback.target<GLFWerrorfun>()));

        /* Initialize GLFW */
        if (!glfwInit()) { // if initialization failed
            /* Terminate GLFW */
            glfwTerminate();
            this->window = nullptr;
            throw GLFW3InitError();
        }
    };

    ~GLContext() { // destructor
        /* Terminate GLFW */
        glfwTerminate();
        this->window = nullptr;
    };

public:
    virtual void setEnvironment() { // set window & opengl hints
        /* GLFW window hints */
        glfwWindowHint(GLFW_RESIZABLE, GL_TRUE); // Window related hint; others: GLFW_VISIBLE, GLFW_FOCUS
        glfwWindowHint(GLFW_SAMPLES, 4); // Framebuffer related hint; set multi-sampling/anti-alias
        glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE); //Framebuffer related hint; set double buffer

        /* Below OpenGL API configurations are essential for specified platforms / graphic hardware */
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // set required major OpenGL versions (3.x)
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // set required minimum OpenGL versions (3.x)
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Mac only supports core profile for 3.x
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Mac only supports forward-compatible contexts for 3.x
    };

    virtual void createWindow(const GLint width, const GLint height, const std::string& title) { // create window
        /* Creating a window and rendering context */
        this->window = glfwCreateWindow(
                width, // width
                height, // height
                title.c_str(), // title
                NULL, // monitor
                NULL // share resources to other window
        );

        /* Assert window creation succeeds */
        if(! this->window){
            std::cerr << "Failed to open GLFW window.\n" << std::endl;
            throw GLFW3CreateWindowError();
        }

        /* Making the OpenGL context current */
        glfwMakeContextCurrent(this->window);
        /* Waiting interval between swapping buffers; the default value 1 recommended */
        glfwSwapInterval(1);

        /* Designate Key action event callback */
        glfwSetKeyCallback(this->window, *(this->key_callback.target<GLFWkeyfun>()));

        /* Window refresh callback; mainly to force redraw when resizing */
        glfwSetWindowSizeCallback(this->window, *(this->window_size_callback.target<GLFWwindowsizefun>()));
    };

protected:
    /* methods that most likely to be overrode by subclass */

    virtual void prepare() { // prepare opengl context
        /* Background color. */
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    };

    virtual void initialize() { /* initialize data objects */ };

    virtual void draw() { /* draw */ };

    virtual void destroy() { /* distructor */};

public:
    virtual void mainloop() { // main loop
        this->prepare();
        this->initialize();

        while(!glfwWindowShouldClose(this->window)) {
            /* Viewport */
            int _width, _height;
            glfwGetFramebufferSize(this->window, &_width, &_height);
            // use frame buffer size instead of windows size for retina monitor adjustment
            _width >= _height ?
            glViewport((_width - _height) / 2, 0, _height, _height) :
            glViewport(0, (_height - _width) / 2, _width, _width); // Aspect ratio always 1

            /* draw */
            this->draw();

            /* Swap buffers */
            glfwSwapBuffers(this->window);

            /* Processing action callbacks */
            glfwPollEvents();
        }

        this->destroy();

        /* Destroy GLFW window */
        glfwDestroyWindow(this->window);
    };
};


#endif //_GL_CONTEXT_H
