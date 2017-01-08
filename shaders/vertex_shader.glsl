#version 130

uniform mat4 translation_mat;
uniform mat4 rotation_z_mat;

in vec3 pos;
in vec3 color;

out vec3 frag_col;

void main() {
    gl_Position = translation_mat * rotation_z_mat * vec4(pos, 1.0);
    frag_col = color;
}
