in vec4 vertex_pos;
in vec3 uvw;
flat in int block_type;
in vec4 light_direction;
flat in vec4 normal;

uniform ivec2 resolution;
uniform vec3 camera_position;
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

vec3 project(vec3 v, vec3 onto) {
    float len_onto = length(onto);
    return dot(onto, v) / (len_onto * len_onto) * onto;
}

vec3 fake_refract(vec3 I, vec3 N, float eta) {
    float k = 1.0 - eta * eta * (1.0 - dot(N, I) * dot(N, I));
    if (k < 0.0) {
        // the fake part: just force the refraction to work
        k = 0;
    }
    return eta * I - (eta * dot(N, I) + sqrt(k)) * N;
}

void main() {
    // compute a fake normal
    const float epsilon = 0.001;
    vec3 base_pos = vec3(vertex_pos.x * 0.5, time + vertex_pos.z * 0.5, vertex_pos.z * 0.5);
    float height_c = perlin3d(base_pos);
    float height_x = perlin3d(base_pos + vec3(epsilon, 0, 0));
    float height_z = perlin3d(base_pos + vec3(0, 0, epsilon));
    vec3 frag_normal = normalize(vec3((height_x - height_c) / epsilon, 1, (height_z - height_c) / epsilon));

    vec3 ray_direction = normalize(vec3(vertex_pos) - camera_position);
	float dot_nl = dot(normalize(light_direction), normalize(normal));
	dot_nl = clamp(dot_nl, 0.0, 1.0);
    
    vec2 screen_pos = gl_FragCoord.xy / resolution;
    float d_background_cam = depth_to_ndc(texture(depth, screen_pos).x);
    float d_foreground_cam = gl_FragCoord.z / gl_FragCoord.w;
    float d_total = d_background_cam - d_foreground_cam;

    // transmissive with super fake refractions
    vec3 refracted_dir = normalize(fake_refract(ray_direction, frag_normal, 1.33));
    vec3 refracted_ray = refracted_dir * d_total;
    vec3 refraction_offset = refracted_ray - project(refracted_ray, ray_direction);
    vec2 refracted_pos = screen_pos + refraction_offset.xz * 0.01; 
    vec3 color = texture(background, refracted_pos).rgb;

    // recompute depth with refraction
    d_background_cam = depth_to_ndc(texture(depth, refracted_pos).x);
    d_foreground_cam = gl_FragCoord.z / gl_FragCoord.w;
    d_total = d_background_cam - d_foreground_cam;

    float f = clamp(d_total / 5., 0.5, 1);
    float f2 = clamp(d_total / 10., 0, 1);
    vec3 water_color = vec3(0.1, 0.5, 0.8);
	vec3 transmissive = color * (1-f) + water_color * color * f;
    transmissive = transmissive * (1-f2) + pow(water_color, vec3(f2 * 8)) * f2;

    // specular
    vec3 reflected_dir = reflect(vec3(light_direction), frag_normal);
    float f_specular = max(dot(ray_direction, reflected_dir), 0.);
    vec4 specular = pow(f_specular, 8) * vec4(0.5, 0.5, 0.5, 0);

    gl_FragColor = vec4(transmissive, 1.) + specular;
    // gl_FragColor = vec4(height_c, specular.r, 0, 1.);
    // gl_FragColor = vec4(ray_direction.rgb * 0.5 + 0.5, 1.);
}