#include "Renderer.h"

#include <glad/glad.h>
#include "Model.h"
#include "Shader.h"
#include "FileUtils.h"
#include "Camera.h"

void Renderer::init() {
     
    std::string vertexShaderSource = FileUtils::readFile("shaders/basic.vert");
    std::string fragmentShaderSource = FileUtils::readFile("shaders/basic.frag");
    _shader.init(vertexShaderSource, fragmentShaderSource);
    _shader.use();

	_shader.setVec3("lightPos", glm::vec3(3.0f, 3.0f, 0.0f));
    _shader.setFloat("lightIntensity", 10.0f);
    _shader.setFloat("albedo", 0.18f);

    // uniform vec3 lightPos;
    // uniform vec3 lightColor;
    // uniform float lightIntensity;
    // uniform float albedo;

	glEnable(GL_DEPTH_TEST);

}
void Renderer::terminate() {
	_shader.terminate();
}

void Renderer::beginFrame() {
	// clear the color buffer
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set view and projection matrices
    _shader.setMat4("view", _camera->getViewMatrix());
    _shader.setMat4("projection", _camera->getProjectionMatrix());
    
}
void Renderer::endFrame() {
    
}

void Renderer::drawModel(Model& model) {
    // draw our first triangle
    //_shader.use();
	_shader.setMat4("model", glm::mat4(1.0f)); // identity matrix for model transformation

    model.draw();
}


void Renderer::enableWireframe() {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}