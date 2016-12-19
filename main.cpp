#include "logger.h"
#include "file_reader.h"

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

int main() {
    using namespace graphix::utilities;
    Logger::initialize("opengl.log");

    if (!glfwInit()) {
        LOG(FATAL) << "Failed to initialize GLFW \n";
        return 1;
    }

    GLFWwindow* window = glfwCreateWindow(640, 480, "Hello world", 0, NULL);
    if (!window) {
        LOG(FATAL) << "Failed to create a window \n";
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);

    glewExperimental = true;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        LOG(FATAL) << "Failed to initialize GLEW: " << glewGetErrorString(err) << "\n";
        glfwTerminate();
        return 1;
    }

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    char* vertex_shader_content;

    {
        std::string vertex_shader_string = FileReader::read_content("../shaders/vertex_shader.glsl");
        size_t content_size = vertex_shader_string.size();
        vertex_shader_content = new char[content_size + 1];
        vertex_shader_string.copy(vertex_shader_content, content_size, 0);
        vertex_shader_content[content_size] = 0;
    }

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_content, NULL);
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
    glShaderSource(fragment_shader, 1, &fragment_shader_content, NULL);
    glCompileShader(fragment_shader);

    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.3f, 0.0f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(vao);
        glUseProgram(shader_program);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
