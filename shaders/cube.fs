in vec4 gs_color;
in vec4 light_direction;
flat in vec4 normal;

void main() {
    // wow nice moves

	float dot_nl = dot(normalize(light_direction), normalize(normal));
	dot_nl = clamp(dot_nl, 0.0, 1.0);
	gl_FragColor = clamp(gs_color * dot_nl, 0.0, 1.0);

    // gl_FragColor = gs_color * dot(-light_direction, normal);
    // gl_FragColor = normal + -light_direction;
}

