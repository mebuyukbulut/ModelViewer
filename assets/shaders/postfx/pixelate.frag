#version 430 core

in vec2 fTexCoords;
out vec4 FragColor;

layout(binding = 0) uniform sampler2D frameTex;

void main(){
    vec2 size = vec2(320.0, 180.0); 
    vec2 uv2 = floor(fTexCoords * size) / size;

    vec3 color = texture(frameTex, uv2).rgb;
    FragColor = vec4( color, 1.0f);
}