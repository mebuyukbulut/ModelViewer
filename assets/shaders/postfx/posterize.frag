#version 430 core

in vec2 fTexCoords;
out vec4 FragColor;

uniform float u_value = 5.0;

layout(binding = 0) uniform sampler2D frameTex;

void main(){
    vec3 color = texture(frameTex, fTexCoords).rgb;
    float levels = u_value;
    color = floor(color * levels) / levels;
    FragColor = vec4( color, 1.0f);
}