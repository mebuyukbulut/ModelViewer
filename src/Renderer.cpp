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
#include "LightManager.h"


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


void Renderer::shadowPass(const SceneRenderData &renderData) 
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    float near_plane = 1.0f, far_plane = 10.0f;
    glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane); 
    glm::mat4 lightView;

    if(renderData.lights.size()){
        glm::quat rot = renderData.lights[0]->owner->transform->getRotationAsQuat();
        glm::vec3 lightDir =
            glm::normalize(
                rot * glm::vec3(0, 1, 0)
            );

        glm::vec3 sceneCenter = {0,0,0};
        glm::vec3 lightPos = sceneCenter - lightDir * 5.0f;

        lightView =
            glm::lookAt(
                lightPos,
                sceneCenter,
                glm::vec3(0, 1,0)
            );
    }
    else{
        lightView = glm::lookAt(glm::vec3(-2.0f, 4.0f, -1.0f), 
                                glm::vec3( 0.0f, 0.0f,  0.0f), 
                                glm::vec3( 0.0f, 1.0f,  0.0f));  
    }
    glm::mat4 lightSpaceMatrix = lightProjection * lightView; 

    _materialShader->use(); _materialShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
    _shadowShader->use();
    _shadowShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
    for (const RenderItem& item : renderData.renderItems) {
        drawModelWithShader(item.model, item.transform, _shadowShader);
    }
}

void Renderer::materialPass(const std::vector<RenderItem> &renderItems)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    _materialShader->use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _shadowMapTarget.depthBuffer());
    _materialShader->use();
    _materialShader->setInt("shadowMap", 0);

    for (const RenderItem& item : renderItems) 
        drawModelWithShader(item.model, item.transform, _materialShader);
}

void Renderer::matcapPass(const std::vector<RenderItem>& renderItems)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glActiveTexture(GL_TEXTURE2);
    matcapTexture->use();

    _matcapShader->use();

    for (const RenderItem& item : renderItems) 
        drawModelWithShader(item.model, item.transform, _matcapShader);
}

void Renderer::wireframePass(const std::vector<RenderItem>& renderItems)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDisable(GL_CULL_FACE);
    //glLineWidth(1.5f);

    _wireframeShader->use();
    for (const RenderItem& item : renderItems) 
        drawModelWithShader(item.model, item.transform, _wireframeShader);

    glEnable(GL_CULL_FACE);
}

void Renderer::backgroundPass()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glDisable(GL_DEPTH_TEST);
    _backgroundShader->use();
    _bgModel->draw(_backgroundShader);
    glEnable(GL_DEPTH_TEST);
}

void Renderer::gridPass()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

    glEnable(GL_BLEND);
    glDisable(GL_CULL_FACE);
    _gridShader->use();
    _gridModel->draw(_gridShader);
    glEnable(GL_CULL_FACE);
    glDisable(GL_BLEND);
}

void Renderer::lightPass()
{

}

void Renderer::selectionPass(const std::vector<RenderItem>& renderItems)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glClearColor(0, 0, 0, 1);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	for (const RenderItem& item : renderItems) 
        drawModelWithShader(item.model, item.transform, _selectionShader, item.entityIndex + 1);
}

void Renderer::init(std::shared_ptr<Camera> camera) {
    _frameUniforms.init(); 

    // Biz bu değişkenleri hardcoded yazıyoruz ama bunları dosyadan okumak yada klasör taraması yapmak daha mantıklı olabilir
    // Ayrıca CWD path olayını da daha net bir hale getirmemiz gerekecek

    // Init all engine::shaders 
    std::vector<ShaderSettings> shaders;
    shaders.push_back({"lambertian", 	"../assets/shaders/lambertian.vert", 		"../assets/shaders/lambertian.frag"});
 	shaders.push_back({"normal", 		"../assets/shaders/normal.vert", 			"../assets/shaders/normal.frag"});
 	shaders.push_back({"particle0", 	"../assets/shaders/particle_point.vert", 	"../assets/shaders/particle_point.frag"});
 	shaders.push_back({"blinn-phong",   "../assets/shaders/blinn-phong.vert", 		"../assets/shaders/blinn-phong.frag"});
 	shaders.push_back({"basic", 		"../assets/shaders/basic_lighting.vert", 	"../assets/shaders/basic_lighting.frag"});
 	shaders.push_back({"pbr", 		    "../assets/shaders/PBR0.vert", 				"../assets/shaders/PBR0.frag"});
	shaders.push_back({"matcap", 		"../assets/shaders/matcap.vert", 			"../assets/shaders/matcap.frag"});
	shaders.push_back({"wireframe", 	"../assets/shaders/wireframe.vert", 		"../assets/shaders/wireframe.frag"});
 	shaders.push_back({"bg", 			"../assets/shaders/bg_grad.vert", 			"../assets/shaders/bg_grad.frag"});
 	shaders.push_back({"skybox", 		"../assets/shaders/skybox.vert", 			"../assets/shaders/skybox.frag"});
 	shaders.push_back({"hdr2cubemap",   "../assets/shaders/hdr2cubemap.vert", 		"../assets/shaders/hdr2cubemap.frag"});
 	shaders.push_back({"grid", 		    "../assets/shaders/gridShader.vert", 		"../assets/shaders/gridShader.frag"});
 	shaders.push_back({"selection", 	"../assets/shaders/selection.vert", 		"../assets/shaders/selection.frag"});
 	shaders.push_back({"shadow",    	"../assets/shaders/simpleShadow.vert", 		"../assets/shaders/simpleShadow.frag"});

    for(auto& ss : shaders)
        g_Assets.get<Shader>("engine::shaders::"+ss.name, &ss);
    
    
	// Setup rendering passes
    _shadowShader = g_Assets.get<Shader>("engine::shaders::shadow").get();
    _materialShader = g_Assets.get<Shader>("engine::shaders::pbr").get();
	_matcapShader = g_Assets.get<Shader>("engine::shaders::matcap").get();
    _wireframeShader = g_Assets.get<Shader>("engine::shaders::wireframe").get();
	_backgroundShader = g_Assets.get<Shader>("engine::shaders::bg").get();
	_gridShader = g_Assets.get<Shader>("engine::shaders::grid").get();
    //void Renderer::setupLightPass() //ışık ögelerinin sahnede çizimi için olabilir. Şimdilik kullanmıyoruz.
	_selectionShader = g_Assets.get<Shader>("engine::shaders::selection").get();



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
    


    _bgModel = g_Assets.get<Model>("engine::models::bgPlane").get();
    _gridModel = g_Assets.get<Model>("engine::models::gridPlane").get();


    //glDisable(GL_FRAMEBUFFER_SRGB);
    //GLboolean srgbEnabled = glIsEnabled(GL_FRAMEBUFFER_SRGB);
    //std::cout << "Framebuffer sRGB: " << (srgbEnabled ? "ENABLED" : "DISABLED") << std::endl;

	initMatcap();

    setCamera(camera);

    setViewMode(ViewMode::Material);


    _rt.create(300,300); // create default frame buffer for viewport
    _shadowMapTarget.create(1024,1024);

}
void Renderer::terminate() {
    //_shaderManager.terminate(); 
}

void Renderer::renderScene(const SceneRenderData &renderData, bool isViewportSelect, glm::vec2 mousePos)
{
    _shadowMapTarget.bind();
    clearBuffer();
    shadowPass(renderData);
    _shadowMapTarget.unbind();




    _frameUniforms.update(_camera->getViewMatrix(), _camera->getProjectionMatrix(), _camera->getPosition());
    _rt.bind();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);    
    clearBuffer();

    if(isViewportSelect){
        selectionPass(renderData.renderItems);
        
        unsigned char data[4];
        glReadPixels(mousePos.x, mousePos.y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
        uint32_t pickedID = data[0] + (data[1] << 8) + (data[2] << 16);
        //LOG_TRACE(std::to_string(pickedID));
        lastSelectedID = pickedID;
    }   
    clearBuffer();

    backgroundPass();
    if      (_viewMode == ViewMode::Material)   materialPass(renderData.renderItems);
    else if (_viewMode == ViewMode::Matcap)     matcapPass(renderData.renderItems);
    else if (_viewMode == ViewMode::Wireframe)  wireframePass(renderData.renderItems);
    gridPass();

    _rt.unbind();

}

void Renderer::clearBuffer(){ glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

void Renderer::resizeViewport(int width, int height)
{
    bool isResized = _rt.resize(width, height); 

    if(isResized)
        _camera->setWindowSize(width,height);
}

void Renderer::drawModelWithShader(Model* model, const glm::mat4& transform, Shader* shader, uint32_t ID){

    shader->use();
    shader->setMat4("model", transform);
    if(ID)
        shader->setInt("objectID", ID);

    model->draw(shader);
}


uint32_t Renderer::getSelection(glm::vec2 mousePos)
{
    // unsigned char data[4];
    // glReadPixels(mousePos.x, mousePos.y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
    // uint32_t pickedID = data[0] + (data[1] << 8) + (data[2] << 16);
    // //LOG_TRACE(std::to_string(pickedID));

    // return pickedID;
    return lastSelectedID;
}

void Renderer::setViewMode(ViewMode mode) {_viewMode = mode; }
ViewMode Renderer::getViewMode()          {return _viewMode; }

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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER); 


    glGenFramebuffers(1, &fbo);
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

GLuint ShadowMapTarget::framebuffer() const { return fbo; }
GLuint ShadowMapTarget::depthBuffer() const { return depthMap; }
