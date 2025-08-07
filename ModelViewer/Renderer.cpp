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

}
void Renderer::terminate() {
	_shader.terminate();
}

void Renderer::beginFrame() {
	// clear the color buffer
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

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
    model.use();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}


void Renderer::enableWireframe() {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}