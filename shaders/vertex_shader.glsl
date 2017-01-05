#version 130

in vec3 pos;
in vec3 color;

out vec3 frag_col;

void main() {
    gl_Position = vec4(pos, 1.0);
    frag_col = color;
}
