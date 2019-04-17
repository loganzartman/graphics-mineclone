layout (location = 0) in vec3 vertex_pos;
layout (location = 1) in vec2 position;
layout (location = 2) in vec4 color;

out vec4 vs_color;

void main() {
    vs_color = color;
    gl_Position = vec4(vertex_pos + vec3(position, 0.), 1.0);
}

