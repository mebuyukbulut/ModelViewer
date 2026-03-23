#include "Renderer.h"

#include <glad/gl.h>
#include "Model.h"
#include "Shader.h"
#include "FileUtils.h"
#include "Camera.h"
#include "Logger.h"
#include "Texture.h"
#include <filesystem>
#include "AssetManager.h"
#include "RenderComponent.h"
#include "Transform.h"

void Renderer::initMatcap() {
    //matcapTexture = TextureFactory::load("data/matcaps/basic_1.png", false);
	
    // Scan directory for matcap textures    
    std::string path = "../assets/env/matcaps/";
    for (const auto& entry : std::filesystem::directory_iterator(path))
        matcapTexturePaths.push_back(entry.path().string());

    for(const auto& p : matcapTexturePaths)
		LOG_TRACE("Found matcap texture: " + p);

    //matcapTexture = TextureFactory::load(matcapTexturePaths.front(), false);
    matcapTexture = g_Assets.get<Texture>( matcapTexturePaths.front());
}


void Renderer::setupMaterialPass(){
    _materialShader = g_Assets.get<Shader>("engine::shaders::pbr").get();
}
void Renderer::setupMatcapPass(){
	_matcapShader = g_Assets.get<Shader>("engine::shaders::matcap").get();
}
void Renderer::setupWireframePass()
{
}
void Renderer::setupBackgroundPass(){
	_backgroundShader = g_Assets.get<Shader>("engine::shaders::bg").get();
}
void Renderer::setupGridPass(){
	_gridShader = g_Assets.get<Shader>("engine::shaders::grid").get();
}
void Renderer::setupLightPass(){
	//ışık ögelerinin sahnede çizimi için olabilir. Şimdilik kullanmıyoruz.
}
void Renderer::setupSelectionPass(){
	_selectionShader = g_Assets.get<Shader>("engine::shaders::selection").get();
}

void Renderer::materialPass(const std::vector<RenderItem>& renderItems)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    _materialShader->use();
    // //_materialShader->_type = Shader::Type::Foreground;
    // for (const RenderItem& item : renderItems) {
    //     _materialShader->setMat4("model", item.transform);    
    //     item.model->draw(_materialShader);
    // }


    for (const RenderItem& item : renderItems) {
        drawModelAsMaterial(item.model, item.transform);
    }
}

void Renderer::matcapPass(const std::vector<RenderItem>& renderItems)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glActiveTexture(GL_TEXTURE2);
    matcapTexture->use();

    _matcapShader->use();
    for (const RenderItem& item : renderItems) {
        _matcapShader->setMat4("model", item.transform);
        item.model->draw(_matcapShader);
    }
}

void Renderer::wireframePass(const std::vector<RenderItem>& renderItems)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glActiveTexture(GL_TEXTURE2);
    matcapTexture->use();

    _matcapShader->use();
    for (const RenderItem& item : renderItems) {
        _matcapShader->setMat4("model", item.transform);
        item.model->draw(_matcapShader);
    }
}

void Renderer::backgroundPass()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glDisable(GL_DEPTH_TEST);
    _backgroundShader->use();
    _bgMesh->draw(_backgroundShader);

    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
    glEnable(GL_DEPTH_TEST);
}

void Renderer::gridPass()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    _gridShader->use();
    _gridMesh->draw(_gridShader);
    glDisable(GL_CULL_FACE);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    glEnable(GL_CULL_FACE);
}

void Renderer::lightPass()
{

}

void Renderer::selectionPass(const std::vector<RenderItem>& renderItems)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glClearColor(0, 0, 0, 1);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	for (const RenderItem& item : renderItems) {
        drawModelAsColor(item.model, item.transform, item.entityIndex + 1);
    }
    // // draw all entities with unique color
    // for (uint32_t pickID = 0; pickID < entities.size(); pickID++) {
    //     Entity* entity = entities[pickID].get();
    //     if (!entity) continue;

    //     if (RenderComponent* renderComponent = entity->getComponent<RenderComponent>())
    //         drawModelAsColor(
    //             renderComponent->_model.get(),
    //             entity->transform->getGlobalMatrix(),
    //             pickID + 1);

    // }
}

void Renderer::init(std::shared_ptr<Camera> camera) {
    // Init all engine::shaders 
    std::vector<ShaderSettings> shaders;
    shaders.push_back({"lambertian", 	"../assets/shaders/lambertian.vert", 		"../assets/shaders/lambertian.frag"});
 	shaders.push_back({"normal", 		"../assets/shaders/normal.vert", 			"../assets/shaders/normal.frag"});
 	shaders.push_back({"particle0", 	"../assets/shaders/particle_point.vert", 	"../assets/shaders/particle_point.frag"});
 	shaders.push_back({"blinn-phong",   "../assets/shaders/blinn-phong.vert", 		"../assets/shaders/blinn-phong.frag"});
 	shaders.push_back({"basic", 		"../assets/shaders/basic_lighting.vert", 	"../assets/shaders/basic_lighting.frag"});
 	shaders.push_back({"pbr", 		    "../assets/shaders/PBR0.vert", 				"../assets/shaders/PBR0.frag"});
	shaders.push_back({"matcap", 		"../assets/shaders/matcap.vert", 			"../assets/shaders/matcap.frag"});
 	shaders.push_back({"bg", 			"../assets/shaders/bg_grad.vert", 			"../assets/shaders/bg_grad.frag"});
 	shaders.push_back({"skybox", 		"../assets/shaders/skybox.vert", 			"../assets/shaders/skybox.frag"});
 	shaders.push_back({"hdr2cubemap",   "../assets/shaders/hdr2cubemap.vert", 		"../assets/shaders/hdr2cubemap.frag"});
 	shaders.push_back({"grid", 		    "../assets/shaders/gridShader.vert", 		"../assets/shaders/gridShader.frag"});
 	shaders.push_back({"selection", 	"../assets/shaders/selection.vert", 		"../assets/shaders/selection.frag"});

    for(auto& ss : shaders)
        g_Assets.get<Shader>("engine::shaders::"+ss.name, &ss);
    
    
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

    setCamera(camera);

	// Setup rendering passes
    setupMaterialPass();
	setupMatcapPass();
	setupBackgroundPass();
	setupGridPass();
    setupSelectionPass();

    //setShader("PBR0", Renderer::ShaderType::Material);
    //setShader("matcap", Renderer::ShaderType::Matcap);

    //setShader("bg", Renderer::ShaderType::Background);
    //setShader("grid", Renderer::ShaderType::Grid);
    //setShader("selection", Renderer::ShaderType::Selection);

    setViewMode(ViewMode::Material);


    _rt.create(300,300); // create default frame buffer for viewport

}
void Renderer::terminate() {
    //_shaderManager.terminate(); 
}

void Renderer::beginFrame() {
	// clear the color buffer
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 viewMatrix = _camera->getViewMatrix();
    glm::mat4 projMatrix = _camera->getProjectionMatrix();

    // TO-DO: tüm shaderlarda ortak olan uniformları UBO veya benzeri bir yapı ile yönetmek daha kolay olabilir. 

	// set view and projection matrices
    _materialShader->use();
    _materialShader->setMat4("view", viewMatrix);
    _materialShader->setMat4("projection", projMatrix);
    _materialShader->setMat4("model", glm::mat4(1.0f));
    _materialShader->setVec3("viewPos", _camera->getPosition());

    _matcapShader->use();
    _matcapShader->setMat4("view", viewMatrix);
    _matcapShader->setMat4("projection", projMatrix);
    _matcapShader->setMat4("model", glm::mat4(1.0f));
    _matcapShader->setVec3("viewPos", _camera->getPosition());


    _selectionShader->use();
    _selectionShader->setMat4("view", viewMatrix);
    _selectionShader->setMat4("projection", projMatrix);

    // skybox için
    _backgroundShader->use(); 
    glm::mat4 viewSkybox = glm::mat4(glm::mat3(viewMatrix));
    _backgroundShader->setMat4("view", viewSkybox);
    _backgroundShader->setMat4("projection", projMatrix);


    _gridShader->use();
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); glEnable(GL_BLEND);
    _gridShader->setMat4("view", viewMatrix);
    _gridShader->setMat4("projection", projMatrix);

}

void Renderer::endFrame() {
    
}

void Renderer::resizeViewport(int width, int height)
{
    bool isResized = _rt.resize(width, height); 

    if(isResized)
        _camera->setWindowSize(width,height);
}

void Renderer::drawModelAsMaterial(Model *model, const glm::mat4 &transform)
{
    _materialShader->use();
    _materialShader->setMat4("model", transform);
    model->draw(_materialShader);
}

void Renderer::drawModelAsMatcap(Model* model, const glm::mat4& transform) {
    _matcapShader->use();
    _matcapShader->setMat4("model", transform);
    model->draw(_matcapShader);
}

void Renderer::drawModelAsColor(Model* model, const glm::mat4& transform, uint32_t ID)
{
    _selectionShader->use();
    _selectionShader->setMat4("model", transform);
    _selectionShader->setInt("objectID", ID);

    model->draw(_selectionShader);
}


uint32_t Renderer::getSelection(glm::vec2 mousePos)
{
    unsigned char data[4];
    glReadPixels(mousePos.x, mousePos.y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
    uint32_t pickedID = data[0] + (data[1] << 8) + (data[2] << 16);
    //LOG_TRACE(std::to_string(pickedID));

    return pickedID;
}

void Renderer::setViewMode(ViewMode mode){
	_viewMode = mode;
}
ViewMode Renderer::getViewMode(){
    return _viewMode;
}

void Renderer::setCamera(std::shared_ptr<Camera> camera) { _camera = camera; }






void ColorRenderTarget::create(int width, int height)
{
    this->width = width; this->height = height; 

    glGenTextures(1, &colorTex);
    glBindTexture(GL_TEXTURE_2D, colorTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenRenderbuffers(1, &depthRbo);
    glBindRenderbuffer(GL_RENDERBUFFER, depthRbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTex, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthRbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        LOG_ERROR("ColorRenderTarget incomplete!");


    unbind(); // bu burada gereksiz gibi ama bakalım
}

void ColorRenderTarget::destroy()
{    
    glDeleteFramebuffers(1, &fbo);
    glDeleteTextures(1, &colorTex);
    glDeleteRenderbuffers(1, &depthRbo);

}

bool ColorRenderTarget::resize(int width, int height)
{    
    if (width == this->width && height == this->height // if(newSize == oldSize) -> do not create new framebuffer
        || (!width || !height) )// or if(newSize.x == 0 || newSize.y == 0) -> do not create new framebuffer
        return false; 

    // Eski GPU kaynaklarını serbest bırak
    destroy();

    // Yeni boyutla tekrar oluştur
    create(width, height);
    return true; 
}

void ColorRenderTarget::bind(){
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, width, height);
}

void ColorRenderTarget::unbind(){
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint ColorRenderTarget::framebuffer()  const { return fbo; }
GLuint ColorRenderTarget::colorTexture() const { return colorTex; }
GLuint ColorRenderTarget::depthBuffer()  const { return depthRbo; }







// 
void ShadowMapTarget::create(int width, int height)
{
    this->width = width; this->height = height;

    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 
                width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 


    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    
    unbind();
}

void ShadowMapTarget::destroy()
{
    glDeleteFramebuffers(1, &fbo);
    glDeleteTextures(1, &depthMap);
}

// width height !width !height !h&&!w expected !h||!w
// 0       0       1     1       1       1       1
// 0       1       1     0       0       1       1
// 1       0       0     1       0       1       1
// 1       1       0     0       0       0       0

bool ShadowMapTarget::resize(int width, int height)
{
    if (width == this->width && height == this->height // if(newSize == oldSize) -> do not create new framebuffer
        || (!width || !height) )// or if(newSize.x == 0 || newSize.y == 0) -> do not create new framebuffer
        return false; 

    // Eski GPU kaynaklarını serbest bırak
    destroy();

    // Yeni boyutla tekrar oluştur
    create(width, height);
    return true; 
}

void ShadowMapTarget::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, width, height);
}

void ShadowMapTarget::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);  
}
