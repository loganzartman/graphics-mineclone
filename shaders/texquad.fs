in vec2 vs_uv;

uniform sampler2D tex;

void main() {
    gl_FragColor = texture(tex, vs_uv + sin(vs_uv.x * 10) * 0.01);
}