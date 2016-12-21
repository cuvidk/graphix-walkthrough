#include "shader.h"

#include <map>
#include <memory>
#include <stdexcept>

namespace graphix {
namespace engine {

Shader::Shader(const ShaderType& type, const std::string& source) :
    type_(type),
    source_(source) {
    GLenum shader_type;
    
    switch (type) {
    case ShaderType::vertex_shader :
        shader_type = GL_VERTEX_SHADER;
        break;
    case ShaderType::fragment_shader :
        shader_type = GL_FRAGMENT_SHADER;
        break;
    }

    handle_ = glCreateShader(shader_type);
    if (!handle_) {
        throw std::runtime_error("Failed to create shader with type: " + shader_type_to_string(type));
    }

    GLchar* shader_source = new GLchar[source.size() + 1];
    source.copy(shader_source, source.size(), 0);
    shader_source[source.size()] = 0;

    glShaderSource(handle_, 1, &shader_source, nullptr);
    glCompileShader(handle_);

    delete [] shader_source;

    check_compile_status();
}

inline const std::string& Shader::shader_type_to_string(const ShaderType& type) const {
    static std::map<ShaderType, std::string> shader_type_to_string_mapper = {
        { ShaderType::vertex_shader, "GL_VERTEX_SHADER" },
        { ShaderType::fragment_shader, "GL_FRAGMENT_SHADER" }
    };

    return shader_type_to_string_mapper[type];
}

void Shader::check_compile_status() const {
    GLint param;
    glGetShaderiv(handle_, GL_COMPILE_STATUS, &param);
    if (param != GL_TRUE) {
        glGetShaderiv(handle_, GL_INFO_LOG_LENGTH, &param);
        std::unique_ptr<GLchar[]> info{new GLchar[param]};
        glGetShaderInfoLog(handle_, param, nullptr, info.get());
        throw std::runtime_error(std::string("Shader compilation error: ") + info.get());
    }
}

} /* namespace engine */
} /* namespace graphix */
