#ifndef SHADER_H
#define SHADER_H

#include <string>

#include <GL/glew.h>

namespace graphix {
namespace engine {

class Shader {
public:
    enum class ShaderType {
        vertex_shader,
        fragment_shader
    };

public:
    Shader(const ShaderType& type, const std::string& source);

    const GLuint& handle() const { return handle_; }


private:
    const std::string& shader_type_to_string(const ShaderType& type) const;
    void check_compile_status() const;

private:
    ShaderType type_;
    std::string source_;
    GLuint handle_;
};

} /* namespace engine */
} /* namespace graphix */

#endif /* SHADER_H */
