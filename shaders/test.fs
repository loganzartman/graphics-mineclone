#version 330 core
uniform ivec2 resolution;
uniform float time;

void main() {
    // wow nice moves
    float f = gl_FragCoord.x / resolution.x;
    gl_FragColor = vec4(sin(f * 10 + time * 5), sin(f * 8 + time * 4), sin(f * 6 + time * 6), 1.);
}

