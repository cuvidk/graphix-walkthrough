#version 130

in vec3 frag_col;
out vec4 frag_color;

void main() {
    frag_color = vec4(frag_col, 1.0f);
}
