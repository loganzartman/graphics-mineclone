in vec2 vs_uv;

uniform sampler2D tex;
uniform sampler2D depth;
uniform bool write_depth;

void main() {
    gl_FragColor = texture(tex, vs_uv);
    if (write_depth) {
        gl_FragDepth = texture(depth, vs_uv).x;
    }
}