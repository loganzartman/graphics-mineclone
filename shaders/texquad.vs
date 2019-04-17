layout (location = 0) in vec2 vertex_pos;
layout (location = 1) in vec2 uv;

uniform vec2 bottom_left;
uniform vec2 top_right;

out vec2 vs_uv;

void main() {
    vs_uv = uv;
    vec2 pos = vertex_pos * (top_right - bottom_left) + bottom_left;
    gl_Position = vec4(pos, 0, 1);
}