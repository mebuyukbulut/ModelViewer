// Fragment shader
#version 330 core
in vec2 uv;
out vec4 FragColor;
void main() {
    //vec3 top = vec3(0.2, 0.3, 0.5);
    //vec3 bottom = vec3(0.8, 0.8, 0.9);

    // "Mystic Nightfall"
    vec3 bottom = vec3(0.035, 0.062, 0.180);
    vec3 top    = vec3(0.149, 0.227, 0.447);

    FragColor = vec4(mix(bottom, top, uv.y), 1.0);
}
