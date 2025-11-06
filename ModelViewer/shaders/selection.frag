#version 330 core

uniform int objectID;

void main(){
    //int a = 120;
    gl_FragColor = vec4(
        float((objectID & 0x000000FF)) / 255.0,
        float((objectID & 0x0000FF00) >> 8) / 255.0,
        float((objectID & 0x00FF0000) >> 16) / 255.0,
        1.0
    );
    //gl_FragColor = vec4(1,1,1,1.0);

}	