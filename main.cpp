#include "logger.h"
#include "file_reader.h"
#include "shader.h"

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

void glfw_error_callback(int error_code, const char* description) {
    LOG(ERROR) << "GLFW Error: code: " << error_code << ", message: " << description << ".\n";
}

int main() {
    using namespace graphix::utilities;
    Logger::initialize("opengl.log");

    LOG(INFO) << "Starting GLFW\n";
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        LOG(FATAL) << "Failed to initialize GLFW \n";
        return 1;
    }
    
    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow* window = glfwCreateWindow(640, 480, "Hello world", 0, nullptr);
    if (!window) {
        LOG(FATAL) << "Failed to create a window \n";
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        LOG(FATAL) << "Failed to initialize GLEW: " << glewGetErrorString(err) << "\n";
        glfwTerminate();
        return 1;
    }

    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version = glGetString(GL_VERSION);
    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "Opengl version: " << version << std::endl;


    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

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

    using namespace graphix::engine;
    Shader vertex_shader{Shader::ShaderType::vertex_shader, FileReader::read_content("../shaders/vertex_shader.glsl")};
    Shader fragment_shader{Shader::ShaderType::fragment_shader, FileReader::read_content("../shaders/fragment_shader.glsl")};

    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader.handle());
    glAttachShader(shader_program, fragment_shader.handle());
    glLinkProgram(shader_program);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.3f, 0.0f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(vao);
        glUseProgram(shader_program);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
