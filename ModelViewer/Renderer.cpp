#include "Renderer.h"

#include <glad/glad.h>
#include "Model.h"
#include "Shader.h"
#include "FileUtils.h"
#include "Camera.h"
#include "Logger.h"
#include "Texture.h"
#include <filesystem>


void Renderer::initMatcap() {
    //matcapTexture = TextureFactory::load("data/matcaps/basic_1.png", false);
	
    // Scan directory for matcap textures    
    std::string path = "data/matcaps/";
    for (const auto& entry : std::filesystem::directory_iterator(path))
        matcapTexturePaths.push_back(entry.path().string());

    for(const auto& p : matcapTexturePaths)
		LOG_TRACE("Found matcap texture: " + p);

    matcapTexture = TextureFactory::load(matcapTexturePaths.front(), false);
}
void Renderer::init() {
	_shaderManager.init(); // load all shaders
	//setShader("basic", ShaderType::Main); // set default shader
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
    //glCullFace(GL_FRONT);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); glEnable(GL_BLEND);

    

    //// Initialize skybox
    //std::vector<std::string> faces
    //{
    //    "data/skybox/right.jpg",
    //    "data/skybox/left.jpg",
    //    "data/skybox/top.jpg",
    //    "data/skybox/bottom.jpg",
    //    "data/skybox/front.jpg",
    //    "data/skybox/back.jpg"
    //};
    //cubemapTexture = TextureFactory::loadCubeMap(faces);
    ////cubemapTexture = TextureFactory::loadHDR("data/HDRs/citrus_orchard_road_puresky_1k.hdr", &_shaderManager.getShader("hdr2cubemap"));

    // Mesh tmp = MeshFactory::create(DefaultShapes::Cube);
    // _bgMesh = new Mesh(tmp);
    

    // Initialize background
    _bgMesh = new Mesh();
    std::vector<Vertex> bgVertices{
        {{-1,-1, 0}, {0,0,0}, {0,0}},
        {{ 3,-1, 0}, {0,0,0}, {2,0}},
        {{-1, 3, 0}, {0,0,0}, {0,2}},
    };
    std::vector<unsigned int> bgIndices{ 0, 1, 2 };
    _bgMesh->init(bgVertices, bgIndices);
    _bgMesh->upload2GPU();



    // Initialize grid 
    _gridMesh = new Mesh();
    std::vector<Vertex> gridVertices{
        {{ -10, 0, -10}, {0,0,0}, {0,0}}, // a 0
        {{ -10, 0,  10}, {0,0,0}, {0,0}}, // b 1    d-c
        {{  10, 0,  10}, {0,0,0}, {0,0}}, // c 2    a-b
        {{  10, 0, -10}, {0,0,0}, {0,0}}, // d 3
    };
    std::vector<unsigned int> gridIndices{ 0, 1, 2, 0, 2, 3 };
    _gridMesh->init(gridVertices, gridIndices);
    _gridMesh->upload2GPU();
    //glDisable(GL_FRAMEBUFFER_SRGB);
    //GLboolean srgbEnabled = glIsEnabled(GL_FRAMEBUFFER_SRGB);
    //std::cout << "Framebuffer sRGB: " << (srgbEnabled ? "ENABLED" : "DISABLED") << std::endl;

	initMatcap();
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

    glm::mat4 viewMatrix = _camera->getViewMatrix();
    glm::mat4 projMatrix = _camera->getProjectionMatrix();

	// set view and projection matrices
    _shader->use();
    _shader->setMat4("view", viewMatrix);
    _shader->setMat4("projection", projMatrix);
    _shader->setMat4("model", glm::mat4(1.0f));
    _shader->setVec3("viewPos", _camera->getPosition());


    _selectionShader->use();
    _selectionShader->setMat4("view", viewMatrix);
    _selectionShader->setMat4("projection", projMatrix);

    // skybox için
    _bgShader->use(); 
    glm::mat4 viewSkybox = glm::mat4(glm::mat3(viewMatrix));
    _bgShader->setMat4("view", viewSkybox);
    _bgShader->setMat4("projection", projMatrix);


    _gridShader->use();
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); glEnable(GL_BLEND);
    _gridShader->setMat4("view", viewMatrix);
    _gridShader->setMat4("projection", projMatrix);




}
void Renderer::endFrame() {
    
}

void Renderer::drawModel(Model* model, const glm::mat4& transform) {
    _shader->use();
	_shader->setMat4("model", transform);
    model->draw(*_shader);
}

void Renderer::drawModelAsColor(Model* model, const glm::mat4& transform, uint32_t ID)
{
    _selectionShader->use();
    _selectionShader->setMat4("model", transform);
    _selectionShader->setInt("objectID", ID);

    model->draw(*_selectionShader);
}

void Renderer::drawBackground()
{
    glDisable(GL_DEPTH_TEST);
    _bgShader->use();

    _bgMesh->draw(*_bgShader);
    //_shaderManager.getShader("bg").use();
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
    glEnable(GL_DEPTH_TEST);
    //_shader->use();



    //glDepthMask(GL_FALSE);
    //glCullFace(GL_FRONT);
    //
    //_bgShader->use();
    //_bgMesh->draw(*_bgShader);
    //cubemapTexture->use();
    //
    //glCullFace(GL_BACK);
    //glDepthMask(GL_TRUE);




}

void Renderer::drawGrid()
{
    //glDepthMask(GL_FALSE);
    _gridShader->use();
    _gridMesh->draw(*_gridShader);
    glDisable(GL_CULL_FACE);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    glEnable(GL_CULL_FACE);
    //glDepthMask(GL_TRUE);
}

uint32_t Renderer::getSelection(glm::vec2 mousePos)
{
    unsigned char data[4];
    glReadPixels(mousePos.x, mousePos.y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
    uint32_t pickedID = data[0] + (data[1] << 8) + (data[2] << 16);
    //LOG_TRACE(std::to_string(pickedID));

    return pickedID;
}

void Renderer::setViewMode(ViewMode mode)
{

    switch (mode)
    {   
    case ViewMode::Wireframe:
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		_viewMode = ViewMode::Wireframe;
        break;

    case ViewMode::Matcap:
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        _viewMode = ViewMode::Matcap;
		_shader = _matcapShader;

        matcapTexture->_type = GL_TEXTURE_2D;
        glActiveTexture(GL_TEXTURE2);
        matcapTexture->use();

        break;

    case ViewMode::Material:
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        _viewMode = ViewMode::Material;
        _shader = _materialShader;
        break;

    default:
        break;
    }
}

ViewMode Renderer::getViewMode(){
    return _viewMode;
}


void Renderer::setCamera(std::shared_ptr<Camera> camera) { _camera = camera; }
void Renderer::setShader(const std::string name, ShaderType shaderType) {
    Shader* shader{};
    shader = &_shaderManager.getShader(name);
    shader->use();

    switch (shaderType)
    {
    case Renderer::ShaderType::Main:
        LOG_ERROR("Wrong call");
        assert(true);

        _shader = shader;
        break;
    case Renderer::ShaderType::Material:
        _materialShader = shader;
        break;
    case Renderer::ShaderType::Background:
        _bgShader = shader;
        break;
    case Renderer::ShaderType::Grid:
        _gridShader = shader;
        break;
    case Renderer::ShaderType::Light:
        _lightShader = shader;
        break;
    case Renderer::ShaderType::Selection:
        _selectionShader = shader;
        break;
    case Renderer::ShaderType::Matcap:
        _matcapShader = shader;
        break;
    default:
        break;
    }
}

//void Renderer::enableWireframe() {
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//}