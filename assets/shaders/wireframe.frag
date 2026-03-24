#version 420 core

in vec3 fPos;
in vec3 fNormal;
in vec2 fTexCoords;
out vec4 FragColor;

uniform mat4 view;

void main()
{
    FragColor = vec4(1.0, 0.65, 0.45, 1.0);    
}