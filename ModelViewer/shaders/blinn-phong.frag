#version 330 core

in vec3 vPos;
in vec3 vNormal;

out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 viewPos;
//uniform vec3 lightColor;
uniform float lightIntensity;
uniform float albedo;

#define _PI 3.1415926535897932384626433832795

void main()
{	
    vec3 lightDir = normalize(lightPos - vPos);
    vec3 viewDir = normalize(viewPos - vPos);

    vec3 halfwayDir = normalize(viewDir + lightDir);  
    float spec = pow(max(dot(vNormal, halfwayDir), 0.0), 256.0);
	vec3 myColor = vec3(spec * lightIntensity/ _PI);



    vec3 surfaceNormal = normalize(vNormal);
	vec3 lightDirection = normalize(lightPos - vPos);
	float incidentLight = max(dot(surfaceNormal, lightDirection), 0.0) * lightIntensity;
	float reflectedLight =  albedo / _PI * incidentLight;
	myColor += vec3(reflectedLight, reflectedLight, reflectedLight); 

    FragColor = vec4(myColor, 1.0f);
}