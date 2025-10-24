#include "Renderer.h"

#include <glad/glad.h>
#include "Model.h"
#include "Shader.h"
#include "FileUtils.h"
#include "Camera.h"

void Renderer::init() {
	_shaderManager.init(); // load all shaders
	setShader("basic", ShaderType::Main); // set default shader
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
    //glCullFace(GL_FRONT);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); glEnable(GL_BLEND);
    

    //glDisable(GL_FRAMEBUFFER_SRGB);
    //GLboolean srgbEnabled = glIsEnabled(GL_FRAMEBUFFER_SRGB);
    //std::cout << "Framebuffer sRGB: " << (srgbEnabled ? "ENABLED" : "DISABLED") << std::endl;

}
void Renderer::terminate() {
    _shaderManager.terminate(); 
    _shader = nullptr; 
}

void Renderer::beginFrame() {
	// clear the color buffer
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    //static float a = 0.0; 
    //a = a > 1.0f ? 0.0f : a + 0.01f;
    //
    //glClearColor(a,a,a, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set view and projection matrices
    _shader->setMat4("view", _camera->getViewMatrix());
    _shader->setMat4("projection", _camera->getProjectionMatrix());
    _shader->setMat4("model", glm::mat4(1.0f));
    _shader->setVec3("viewPos", _camera->getPosition());
}
void Renderer::endFrame() {
    
}

void Renderer::drawModel(Model* model, const glm::mat4& transform) { 
    //_shader.use();
	_shader->setMat4("model", transform);

    model->draw(*_shader);
}

void Renderer::drawBackground()
{

    glDisable(GL_DEPTH_TEST);
    _shaderManager.getShader("bg").use();
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
    glEnable(GL_DEPTH_TEST);

    _shader->use();
}


void Renderer::setCamera(std::shared_ptr<Camera> camera) { _camera = camera; }
void Renderer::setShader(const std::string name, ShaderType shaderType) {
    Shader* shader{};
    shader = &_shaderManager.getShader(name);
    shader->use();

    switch (shaderType)
    {
    case Renderer::ShaderType::Main:
        _shader = shader;
        break;
    case Renderer::ShaderType::Light:
        _lightShader = shader;
        break;
    case Renderer::ShaderType::Selection:
        _selectShader = shader;
        break;
    default:
        break;
    }

    //_shader->setVec3("lightPos", glm::vec3(3.0f, 3.0f, 0.0f));
    //_shader->setFloat("lightIntensity", 30.0f);
    //_shader->setFloat("albedo", 0.18f);
}

void Renderer::enableWireframe() {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}