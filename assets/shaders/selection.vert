#version 430 core
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoords;

uniform mat4 model;

layout(std140, binding = 0) uniform FrameUniforms
{
    mat4 view;
    mat4 projection;
    vec3 viewPos;
    float _pad0;
};

void main(){
	gl_Position = projection * view * model * vec4(vPos, 1.0);
}