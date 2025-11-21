#version 330 core
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoords;

out vec3 uvCoords;

uniform mat4 projection;
uniform mat4 view;

void main() {    
    uvCoords = vPos; //vTexCoords;
    gl_Position = projection * view * vec4(vPos, 1.0);

}
