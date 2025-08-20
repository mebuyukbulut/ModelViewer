#version 330 core

in vec3 fPos;
in vec3 fNormal;
in vec2 fTexCoords;
out vec4 FragColor;

layout (std140) uniform LightUniforms
{
     vec3 lightPos;
	 vec3 lightColor;
	 float lightIntensity;
};

//uniform vec3 lightPos;
////uniform vec3 lightColor;
//uniform float lightIntensity;
uniform float albedo;

uniform sampler2D texture_diffuse1;


#define _PI 3.1415926535897932384626433832795


void main()
{
	// Calculate the Lambertian reflectance
	vec3 surfaceNormal = normalize(fNormal);
	vec3 lightDirection = normalize(lightPos - fPos);
	float incidentLight = max(dot(surfaceNormal, lightDirection), 0.0) * lightIntensity;
	float reflectedLight =  albedo / _PI * incidentLight;
	vec3 finalColor = vec3(reflectedLight, reflectedLight, reflectedLight); 


	//FragColor = texture(texture_diffuse1, fTexCoords) * vec4(finalColor, 1);
	FragColor = vec4(finalColor, 1);

}