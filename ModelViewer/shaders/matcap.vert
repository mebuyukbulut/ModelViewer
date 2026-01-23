#version 430 core
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoords;

out vec3 fPos;
out vec3 fNormal;
out vec2 fTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{

	fPos = vec3(model * vec4(vPos, 1.0));
	fTexCoords = vTexCoords;

	// Non-uniform scaling sebebiyle bozulan normallerdeki 
	// scaling etkisini geri çeivirmek için : 
	vec3 normalScaled = transpose(inverse(mat3(model))) * vNormal;
	fNormal = mat3(view) * normalScaled; 
	// view space e dönüştürdük çünkü her zaman kameraya doğru baksın istiyoruz. 

	gl_Position = projection * view * model * vec4(vPos, 1.0);
}