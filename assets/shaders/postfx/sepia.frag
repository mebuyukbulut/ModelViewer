#version 430 core

in vec2 fTexCoords;
out vec4 FragColor;

layout(binding = 0) uniform sampler2D frameTex;

void main(){
    vec3 color = texture(frameTex, fTexCoords).rgb;

    color = vec3(
        dot(color, vec3(0.393, 0.769, 0.189)),
        dot(color, vec3(0.349, 0.686, 0.168)),
        dot(color, vec3(0.272, 0.534, 0.131))
    );
    FragColor = vec4(color, 1.0f);
}