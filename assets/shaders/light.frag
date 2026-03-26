#version 430 core

in vec3 fPos;
in vec3 fNormal;
in vec2 fTexCoords;
out vec4 FragColor;

uniform vec3 lightColor;


void main(){
    //FragColor = vec4(0.38, 0.42, 0.18 ,1.0);
    FragColor = vec4(lightColor, 1.0);
}	
