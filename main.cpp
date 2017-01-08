#include "logger.h"
#include "file_reader.h"
#include "shader.h"
#include "shader_program.h"
#include "mat4.h"

#include <iostream>
#include <cassert>
#include <chrono>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

static int g_gl_width = 640;
static int g_gl_height = 480;

static void glfw_error_callback(int error_code, const char* description) {
    LOG(ERROR) << "GLFW Error : code: " << error_code << ", message: " << description << "\n";
}

static void glfw_window_size_callback(GLFWwindow* window, int width, int height) {
    g_gl_width = width;
    g_gl_height = height;
}

static void update_fps_counter() {
    static float frame_count = 0;
    static auto last_time = std::chrono::high_resolution_clock::now();
    auto crt_time = std::chrono::high_resolution_clock::now();
    auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(crt_time - last_time).count();

    ++frame_count;

    if (elapsed_time > 250) {
        std::cout << "FPS: " << frame_count / (elapsed_time / 1000.0f) << std::endl;
        frame_count = 0;
        last_time = crt_time;
    }
}

int main() {
    using namespace graphix::engine::utilities;
    using namespace graphix::engine::shader;
    using namespace graphix::engine::math;
    try {
        Logger::initialize("opengl.log");

        LOG(INFO) << "Starting GLFW\n";
        glfwSetErrorCallback(glfw_error_callback);
        if (!glfwInit()) {
            LOG(FATAL) << "Failed to initialize GLFW \n";
            return 1;
        }
        glfwSetErrorCallback(glfw_error_callback);

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
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

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        const float first_triangle[] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f, 0.5f, 0.0f
        };

        const float colors[] = {
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f
        };

        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(first_triangle), first_triangle, GL_STATIC_DRAW);

        GLuint color_vbo;
        glGenBuffers(1, &color_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);


        GLuint vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        ShaderProgram shader_program;
        shader_program.attach_shader({Shader::ShaderType::vertex_shader,
                FileReader::read_content("../shaders/vertex_shader.glsl")});
        shader_program.attach_shader({Shader::ShaderType::fragment_shader,
                FileReader::read_content("../shaders/fragment_shader.glsl")});

        glBindAttribLocation(shader_program.handle(), 0, "pos");
        glBindAttribLocation(shader_program.handle(), 1, "color");

        shader_program.link();

        Mat4 translation_mat{};
        Mat4 rotation_crazines{};

        GLint uniform_translation_mat = glGetUniformLocation(shader_program.handle(), "translation_mat");
        GLint uniform_rotation_z_mat = glGetUniformLocation(shader_program.handle(), "rotation_z_mat");
        assert(uniform_translation_mat > -1);
        assert(uniform_rotation_z_mat > -1);

        while (!glfwWindowShouldClose(window)) {
            update_fps_counter();

            glClearColor(0.3f, 0.0f, 0.5f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glViewport(0, 0, g_gl_width, g_gl_height);

            glBindVertexArray(vao);
            shader_program.use();
            glUniformMatrix4fv(uniform_translation_mat, 1, GL_TRUE, (float*)&translation_mat);
            glUniformMatrix4fv(uniform_rotation_z_mat, 1, GL_TRUE, (float*)&rotation_crazines);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            glfwSwapBuffers(window);

            glfwPollEvents();

            if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE)) {
                glfwSetWindowShouldClose(window, 1);
            } else if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_RIGHT)) {
                translation_mat.translate(0.009, 0.0f, 0.0f);
            } else if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_LEFT)) {
                translation_mat.translate(-0.009f, 0.0f, 0.0f);
            } else if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_UP)) {
                translation_mat.translate(0.0f, 0.009f, 0.0f);
            } else if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_DOWN)) {
                translation_mat.translate(0.0f, -0.009f, 0.0f);
            }
            rotation_crazines.rotate_x_axis(0.01f);
            rotation_crazines.rotate_y_axis(0.01f);
            rotation_crazines.rotate_z_axis(0.01f);
        }

        glfwTerminate();
    } catch (std::exception& e) {
        LOG(FATAL) << e.what() << "\n";
        std::cout << "FATAL: " << e.what() << "\nSee: " << Logger::log_destination()
                  << " for more debugging info." << std::endl;
    }

    return 0;
}
