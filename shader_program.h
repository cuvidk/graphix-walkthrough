#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include "shader.h"
#include "non_copyable.h"

#include <vector>

#include <GL/glew.h>

namespace graphix {
namespace engine {
namespace shader {

class ShaderProgram : public graphix::engine::utilities::NonCopyable {
public:
    ShaderProgram();
    ShaderProgram(ShaderProgram&& other);
    ShaderProgram& operator=(ShaderProgram&& other);
    ~ShaderProgram();

    void attach_shader(Shader&& shader);
    void link() const;
    void use() const;

    std::string info() const;
    const Shader& shader_by_type(const Shader::ShaderType& shader_type);

private:
    void validate() const;

    void check_link_status() const;
    void check_validate_status() const;

private:
    std::vector<Shader> attached_shader_;
    GLuint handle_;
};

} /* namespace shader */
} /* namespace engine */
} /* namespace graphix */

#endif /* SHADER_PROGRAM_H */
