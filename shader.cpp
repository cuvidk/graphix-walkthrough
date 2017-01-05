#include "shader.h"
#include "logger.h"

#include <map>
#include <memory>
#include <stdexcept>

namespace graphix {
namespace engine {
namespace shader {

Shader::Shader(const ShaderType& type, const std::string& source) :
    type_(type),
    source_(source) {
    create();
}

Shader::Shader(ShaderType&& type, std::string&& source) :
    type_(std::move(type)),
    source_(std::move(source)) {
    create();
}

Shader::Shader(Shader&& other) {
    this->operator=(std::move(other));
}

Shader& Shader::operator=(Shader&& other) {
    type_ = std::move(other.type_);
    source_ = std::move(other.source_);
    handle_ = std::move(other.handle_);
    other.handle_ = 0;
    return *this;
}

Shader::~Shader() {
    if (handle_) {
        glDeleteShader(handle_);
    }
}

bool Shader::operator==(const Shader& other) const {
    return handle_ == other.handle_;
}

const Shader::ShaderType& Shader::type() const {
    return type_;
}

const std::string& Shader::source() const {
    return source_;
}

std::string Shader::info() const {
    return "Shader: [ handle: " + std::to_string(handle_) +
        ", type: " + shader_type_to_string(type_) + " ]";
}

void Shader::create() {
    GLenum shader_type;

    switch (type_) {
    case ShaderType::vertex_shader :
        shader_type = GL_VERTEX_SHADER;
        break;
    case ShaderType::fragment_shader :
        shader_type = GL_FRAGMENT_SHADER;
        break;
    }

    handle_ = glCreateShader(shader_type);
    if (!handle_) {
        throw std::runtime_error{"Failed to create shader with type: " + shader_type_to_string(type_)};
    }

    GLchar* shader_source = new GLchar[source_.size() + 1];
    source_.copy(shader_source, source_.size(), 0);
    shader_source[source_.size()] = 0;

    glShaderSource(handle_, 1, &shader_source, nullptr);
    glCompileShader(handle_);

    delete [] shader_source;

    check_compile_status();
}

inline const std::string& Shader::shader_type_to_string(const ShaderType& type) {
    static std::map<ShaderType, std::string> shader_type_to_string_mapper = {
        { ShaderType::vertex_shader, "GL_VERTEX_SHADER" },
        { ShaderType::fragment_shader, "GL_FRAGMENT_SHADER" }
    };

    return shader_type_to_string_mapper[type];
}

void Shader::check_compile_status() const {
    GLint param;
    std::unique_ptr<GLchar[]> log_info;
    glGetShaderiv(handle_, GL_INFO_LOG_LENGTH, &param);
    if (param > 1) {
        log_info.reset(new GLchar[param]);
        glGetShaderInfoLog(handle_, param, nullptr, log_info.get());

        glGetShaderiv(handle_, GL_COMPILE_STATUS, &param);
        if (param != GL_TRUE) {
            throw std::runtime_error{info() + ". Shader compilation error: " + log_info.get()};
        }

        LOG(DEBUG) << info() << ". " << log_info.get() << "\n";
    }
}

} /* namespace shader */
} /* namespace engine */
} /* namespace graphix */
