#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTexCoords;

out vec3 vPos;
out vec3 vNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

//uniform vec3 lightPos;
//uniform vec3 viewPos;
////uniform vec3 lightColor;
//uniform float lightIntensity;
//uniform float albedo;

out vec3 ourColor;

#define _PI 3.1415926535897932384626433832795

void main()
{
	vPos = aPos;
	vNormal = aNormal;

	// Calculate the position of the vertex in clip space
	gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}



