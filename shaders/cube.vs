layout (location = 0) in vec3 vertex_pos;
layout (location = 1) in vec3 position;
layout (location = 2) in int block_type;

flat out int vs_block_type;
out vec3 vs_uvw;

void main() {
    vs_block_type = block_type;
    gl_Position = vec4(vertex_pos + position, 1.0);
    vs_uvw = vertex_pos;
}

