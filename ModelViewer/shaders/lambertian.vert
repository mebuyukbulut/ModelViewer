#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightPos;
//uniform vec3 lightColor;
uniform float lightIntensity;
uniform float albedo;

out vec3 ourColor;
out vec2 texCoords;

#define _PI 3.1415926535897932384626433832795

void main()
{
	texCoords = aTexCoords;

	// Calculate the Lambertian reflectance
	vec3 surfaceNormal = normalize(aNormal);
	vec3 lightDirection = normalize(lightPos - aPos);
	float incidentLight = max(dot(surfaceNormal, lightDirection), 0.0) * lightIntensity;
	float reflectedLight =  albedo / _PI * incidentLight;
	ourColor = vec3(reflectedLight, reflectedLight, reflectedLight); 

	gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}



//	// view-space normal transformation 
//	mat3 normalMatrix = transpose(inverse(mat3(view)));
//	vec3 normalView = normalize(normalMatrix * aNormal);
//	vec3 rgb_normal = normalView * 0.5 + 0.5;
//
//	// Alternatively, if you want to use the original normal directly:
//	//vec3 rgb_normal = aNormal * 0.5 + 0.5;
//	ourColor = vec3(rgb_normal); // Pass normal to fragment shader
