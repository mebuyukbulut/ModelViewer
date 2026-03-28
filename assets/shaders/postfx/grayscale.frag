#version 430 core

in vec2 fTexCoords;
out vec4 FragColor;

layout(binding = 0) uniform sampler2D frameTex;

void main(){
    vec4 a = texture(frameTex, fTexCoords);

    float value = 0.299 * a.r + 0.587 * a.g + 0.114 * a.b;
    FragColor = vec4(value, value, value, 1.0f);
}