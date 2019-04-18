in vec4 vertex_pos;
in vec3 uvw;
flat in int block_type;
in vec4 light_direction;
flat in vec4 normal;

uniform ivec2 resolution;
uniform vec3 look;
uniform mat4 projection;
uniform mat4 view;
uniform sampler2D background;
uniform sampler2D depth;
uniform float z_near;
uniform float z_far;
uniform float time;

float depth_to_ndc(float d) {
    // https://stackoverflow.com/a/6657284/1175802
    float z_n = 2.0 * d - 1.0;
    float z_e = 2.0 * z_near * z_far / (z_far + z_near - z_n * (z_far - z_near));
    return z_e;
}

void main() {
    // wow nice moves
	float dot_nl = dot(normalize(light_direction), normalize(normal));
	dot_nl = clamp(dot_nl, 0.0, 1.0);
    
    vec2 screen_pos = gl_FragCoord.xy / resolution;
    float d_background_cam = depth_to_ndc(texture(depth, screen_pos).x);
    float d_foreground_cam = gl_FragCoord.z / gl_FragCoord.w;
    float d_total = d_background_cam - d_foreground_cam;

    vec3 color = texture(background, gl_FragCoord.xy / resolution).rgb;

    float f = clamp(d_total / 5., 0.5, 1);
    float f2 = clamp(d_total / 10., 0, 1);
    vec3 water_color = vec3(0.1, 0.5, 0.8);
	vec3 transmissive = color * (1-f) + water_color * color * f;
    transmissive = transmissive * (1-f2) + pow(water_color, vec3(f2 * 8)) * f2;
    gl_FragColor = vec4(transmissive, 1.);
}