in vec4 vertex_pos;
in vec3 uvw;
in vec4 gs_color;
in vec4 light_direction;
flat in vec4 normal;

vec4 dirt_sample(vec4 vertex_pos, vec3 uvw) {
    float f = noise((vertex_pos.xz + vertex_pos.zy) * 8, 3, 0.5) * 0.25 + 0.5;
    return vec4(vec3(f) * vec3(0.6, 0.4, 0.2), 1.);
}

vec4 grass_sample(vec4 vertex_pos, vec3 uvw) {
    if (uvw.y > 0.8) {
        // grass
        float f = noise((vertex_pos.xz * vec2(2, 1) + vertex_pos.zy) * 8, 3, 0.5) * 0.25 + 0.5;
        return vec4(vec3(f) * vec3(0.8, 1.0, 0.4), 1.);
    } else {
        // dirt
        return dirt_sample(vertex_pos, uvw);
    }
}

void main() {
    // wow nice moves
	float dot_nl = dot(normalize(light_direction), normalize(normal));
	dot_nl = clamp(dot_nl, 0.0, 1.0);
    float light = clamp(0.5 + dot_nl, 0.0, 1.0);

    vec4 color = vec4(1, 1, 0, 1);
    color = grass_sample(vertex_pos, uvw);
    
	gl_FragColor = clamp(color * light, 0.0, 1.0);
}
