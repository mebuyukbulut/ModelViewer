#version 330 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vVelocity;
layout (location = 2) in float vLifeTime;
layout (location = 3) in float vAge;
layout (location = 4) in float vIsActive;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	

	//gl_Position = projection * view * model * vec4(vPos, 1.0);
	gl_Position = projection * view * model *vec4(vPosition, 1.0);
	//gl_PointSize = 2; // gl_Position.z;    

	float lifeRatio = 1.0 - (vAge / vLifeTime);
	gl_PointSize = mix(5.0, 0.0, 1.0 - lifeRatio); // küçülerek kaybolsun

}