#version 330 core
in vec4 vs_color;
uniform ivec2 resolution;
uniform float time;

void main() {
    // wow nice moves
    float f = gl_FragCoord.x / resolution.x;
    gl_FragColor = vs_color + vec4(sin(time + f), 0., 0., 0.);
}

