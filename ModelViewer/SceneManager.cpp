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
#include "EventDispatcher.h"
#include "Inspectable.h"
#include "LightManager.h"
//#include "Material.h"
#include "Renderer.h"
//#include "Scene.h"
#include "Shader.h"
#include "UIManager.h"


enum class ImguizmoState
{
    Translate,
    Rotate,
    Scale
};
ImguizmoState _GizmoState;

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

    dispatcher.subscribe(EventType::Delete, [&](const Event& e) {
        deleteSelected();
        });
    dispatcher.subscribe(EventType::Select, [&](const Event& e) {
        mousePos = glm::vec2(e.data.vec.x, e.data.vec.y);
        isSelect = true;
        });

    dispatcher.subscribe(EventType::ScenePopup, [&](const Event& e) {
        isScenePopupOpen = true;
        });

    _materialMng.reset(new MaterialManager(shader));

    CreateRenderTarget(_rt, 300, 300);

    //Transform* transform = new Transform;
    //Entity* entity = new Entity;
    ////_materialMng.reset(new MaterialManager(shader));
    //entity->model.reset(new Model(_materialMng.get()));
    //entity->model->loadFromFile("models\\monkey.obj");
    //entity->_renderer = renderer;
    //transform->setEntity(entity);
    //transform->name = "myobject"; 
    //_transforms.emplace_back(transform);

    //Transform* t2 = new Transform;
    //t2->name = "my new object";
    //t2->setEntity(entity);
    //_transforms.emplace_back(t2);

}
void SceneManager::draw() {
    // FBO’ya çiz
    glBindFramebuffer(GL_FRAMEBUFFER, _rt.fbo);
    glViewport(0, 0, _rt.width, _rt.height);
    //glClearColor(1, 0, 0, 1); // error check

    if (isSelect) {
        glClearColor(0, 0, 0, 1);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        for (auto& transform : _transforms) {
            if (transform.get()->getEntity()->model)
                transform->drawAsColor(_renderer);
        }
        glm::vec2 mPos = glm::vec2(mousePos.x, mousePos.y);
        glm::vec2 panelPos = glm::vec2(viewportPos.x, viewportPos.y);
        glm::vec2 panelSize = glm::vec2(viewportPanelSize.x, viewportPanelSize.y);
        mPos = mPos - panelPos;
        mPos.y = panelSize.y - mPos.y;
        uint32_t selectedID = _renderer->getSelection(mPos);
        //LOG_TRACE(std::to_string(_renderer->getSelection(mPos)));
        //LOG_TRACE(std::to_string(mPos.x) + " " + std::to_string(mPos.y));
        if (selectedID != 0)
            for (auto t : _transforms) {
                if (t->ID == selectedID)
                    _selectedTransform = t.get();
            }
        else; // Deselect
        //_selectedTransform = nullptr;

        isSelect = false;
    }

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    _renderer->drawBackground();

    for (auto& transform : _transforms) {
        if (transform.get()->getEntity()->model)
            transform->draw(_renderer);
    }
    _renderer->drawGrid();
    _renderer->getShader().use();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void SceneManager::loadScene(std::string path) {
    LOG_TRACE("Load Scene");

    //YAML::Node root = YAML::LoadFile(path);

    //for (const auto& lightNode : root["Lights"]) {
    //    auto light = LightFactory::create(lightNode);

    //    if (light.get()) {

    //        LOG_TRACE("Load light");
    //        addLight(std::move(light));
    //    }
    //}


    YAML::Node rootTest = YAML::LoadFile("save0.yaml");
    for (const auto& transformNode : rootTest) {
        auto transform = TransformFactory::create(transformNode, _materialMng.get());

        if (transform.get()) {

            LOG_TRACE("Load transform...");
            _transforms.emplace_back(std::move(transform));
            //addLight(std::move(transform));
        }
    }


}
void SceneManager::saveScene() {
    LOG_TRACE("Save Scene");
    YAML::Node node;
    for (auto transform : _transforms) {
        node.push_back(transform->serialize());
        //Light* light = transform->getEntity()->light.get();
        //if(light)
        //    node["Lights"].push_back(light->serialize());
    }
    std::string path = "save0.yaml";
    std::ofstream fout(path);
    fout << node;
}


void SceneManager::drawUI()
{
    ImGui::Begin("Scene");
    ImGuiTreeNodeFlags flag = 
        ImGuiTreeNodeFlags_DefaultOpen |
        ImGuiTreeNodeFlags_Bullet |
        ImGuiTreeNodeFlags_Leaf;
    ImGuiTreeNodeFlags flag2 =  ImGuiTreeNodeFlags_Leaf;
    ImGuiTreeNodeFlags selectedFlag =  ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Selected;
    //ImGuiTreeNodeFlags selectedFlag =  ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Framed;
    if (ImGui::IsItemClicked())
    {
        std::cout << " scene window was clicked \n";
        // Mark rendered node as being clicked
    }
    if (ImGui::TreeNodeEx("scene", flag))
    {
        if (ImGui::IsItemClicked())
        {
            std::cout << " I am clicked \n"; 
            // Mark rendered node as being clicked
        }
        // Call ImGui::TreeNodeEx() recursively to populate each level of children

        // gray background
        ImGui::PushStyleColor(ImGuiCol_Header, IM_COL32(55, 55, 55, 255));
        for (auto&& i : _transforms) {
            bool lastSelected = false;
            if (i.get() == getSelectedTransform()) {
                lastSelected = true;
                // red foreground
                ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(230, 125, 15, 255));
                //// gray background
                //ImGui::PushStyleColor(ImGuiCol_Header, IM_COL32(65, 65, 65, 255));
            }
            ImGui::TreeNodeEx(i->name.c_str(), i->isSelected() ? selectedFlag : flag2); 
            if (lastSelected)
                ImGui::PopStyleColor(1);

            if (ImGui::IsItemClicked())
            {
                // One item select 
                for (auto&& i : _selectedTransforms)
                    i->deselect();
                _selectedTransforms.clear();

                // TO-DO multiple item select

                _selectedTransforms.push_back(i.get());
                _selectedTransform = i.get();

                _selectedTransforms.sort();
                _selectedTransforms.unique();
                //std::cout << _selectedTransforms.size() << std::endl; 
                i->select();
            }
            ImGui::TreePop();
        }

        ImGui::PopStyleColor(1);


        ////fmt::format()
        //ImGui::TreeNodeEx("1bc", flag2); ImGui::TreePop();
        //if (ImGui::TreeNodeEx("2bc")) { 
        //    ImGui::TreeNodeEx("0bc", flag2); ImGui::TreePop();
        //    ImGui::TreeNodeEx("1bc", flag2); ImGui::TreePop();
        //    ImGui::TreePop();
        //}

        ImGui::TreePop();  // This is required at the end of the if block
    }


    // bos alana tiklamayi yakala:
    if (ImGui::IsWindowFocused() &&
        ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem) &&
        ImGui::IsMouseClicked(ImGuiMouseButton_Left) &&
        !ImGui::IsAnyItemHovered()
        )
    {
        std::for_each(_selectedTransforms.begin(), _selectedTransforms.end(), [](auto&& t) { t->deselect(); });
        _selectedTransforms.clear();
        _selectedTransform = nullptr; 
        //deselectAll();
        
    }


    ImGui::End();


    ImGui::Begin("Properties");
	if (auto t = getSelectedTransform())
		t->drawUI();


    ImGui::End();





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
        _GizmoState = ImguizmoState::Translate; 
    ImGui::SameLine();
    if (ImGui::Button("R", ImVec2(0, 37))) // rotate
        _GizmoState = ImguizmoState::Rotate; 
    ImGui::SameLine();
    if (ImGui::Button("S", ImVec2(0, 37))) // scale
        _GizmoState = ImguizmoState::Scale; 
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






    if (!_selectedTransform) return;

    // Make sure to call inside ImGui frame:
    ImGuizmo::BeginFrame();

    // Get viewport size
    ImVec2 windowPos = ImGui::GetWindowPos();
    auto windowSize = ImGui::GetWindowSize(); //_camera->getWindowSize();

    // Setup ImGuizmo rect
    //ImGuizmo::SetDrawlist(); // bu olmadan gizmo gozukmuyor
    ImGuizmo::SetDrawlist(ImGui::GetWindowDrawList());
    ImGuizmo::SetRect(windowPos.x, windowPos.y, windowSize.x, windowSize.y);



    // Choose operation: translate / rotate / scale
    static ImGuizmo::OPERATION mCurrentGizmoOperation = ImGuizmo::TRANSLATE;// | ImGuizmo::ROTATE | ImGuizmo::SCALE;
    switch (_GizmoState)
    {
    case ImguizmoState::Translate:
        mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
        break;
    case ImguizmoState::Rotate:
        mCurrentGizmoOperation = ImGuizmo::ROTATE;
        break;
    case ImguizmoState::Scale:
        mCurrentGizmoOperation = ImGuizmo::SCALE;
        break;
    default:
        break;
    }





    glm::mat4 modelMatrix(1); 
    modelMatrix = glm::translate(glm::mat4(1.0f), _selectedTransform->getPosition());
    //modelMatrix *= glm::mat4_cast(glm::quat(_selectedTransform->getRotation())); // if rotation stored as quat
    modelMatrix = glm::rotate(modelMatrix, glm::radians(_selectedTransform->getRotation().x), glm::vec3(1, 0, 0));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(_selectedTransform->getRotation().y), glm::vec3(0, 1, 0));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(_selectedTransform->getRotation().z), glm::vec3(0, 0, 1));
    modelMatrix = glm::scale(modelMatrix, _selectedTransform->getScale());


    ImGuizmo::Manipulate(
        glm::value_ptr(_camera->getViewMatrix()),
        glm::value_ptr(_camera->getProjectionMatrix()),
        mCurrentGizmoOperation,
        ImGuizmo::WORLD,
        glm::value_ptr(modelMatrix)
    );

    float translation[3], rotation[3], scale[3];
    ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(modelMatrix), translation, rotation, scale);

    
    // If modified, write back into your glm::mat4
    if (ImGuizmo::IsUsing()) {
        switch (_GizmoState)
        {
        case ImguizmoState::Translate:
            _selectedTransform->setPosition(glm::make_vec3(translation));
            break;
        case ImguizmoState::Rotate:
            _selectedTransform->setRotation(glm::make_vec3(rotation)); // convert to radians if needed
            break;
        case ImguizmoState::Scale:
            _selectedTransform->setScale(glm::make_vec3(scale));
            break;
        default:
            break;
        }
    }


}


const int MAX_LIGHTS = 8; 
int lightCount = 0;
void SceneManager::addLight(LightType lightType)
{
    //static int lightCounter = 0; // Static counter to keep track of light names

    if (lightCount >= MAX_LIGHTS) {
        std::cout << "ERROR: Maximum number of lights reached!" << std::endl;
        return; // Prevent adding more lights if the limit is reached
    }
    std::unique_ptr<Light> newLight;
    std::string lightName = "";
    switch (lightType)
    {
    case LightType::Directional:
        newLight = std::make_unique<DirectionalLight>();
        lightName = "Directional Light" + std::to_string(lightCount);
        break;
    case LightType::Point:
        newLight = std::make_unique<PointLight>();
        lightName = "Point Light" + std::to_string(lightCount);
        break;
    case LightType::Spot:
        newLight = std::make_unique<SpotLight>();
        lightName = "Spot Light" + std::to_string(lightCount);
        break;
    default:
        break;
    }
    Transform* t = new Transform; 
    Entity* en = new Entity;
    en->light = std::move(newLight);
    t->setEntity(en);
    t->name = lightName;
    _transforms.emplace_back(t);


    lightCount++;

}

void SceneManager::addLight(std::unique_ptr<Light> light)
{

    //std::unique_ptr<Light> newLight;
    Transform* t = new Transform;
    t->name = light->name;
    Entity* en = new Entity;
    en->light = std::move(light);
    t->setEntity(en);
    _transforms.emplace_back(t);


    lightCount++;

}

void SceneManager::configShader(Shader& shader)
{
    int counter = 0; 
    for (auto t : _transforms) {
        if (Light* light = t.get()->getEntity()->light.get()) {

            std::string prefix = "_lights[" + std::to_string(counter++) + "].";
            light->configShader(shader, prefix);
        }
    }

    shader.setInt("numLights", counter);
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

    Transform* transform = new Transform;
    transform->setEntity(new Entity);
    transform->name = getUniqueName(shapedToString[shape]);
    LOG_TRACE(shapedToString[shape]);

    transform->setPosition(glm::vec3(0,0,0));
    transform->setRotation(glm::vec3(0,0,0));
    transform->setScale(glm::vec3(1,1,1));


    Model* model = new Model(_materialMng.get());
    model->loadDefault(shape);
    transform->getEntity()->model.reset(model);

    _transforms.emplace_back(transform);
}

void SceneManager::deleteSelected()
{
    if (!_selectedTransform) return;

    _selectedTransform->terminate();

    _transforms.remove_if([this](const std::shared_ptr<Transform>& t) {
        return t.get() == _selectedTransform;
        });

    _selectedTransforms.remove_if([this](Transform* t) {
        return t == _selectedTransform;
        });

    _selectedTransform = nullptr;
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
    for (auto transform : _transforms)
        if (transform->name == name) return false;
    return true;
}

