#include "shader_program.h"

#include <stdexcept>
#include <algorithm>

namespace graphix {
namespace engine {

ShaderProgram::ShaderProgram() : handle_(glCreateProgram()) {
    if (!handle_) {
        throw std::runtime_error("Shader program creation failed");
    }
}

void ShaderProgram::attach_shader(Shader&& shader) {
    auto result = std::find_if(attached_shader_.begin(), attached_shader_.end(),
            [&shader](const Shader& attached) -> bool {
                return attached == shader || attached.type_ == shader.type_;
            });

    if (result != attached_shader_.end()) {
        throw std::runtime_error("Shader attachment failed. Attempt to attach "
                                 "already attached shader or shader type. Shader type: "
                                 + shader.shader_type_to_string(shader.type_));
    }

    glAttachShader(handle_, shader.handle_);

    attached_shader_.emplace_back(std::move(shader));
}

} /* namespace engine */
} /* namespace graphix */
