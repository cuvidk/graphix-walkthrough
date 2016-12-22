#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include "shader.h"

#include <vector>

#include <GL/glew.h>

namespace graphix {
namespace engine {

class ShaderProgram {
public:
    ShaderProgram();
    void attach_shader(Shader&& shader);
private:
    std::vector<Shader> attached_shader_;
    GLuint handle_;
};

} /* namespace engine */
} /* namespace graphix */

#endif /* SHADER_PROGRAM_H */
