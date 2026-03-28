#version 430 core

in vec2 fTexCoords;
out vec4 FragColor;

layout(binding = 0) uniform sampler2D frameTex;

void main(){
    vec3 color = texture(frameTex, fTexCoords).rgb;
    color = vec3(1.0f) - color;
    FragColor = vec4(color, 1.0f);
}