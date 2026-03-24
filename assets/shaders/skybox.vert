#version 430 core
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoords;

out vec3 uvCoords;

layout(std140, binding = 0) uniform FrameUniforms
{
    mat4 view;
    mat4 projection;
    vec3 viewPos;
    float _pad0;
};

void main() {    
    uvCoords = vPos; //vTexCoords;
    gl_Position = projection * mat4(mat3(view)) * vec4(vPos, 1.0);

}

//    // skybox için
//    _backgroundShader->use(); 
//    glm::mat4 viewSkybox = glm::mat4(glm::mat3(viewMatrix));
//    _backgroundShader->setMat4("view", viewSkybox);
//    _backgroundShader->setMat4("projection", projMatrix);