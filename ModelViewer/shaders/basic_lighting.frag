#version 330 core

in vec3 fPos;
in vec3 fNormal;
in vec2 fTexCoords;
out vec4 FragColor;

uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float lightIntensity;
uniform float albedo;
uniform float ambientIntensity;
uniform vec3 ambientColor;
uniform vec3 objectColor;
uniform float specularIntensity;

uniform sampler2D texture_diffuse1;


#define _PI 3.1415926535897932384626433832795


void main()
{
	// Calculate normals
	vec3 surfaceNormal = normalize(fNormal);
	vec3 lightDir = normalize(lightPos - fPos);
	vec3 viewDir = normalize(viewPos - fPos);
	
	// ambient 
	vec3 ambient = ambientIntensity * ambientColor; 

	// diffuse 
	float diff = max(dot(surfaceNormal, lightDir), 0.0);
	vec3 diffuse = (1.0f - specularIntensity) * diff * lightIntensity * lightColor; 
	
    // specular
    vec3 reflectDir = reflect(-lightDir, surfaceNormal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularIntensity * spec * lightIntensity * lightColor;  
        
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}	

//FragColor = texture(texture_diffuse1, fTexCoords) * vec4(finalColor, 1);
//FragColor = vec4(finalColor, 1);