#ifndef SHADER_H
#define SHADER_H

#include "non_copyable.h"

#include <string>

#include <GL/glew.h>

namespace graphix {
namespace engine {

class Shader : public graphix::utilities::NonCopyable {
    friend class ShaderProgram;

public:
    enum class ShaderType {
        vertex_shader,
        fragment_shader
    };

public:
    Shader(const ShaderType& type, const std::string& source);
    Shader(ShaderType&& type, std::string&& source);
    Shader(Shader&& other);
    Shader& operator=(Shader&& other);
    ~Shader();

    bool operator==(const Shader& other) const;

    const ShaderType& type() const;
    const std::string& source() const;

    void destroy();

    //TODO: to be deleted
    const GLuint& handle() const { return handle_; }

private:
    void create();

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
