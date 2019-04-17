in vec4 vertex_pos;
in vec4 gs_color;
in vec4 light_direction;
flat in vec4 normal;

void main() {
    // wow nice moves

	float dot_nl = dot(normalize(light_direction), normalize(normal));
	dot_nl = clamp(dot_nl, 0.0, 1.0);


    float f = perlin3d(vertex_pos.xyz * 5, 1, 0.5) + 0.7;
    vec4 color = vec4(vec3(f) * vec3(0.4, 0.7, 0.1), 1.0);
	gl_FragColor = clamp(color * dot_nl, 0.0, 1.0);

    // gl_FragColor = gs_color * dot(-light_direction, normal);
    // gl_FragColor = normal + -light_direction;
}

