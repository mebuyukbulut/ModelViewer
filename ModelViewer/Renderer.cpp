#include "Renderer.h"

#include <glad/glad.h>
#include "Model.h"

void Renderer::init() {}
void Renderer::terminate() {}

void Renderer::beginFrame() {
    // render
    // ------
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}
void Renderer::endFrame() {
    
}

void Renderer::drawModel(Model& model) {
    // draw our first triangle
    _shader.useShader();
    model.use();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}


void Renderer::enableWireframe() {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}