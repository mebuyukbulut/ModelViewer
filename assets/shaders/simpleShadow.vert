#version 430 core
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoords;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;
//uniform mat4 lightView;
//uniform mat4 lightProjection;
//uniform float near_plane;
//uniform float far_plane;

void main()
{
    // try LinearizeDepth for perspective projection  
    gl_Position = lightSpaceMatrix * model * vec4(vPos, 1.0);
}  