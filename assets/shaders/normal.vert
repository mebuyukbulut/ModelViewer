#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 ourColor;

void main()
{
	// view-space normal transformation 
	mat3 normalMatrix = transpose(inverse(mat3(view)));
	vec3 normalView = normalize(normalMatrix * aNormal);
	vec3 rgb_normal = normalView * 0.5 + 0.5;

	// Alternatively, if you want to use the original normal directly:
	//vec3 rgb_normal = aNormal * 0.5 + 0.5;
	ourColor = vec3(rgb_normal); // Pass normal to fragment shader

	gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}



