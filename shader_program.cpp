#include "shader_program.h"
#include "logger.h"

#include <stdexcept>
#include <algorithm>
#include <string>

namespace graphix {
namespace engine {
namespace shader {

ShaderProgram::ShaderProgram() : handle_(glCreateProgram()) {
    if (!handle_) {
        throw std::runtime_error("Shader program creation failed.");
    }
}

ShaderProgram::ShaderProgram(ShaderProgram&& other) {
    this->operator=(std::move(other));
}

ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other) {
    handle_ = std::move(other.handle_);
    attached_shader_ = std::move(other.attached_shader_);
    other.handle_ = 0;
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(handle_);
}

void ShaderProgram::attach_shader(Shader&& shader) {
    glAttachShader(handle_, shader.handle_);
    if (glGetError() != GL_NO_ERROR) {
        throw std::runtime_error{info() + ". Shader attachment failed. Attempt to attach "
                                 "already attached shader or shader type. Shader type: "
                                 + shader.shader_type_to_string(shader.type_)};
    }

    attached_shader_.emplace_back(std::move(shader));
}

void ShaderProgram::link() const {
    glLinkProgram(handle_);
    check_link_status();

    validate();
}

void ShaderProgram::use() const {
    glUseProgram(handle_);
}

std::string ShaderProgram::info() const {
    return "ShaderProgram: [ handle: " + std::to_string(handle_) + " ]";
}

const Shader& ShaderProgram::shader_by_type(const Shader::ShaderType& shader_type) {
    auto result = std::find_if(attached_shader_.begin(), attached_shader_.end(),
            [&shader_type] (const Shader& shader) -> bool {
                if (shader.type_ == shader_type) {
                    return true;
                }
                return false;
            });
    if (result == attached_shader_.end()) {
        throw std::runtime_error{info() + ". No shader of type: " + Shader::shader_type_to_string(shader_type) +
                                 " attached."};
    }
    return *result;
}

void ShaderProgram::validate() const {
    glValidateProgram(handle_);
    check_validate_status();
}

void ShaderProgram::check_link_status() const {
    GLint param;
    std::unique_ptr<GLchar[]> log_info;
    glGetProgramiv(handle_, GL_INFO_LOG_LENGTH, &param);
    if (param > 1) {
        log_info.reset(new GLchar[param]);
        glGetProgramInfoLog(handle_, param, nullptr, log_info.get());
        LOG(DEBUG) << info() << ". " << log_info.get() << "\n";
    }
    glGetProgramiv(handle_, GL_LINK_STATUS, &param);
    if (param != GL_TRUE) {
        throw std::runtime_error{info() + ". Shader program linking error: " + log_info.get()};
    }
}

void ShaderProgram::check_validate_status() const {
    GLint param;
    std::unique_ptr<GLchar[]> log_info;
    glGetProgramiv(handle_, GL_INFO_LOG_LENGTH, &param);
    if (param > 1) {
        log_info.reset(new GLchar[param]);
        glGetProgramInfoLog(handle_, param, nullptr, log_info.get());
        LOG(DEBUG) << info() << ". " << log_info.get() << "\n";
    }
    glGetProgramiv(handle_, GL_VALIDATE_STATUS, &param);
    if (param != GL_TRUE) {
        throw std::runtime_error{info() + ". Shader program validation error: " + log_info.get()};
    }
}

} /* namespace shader */
} /* namespace engine */
} /* namespace graphix */
