#include "SceneManager.h"

#include <iostream>
#include <fstream>

#include <imgui.h>
#include <ImGuizmo.h>
#include <imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>
#include <yaml-cpp/yaml.h>

#include "Camera.h"
//#include "Entity.h"
#include "Transform.h"
#include "EventDispatcher.h"
#include "Inspectable.h"
#include "LightManager.h"
//#include "Material.h"
#include "Renderer.h"
//#include "Scene.h"
#include "Shader.h"
#include "UIManager.h"

#include "Logger.h"


ImGuizmo::OPERATION mCurrentGizmoOperation = ImGuizmo::TRANSLATE;


void SceneManager::CreateRenderTarget(RenderTarget& rt, int width, int height) 
{
    rt.width = width;
    rt.height = height;

    glGenTextures(1, &rt.colorTex);
    glBindTexture(GL_TEXTURE_2D, rt.colorTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenRenderbuffers(1, &rt.depthRbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rt.depthRbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

    glGenFramebuffers(1, &rt.fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, rt.fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, rt.colorTex, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rt.depthRbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        LOG_ERROR("RenderTarget incomplete!");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void SceneManager::ResizeRenderTarget(int newWidth, int newHeight)
{
    if (newWidth == _rt.width && newHeight == _rt.height)
        return; // boyut değişmemiş

    _rt.width = newWidth;
    _rt.height = newHeight;

    if (_rt.width && _rt.height)
        _camera->setWindowSize(_rt.width, _rt.height);

    // Eski GPU kaynaklarını serbest bırak
    glDeleteFramebuffers(1, &_rt.fbo);
    glDeleteTextures(1, &_rt.colorTex);
    glDeleteRenderbuffers(1, &_rt.depthRbo);

    // Yeni boyutla tekrar oluştur
    CreateRenderTarget(_rt, newWidth, newHeight);
}

void SceneManager::init(Renderer* renderer, Camera* camera, Shader* shader, UIManager* UI) {
    _renderer = renderer;
    _camera = camera;
    _UI = UI;

	_lightMng = std::make_unique<LightManager>();
    _materialMng.reset(new MaterialManager(shader));
    

    dispatcher.subscribe(EventType::AddPointLight, [&](const Event& e) {
        addLight(LightType::Point);
        });
    dispatcher.subscribe(EventType::AddSpotLight, [&](const Event& e) {
        addLight(LightType::Spot);
        });
    dispatcher.subscribe(EventType::AddDirectionalLight, [&](const Event& e) {
        addLight(LightType::Directional);
        });


    dispatcher.subscribe(EventType::AddCube, [&](const Event& e) {
        addShape(DefaultShapes::Cube);
        });
    dispatcher.subscribe(EventType::AddCone, [&](const Event& e) {
        addShape(DefaultShapes::Cone);
        });
    dispatcher.subscribe(EventType::AddCylinder, [&](const Event& e) {
        addShape(DefaultShapes::Cylinder);
        });
    dispatcher.subscribe(EventType::AddPlane, [&](const Event& e) {
        addShape(DefaultShapes::Plane);
        });
    dispatcher.subscribe(EventType::AddTorus, [&](const Event& e) {
        addShape(DefaultShapes::Torus);
        });
    dispatcher.subscribe(EventType::AddMonkey, [&](const Event& e) {
        addMonkey();
        });

    dispatcher.subscribe(EventType::Delete, [&](const Event& e) {
        deleteSelected();
        });
    dispatcher.subscribe(EventType::Select, [&](const Event& e) {
        mousePos = glm::vec2(e.data.vec.x, e.data.vec.y);
        isViewportSelect = true;
        });

    dispatcher.subscribe(EventType::ScenePopup, [&](const Event& e) {
        isScenePopupOpen = true;
        });



    dispatcher.subscribe(EventType::ModelOpened, [&](const Event& e) {
        std::string modelPath = e.data.text;

        if (modelPath.empty())
            return;

        Model *model = new Model(_materialMng.get());
        if (model->loadFromFile(modelPath)) {
            //_models.push_back(model);
            //std::wcout << L"Success to load model: " << FileUtils::UTF8ToWString(modelPath) << std::endl;
            LOG_INFO("Success to load model: " + modelPath);
        }
        else {
            LOG_ERROR("Failed to load model: " + modelPath);
        }

        //The position of the last character that matches.
        //If no matches are found, the function returns string::npos.
        //
        //size_t is an unsigned integral type(the same as member type string::size_type).
        
        unsigned int slashIndex = modelPath.find_last_of('\\');
        unsigned int pointIndex = modelPath.find_last_of('.');
        std::string directory = modelPath.substr(0, slashIndex);
        std::string modelName = modelPath.substr(slashIndex+1, pointIndex-slashIndex -1);

		// Yeni 
        Entity* entity = new Entity;
		entity->addComponent(std::unique_ptr<Model>(model));
		entity->name = getUniqueName(modelName);
		_entities.emplace_back(entity);

        //// Eski
        //Transform* transform = new Transform;
        //transform->setEntity(new Entity);
        //transform->getEntity()->model.reset(model);
        //transform->name = getUniqueName(modelName);
        //_transforms.emplace_back(transform);  

        LOG_TRACE(modelName);
        });


    CreateRenderTarget(_rt, 300, 300);

	dispatcher.dispatch(Event{ EventType::AddTorus, EventData{} });
	_entities[0]->transform->setPosition(glm::vec3(5, 0, 0));
    dispatcher.dispatch(Event{ EventType::AddTorus, EventData{} });
	_entities[0]->transform->setParent(_entities[1]->transform.get());

}
void SceneManager::draw() {
    // FBO’ya çiz
    glBindFramebuffer(GL_FRAMEBUFFER, _rt.fbo);
    glViewport(0, 0, _rt.width, _rt.height);
    //glClearColor(1, 0, 0, 1); // error check


    // mouse click ile ekranda öge yakalama
    if (isViewportSelect) {
        glClearColor(0, 0, 0, 1);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        

        for (const auto& entity : _entities) {
            if (entity->transform->isRoot() && entity->getComponent<Model>())
                drawAsColorRecursive(entity.get());
        }

        glm::vec2 mPos = glm::vec2(mousePos.x, mousePos.y);
        glm::vec2 panelPos = glm::vec2(viewportPos.x, viewportPos.y);
        glm::vec2 panelSize = glm::vec2(viewportPanelSize.x, viewportPanelSize.y);
        mPos = mPos - panelPos;
        mPos.y = panelSize.y - mPos.y;
        uint32_t selectedID = _renderer->getSelection(mPos);
        LOG_TRACE("selection UUID: " + std::to_string(_renderer->getSelection(mPos)));
        //LOG_TRACE(std::to_string(mPos.x) + " " + std::to_string(mPos.y));

        if (selectedID != 0)
            for (const auto& entity : _entities) {
                
                if (entity->transform->UUID == selectedID) {
                    if (!ImGui::GetIO().KeyCtrl)
                        deselectAll();
                    select(entity.get());
                }
            }
        else
            deselectAll();

        isViewportSelect = false;
    }

    
    
    
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    _renderer->drawBackground();



    for (const auto& entity : _entities) {
        if (entity->transform->isRoot() && entity->getComponent<Model>())
            drawRecursive(entity.get()); 
    }

    _renderer->drawGrid();
    _renderer->getShader().use();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// draw light? 
void SceneManager::drawRecursive(Entity* entity)
{
    auto model = entity->getComponent<Model>();
    if(model)
        _renderer->drawModel(model, entity->transform->getGlobalMatrix());

    for (Transform* i : entity->transform->getChildren())
        drawRecursive(i->owner);
}

void SceneManager::drawAsColorRecursive(Entity* entity)
{
    auto model = entity->getComponent<Model>();
    if (model)
        _renderer->drawModelAsColor(model, entity->transform->getGlobalMatrix(), entity->transform->UUID);

    for (Transform* i : entity->transform->getChildren())
        drawAsColorRecursive(i->owner);
}







bool SceneManager::isSelected(Entity* entity){
    return std::find(_selectedEntities.begin(), _selectedEntities.end(), entity) != _selectedEntities.end();
}

bool SceneManager::isLastSelected(Entity* entity){
    return entity == _selectedEntity;
}

void SceneManager::select(Entity* entity){
    if (!isSelected(entity))
        _selectedEntities.push_back(entity);

    _selectedEntity = entity;
}

void SceneManager::deselect(Entity* entity)
{
    if (!isSelected(entity))
        return; 
    _selectedEntities.erase(std::find(_selectedEntities.begin(), _selectedEntities.end(), entity));
    _selectedEntity = nullptr;

    if (!_selectedEntities.empty())
        _selectedEntity = _selectedEntities.back();

}

void SceneManager::deselectAll(){
    _selectedEntities.clear();
    _selectedEntity = nullptr;
}

void SceneManager::loadScene(std::string path) {
    LOG_ERROR("TO-DO: Load Scene");

    ////YAML::Node root = YAML::LoadFile(path);
    //
    ////for (const auto& lightNode : root["Lights"]) {
    ////    auto light = LightFactory::create(lightNode);
    //
    ////    if (light.get()) {
    //
    ////        LOG_TRACE("Load light");
    ////        addLight(std::move(light));
    ////    }
    ////}
    //
    //
    //YAML::Node rootTest = YAML::LoadFile("save0.yaml");
    //for (const auto& transformNode : rootTest) {
    //    auto transform = TransformFactory::create(transformNode, _materialMng.get());
    //
    //    if (transform.get()) {
    //
    //        LOG_TRACE("Load transform...");
    //        _transforms.emplace_back(std::move(transform));
    //        //addLight(std::move(transform));
    //    }
    //}


}
void SceneManager::saveScene() {
    LOG_ERROR("TO-DO: Save Scene");
    //YAML::Node node;
    //for (const auto& entity : _entities) {
    //    node.push_back(entity->serialize());
    //    //Light* light = transform->getEntity()->light.get();
    //    //if(light)
    //    //    node["Lights"].push_back(light->serialize());
    //}
    //std::string path = "save0.yaml";
    //std::ofstream fout(path);
    //fout << node;
}

/// Call recursively to populate each level of children
void SceneManager::drawHierarchyTreeRecursive(Entity* entity) {
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
    if (isSelected(entity)) flags |= ImGuiTreeNodeFlags_Selected;
    if (entity->transform->getChildren().empty()) flags |= ImGuiTreeNodeFlags_Leaf;

    bool opened = false;

    if (isLastSelected(entity)) {
        //ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(230, 125, 15, 255)); // orange foreground
        opened = ImGui::TreeNodeEx(entity->name.c_str(), flags);
        //ImGui::PopStyleColor(1);
    }
    else {
        opened = ImGui::TreeNodeEx(entity->name.c_str(), flags);
    }


    // Seçme işlemi
    if (ImGui::IsItemClicked()){
        if (!ImGui::GetIO().KeyCtrl)
            deselectAll();
        
        select(entity);
    }


    // --- BURASI: DRAG SOURCE (Sürüklemeyi Başlat) ---
    if (ImGui::BeginDragDropSource()) {
        // Sürüklenen nesnenin pointer'ını paketle (Payload)
        ImGui::SetDragDropPayload("ENTITY_HIERARCHY_NODE", &entity, sizeof(Entity*));

        // Sürüklerken farenin yanında ne görünsün?
        ImGui::Text("Moving: %s", entity->name.c_str());
        ImGui::EndDragDropSource();
    }

    // --- BURASI: DRAG TARGET (Üzerine Bırakmayı Kabul Et) ---
    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY_HIERARCHY_NODE")) {
            // Paketi aç (Sürüklenen Entity'yi al)
            Entity* draggedEntity = *(Entity**)payload->Data;

            // Kendi kendine veya zaten parent'ı olan birine bırakılmadığından emin ol
            if (draggedEntity != entity) {
                // MATEMATİKSEL PARENT ATAMA (Zıplamayı engelleyen fonksiyon)
                draggedEntity->transform->setParent(entity->transform.get());
            }
        }
        ImGui::EndDragDropTarget();
    }

    // Recursive çağrı
    if (opened) {
        for (Transform* transform : entity->transform->getChildren())
            drawHierarchyTreeRecursive(transform->owner);
    
        ImGui::TreePop();
    }
}

void SceneManager::onInspect()
{
    // HIERARCHY PANEL

    ImGuiTreeNodeFlags rootFlag =
        ImGuiTreeNodeFlags_DefaultOpen |
        ImGuiTreeNodeFlags_Bullet |
        ImGuiTreeNodeFlags_Leaf;

    ImGui::Begin("Scene");

    if (ImGui::IsItemClicked())
        LOG_TRACE("Scene window tab was clicked");


    if (ImGui::TreeNodeEx("root", rootFlag)){
        if (ImGui::IsItemClicked())
            LOG_TRACE("Scene root was clicked");
        
        ImGui::PushStyleColor(ImGuiCol_Header, IM_COL32(55, 55, 55, 255)); // gray background
        
        for (const auto& i : _entities) 
            if(i->transform->isRoot())
                drawHierarchyTreeRecursive(i.get()); // Call recursively to populate each level of children

        ImGui::PopStyleColor(1);

        ImGui::TreePop();  // This is required at the end of the if block
    }

    // drawHierarchy Tree döngüsünün dışına, Begin/End arasına:
    ImGui::Dummy(ImGui::GetContentRegionAvail()); // Boş bir alan yarat
    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY_HIERARCHY_NODE")) {
            Entity* draggedEntity = *(Entity**)payload->Data;
            draggedEntity->transform->setParent(nullptr); // Bağımsız yap
        }
        ImGui::EndDragDropTarget();
    }


    // bos alana tiklamayi yakala:
    if (ImGui::IsWindowFocused() &&
        ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem) &&
        ImGui::IsMouseClicked(ImGuiMouseButton_Left) &&
        !ImGui::IsAnyItemHovered()
        )
    {
        deselectAll();        
    }


    ImGui::End();



    // PROPERTIES PANEL

    ImGui::Begin("Properties");
    if(_selectedEntity)
		_selectedEntity->onInspect();
    ImGui::End();



    // VIEWPORT PANEL

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("Viewport");// &viewport->get_active()); //ImGuiWindowFlags_NoTitleBar| ImGuiWindowFlags_UnsavedDocument
    _UI->setHoverOnUI(ImGui::IsWindowHovered());
    //viewport->set_hovered(ImGui::IsWindowHovered());
    ////std::cout << ImGui::IsWindowHovered() << std::endl;
    

    //ImGui::OpenPopupOnItemClick("SceneContextMenu", ImGuiPopupFlags_None);
    
    if (isScenePopupOpen) {
        isScenePopupOpen = false;
        ImGui::OpenPopup("SceneContextMenu");
    }

    if (ImGui::BeginPopupContextWindow("SceneContextMenu",2)) // sağ tıkla pencere boşluğuna tıklanırsa
    {
        if (ImGui::BeginMenu("Add Light")) {
            if (ImGui::MenuItem("Add Point Light")) {
                Event e{ EventType::AddPointLight };
                dispatcher.dispatch(e);
            }
            if (ImGui::MenuItem("Add Spot Light")) {
                Event e{ EventType::AddSpotLight };
                dispatcher.dispatch(e);
            }
            if (ImGui::MenuItem("Add Direction Light")) {
                Event e{ EventType::AddDirectionalLight };
                dispatcher.dispatch(e);
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Add Shape")) {
            if (ImGui::MenuItem("Add Cube")) {
                Event e{ EventType::AddCube };
                dispatcher.dispatch(e);
            }
            if (ImGui::MenuItem("Add Cone")) {
                Event e{ EventType::AddCone };
                dispatcher.dispatch(e);
            }
            if (ImGui::MenuItem("Add Cylinder")) {
                Event e{ EventType::AddCylinder };
                dispatcher.dispatch(e);
            }
            if (ImGui::MenuItem("Add Plane")) {
                Event e{ EventType::AddPlane };
                dispatcher.dispatch(e);
            }
            if (ImGui::MenuItem("Add Torus")) {
                Event e{ EventType::AddTorus };
                dispatcher.dispatch(e);
            }
            ImGui::EndMenu();

        }


        ImGui::Separator();

        ImGui::EndPopup();
    }



    auto t_size = ImGui::GetContentRegionAvail();//ImGui::GetWindowSize();
    glm::ivec2 window_size{ t_size.x, t_size.y };

    //if (window_size != viewport->get_resolution()) {
    //    std::string res = "(" + std::to_string(window_size.x) + ", " + std::to_string(window_size.y) + ")";
    //    //LogUtils::get().log("New window size: " + res);
    //    viewport->set_resolution(window_size);
    //    viewport->set_dirty(true);
    //}
     
     
     
    //GLint defaultFBO = 0;
    ////glGetIntegerv(GL_FRAMEBUFFER_BINDING_OES, &defaultFBO);
    //ImGui::Image(
    //    (ImTextureID)defaultFBO,//viewport->texID(),
    //    ImGui::GetContentRegionAvail(),
    //    ImVec2(0, 1),
    //    ImVec2(1, 0)
    //);

    ImVec2 panelSize = ImGui::GetContentRegionAvail();
    viewportPanelSize = glm::vec2(panelSize.x, panelSize.y);
    ImVec2 cursorScreenPos = ImGui::GetCursorScreenPos(); 
    viewportPos = glm::vec2(cursorScreenPos.x, cursorScreenPos.y);
    ImGui::Image((ImTextureID)(intptr_t)_rt.colorTex,
        panelSize, ImVec2(0, 1), ImVec2(1, 0));

    // viewport toolbar BEGIN
    // https://gist.github.com/rmitton/f80cbb028fca4495ab1859a155db4cd8
    float menuBarHeight = 25;
    float toolbarSize = 30;
    ImGuiWindow* viewport = ImGui::GetCurrentWindow();
    //ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x, viewport->Pos.y + menuBarHeight));
    ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, toolbarSize));
    ImGui::SetNextWindowViewport(ImGui::GetMainViewport()->ID);

    ImGuiWindowFlags window_flags = 0
        | ImGuiWindowFlags_NoDocking
        | ImGuiWindowFlags_NoTitleBar
        | ImGuiWindowFlags_NoResize
        | ImGuiWindowFlags_NoMove
        | ImGuiWindowFlags_NoScrollbar
        | ImGuiWindowFlags_NoSavedSettings
        ;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
    ImGui::Begin("TOOLBAR", NULL, window_flags);

    //_GizmoState
    if(ImGui::Button("T", ImVec2(0, 37))) // translate 
        mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
    ImGui::SameLine();
    if (ImGui::Button("R", ImVec2(0, 37))) // rotate
        mCurrentGizmoOperation = ImGuizmo::ROTATE;
    ImGui::SameLine();
    if (ImGui::Button("S", ImVec2(0, 37))) // scale
        mCurrentGizmoOperation = ImGuizmo::SCALE;
    //ImGui::Button("Toolbar goes here", ImVec2(0, 37));

    ImGui::End();
    ImGui::PopStyleVar();


    ResizeRenderTarget(panelSize.x, panelSize.y);
    // viewport toolbar END

    drawGizmo();

    ImGui::End();
    ImGui::PopStyleVar();
}
void SceneManager::drawGizmo()
{
    ImGuiIO& io = ImGui::GetIO();
    ImGuizmo::BeginFrame();
    ImGuizmo::SetDrawlist();


    // Viewport boyutu
    float windowWidth = ImGui::GetWindowSize().x;
    float windowHeight = ImGui::GetWindowSize().y;

    // Küçük bir alan ayır (örneğin 128x128 px)
    float gizmoSize = 100.0f;
    ImVec2 gizmoPos = ImVec2(windowWidth-gizmoSize-1, 80); // sağ üst köşe

    

    // Kamera matrislerini kullan
    const float* view = glm::value_ptr(_camera->getViewMatrix());
    const float* projection = glm::value_ptr(_camera->getProjectionMatrix());


    // ImGuizmo’nun çizim bölgesini ayarla
    ImGuizmo::SetRect(gizmoPos.x, gizmoPos.y, gizmoSize, gizmoSize);
    //ImGuizmo::DrawCubes(view, projection, nullptr, 0);

    //float a = view[0];

    ImGuizmo::ViewManipulate(
        (float*)view,
        10.0f,                // manipülasyon boyutu
        gizmoPos,
        ImVec2(gizmoSize, gizmoSize),
        0x79000000            // opsiyonel arka plan rengi
    );
    //float b = view[0];
    //if(a!= b)
    //    std::cout << "a: " << a << "\tb: "<<b << std::endl;


    if (!_selectedEntity) return;

    // Make sure to call inside ImGui frame:
    ImGuizmo::BeginFrame();

    // Get viewport size
    ImVec2 windowPos = ImGui::GetWindowPos();
    auto windowSize = ImGui::GetWindowSize(); //_camera->getWindowSize();

    // Setup ImGuizmo rect
    //ImGuizmo::SetDrawlist(); // bu olmadan gizmo gozukmuyor
    ImGuizmo::SetDrawlist(ImGui::GetWindowDrawList());
    ImGuizmo::SetRect(windowPos.x, windowPos.y, windowSize.x, windowSize.y);


    glm::mat4 modelMatrix = _selectedEntity->transform->getGlobalMatrix();

    bool gizmoUsed = ImGuizmo::Manipulate(
        glm::value_ptr(_camera->getViewMatrix()),
        glm::value_ptr(_camera->getProjectionMatrix()),
        mCurrentGizmoOperation,
        ImGuizmo::WORLD,
        glm::value_ptr(modelMatrix)
    );

    if (gizmoUsed)
        _selectedEntity->transform->setLocalMatrix(modelMatrix);

}


void SceneManager::addLight(LightType lightType)
{
    auto light = _lightMng->createLight(lightType);
    if (!light) {
        LOG_ERROR("SceneManager: Failed to create light");
        return;
	}
	auto entity = std::make_unique<Entity>();
	entity->name = light->name;
    entity->addComponent(std::move(light));
	_entities.push_back(std::move(entity));
}

void SceneManager::sceneQuery()//(Shader& shader)
{
	std::vector<Light*> lights;
    for (const auto& entity : _entities) 
        if (Light* light = entity->getComponent<Light>()) 
			lights.push_back(light);

    _lightMng->queryLights(lights);
}


void SceneManager::addShape(DefaultShapes shape)
{
    std::map<DefaultShapes, std::string> shapedToString {
        { DefaultShapes::Cube, "Cube" },
        { DefaultShapes::Cylinder, "Cylinder" },
        { DefaultShapes::Cone, "Cone" },
        { DefaultShapes::Plane, "Quad" },
        { DefaultShapes::Torus, "Torus" },
    };

	auto entity = std::make_unique<Entity>();
	std::string name = getUniqueName(shapedToString[shape]);
    
    entity->transform->name = name;
    entity->name = name;
    LOG_TRACE(shapedToString[shape]);

    entity->transform->setPosition(glm::vec3(0,0,0));
    entity->transform->setRotation(glm::vec3(0,0,0));
    entity->transform->setScale(glm::vec3(1,1,1));

	auto model = std::make_unique<Model>(_materialMng.get());
    model->loadDefault(shape);
	entity->addComponent(std::move(model));

    _entities.push_back(std::move(entity));
}

void SceneManager::addMonkey()
{
    auto entity = std::make_unique<Entity>();
    entity->name = getUniqueName("Monkey");
    auto model = std::make_unique<Model>(_materialMng.get());
    model->loadFromFile("models\\monkey.obj");
    entity->addComponent(std::move(model));
    _entities.push_back(std::move(entity));
}

void SceneManager::deleteSelected()
{
    if (!_selectedEntity) return;

    //_selectedEntity->terminate();

    //_entities.remove_if([this](const std::unique_ptr<Entity>& t) {
    //    return t.get() == _selectedEntity;
    //    });

    //_selectedEntities.remove_if([this](Entity* t) {
    //    return t == _selectedEntity;
    //    });

    _selectedEntity = nullptr;
}



std::string SceneManager::getUniqueName(std::string name)
{
    int i{};
    std::string uniq_name = name; 
    while (!isUniqueName(uniq_name))    
        uniq_name = name + std::to_string(i++);
    
    return uniq_name;
}

bool SceneManager::isUniqueName(std::string name)
{
    for (const auto& entity : _entities)
        if(entity->name == name) return false;

    return true;
}

