#include "logger.h"
#include "file_reader.h"

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

int g_gl_width = 640;
int g_gl_height = 480;

static void glfw_error_callback(int error_code, const char* description) {
    LOG(ERROR) << "GLFW Error : code: " << error_code << ", message: " << description << "\n";
}

static void glfw_window_size_callback(GLFWwindow* window, int width, int height) {
    g_gl_width = width;
    g_gl_height = height;
}

int main() {
    using namespace graphix::utilities;
    Logger::initialize("opengl.log");

    if (!glfwInit()) {
        LOG(FATAL) << "Failed to initialize GLFW \n";
        return 1;
    }
    glfwSetErrorCallback(glfw_error_callback);

    //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow* window = glfwCreateWindow(g_gl_width, g_gl_height, "Hello world", 0, nullptr);
    if (!window) {
        LOG(FATAL) << "Failed to create a window \n";
        glfwTerminate();
        return 1;
    }

    glfwSetWindowSizeCallback(window, glfw_window_size_callback);
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        LOG(FATAL) << "Failed to initialize GLEW: " << glewGetErrorString(err) << "\n";
        glfwTerminate();
        return 1;
    }
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version = glGetString(GL_VERSION);

    LOG(INFO) << "Renderer: " << renderer << "\n";
    LOG(INFO) << "GL version: " << version << "\n";

    const float first_triangle[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
    };

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(first_triangle), first_triangle, GL_STATIC_DRAW);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    char* vertex_shader_content;

    {
        std::string vertex_shader_string = FileReader::read_content("../shaders/vertex_shader.glsl");
        size_t content_size = vertex_shader_string.size();
        vertex_shader_content = new char[content_size + 1];
        vertex_shader_string.copy(vertex_shader_content, content_size, 0);
        vertex_shader_content[content_size] = 0;
    }

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_content, nullptr);
    glCompileShader(vertex_shader);

    char* fragment_shader_content;

    {
        std::string fragment_shader_string = FileReader::read_content("../shaders/fragment_shader.glsl");
        size_t content_size = fragment_shader_string.size();
        fragment_shader_content = new char[content_size + 1];
        fragment_shader_string.copy(fragment_shader_content, content_size, 0);
        fragment_shader_content[content_size] = 0;
    }

    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_content, nullptr);
    glCompileShader(fragment_shader);

    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.3f, 0.0f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, g_gl_width, g_gl_height);

        glBindVertexArray(vao);
        glUseProgram(shader_program);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);

        glfwPollEvents();

        if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(window, 1);
        }
    }

    glfwTerminate();

    return 0;
}
