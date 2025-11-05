#include "SceneManager.h"
#include <imgui.h>
#include <ImGuizmo.h>
#include "Camera.h"
#include <glm/gtc/type_ptr.hpp>
#include "LightManager.h"
#include <imgui_internal.h>
#include "UIManager.h"


enum class ImguizmoState
{
    Translate,
    Rotate,
    Scale
};
ImguizmoState _GizmoState;

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


//void UIManager::beginFrame() {
//    ImGui_ImplOpenGL3_NewFrame();
//    ImGui_ImplGlfw_NewFrame();
//    ImGui::NewFrame();
//
//
//    // dock directly to main window
//    ImGuiID dockspace_id = ImGui::DockSpaceOverViewport();
//}
//void UIManager::endFrame() {
//    ImGui::Render();
//    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//}


// LIGHTS 

//void SceneManager::lightUI() {
//    // Shader selection combo box
//    const char* items[] = { "Point", "Direction", "Spot" };
//    static int currentItem = 0;
//
//    ImGui::Begin("Light Controls");
//
//    ImGui::SeparatorText("Add Light");
//    ImGui::Combo("type", &currentItem, items, IM_ARRAYSIZE(items));
//    if (ImGui::Button("Add Light"))
//        addLight(currentItem);
//
//
//    ImGui::SeparatorText("Light List");
//
//    if (ImGui::BeginListBox("listbox 1"))
//    {
//        for (int i{}; i < _lights.size(); i++) //(int n = 0; n < IM_ARRAYSIZE(items); n++)
//        {
//            std::string lightName = _lights[i]->name;
//            const bool is_selected = (selectedLight == i);
//            if (ImGui::Selectable(lightName.c_str(), is_selected)) {
//                //std::cout << "Selected light: " << light.name << std::endl;
//                selectedLight = i;
//            }
//            //if (ImGui::IsItemHovered())
//            //    std::cout << "hovered light: " << light.name << std::endl;
//            //// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
//            //if (is_selected)
//            //    ImGui::SetItemDefaultFocus();
//        }
//        ImGui::EndListBox();
//    }
//
//
//    ImGui::SeparatorText("Light properties");
//    if (selectedLight == -1) { ImGui::End(); return; } // No light selected
//
//    _lights[selectedLight]->drawUI();
//
//    ImGui::End();
//}
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


void SceneManager::addShape(DefaultShapes shape) {
    ////MeshFactory mf; 
    ////auto cubeMesh = mf.create(DefaultShapes::Cube);

    //Transform* transform = new Transform;
    //Entity* entity = new Entity;

    //entity->model.reset(new Model(_materialMng.get()));
    //entity->model->loadDefault();
    ////entity->model->addMesh(cubeMesh);
    //entity->_renderer = _renderer;
    //transform->setEntity(entity);
    //transform->name = "my cube object";
    //_transforms.emplace_back(transform);




    Transform* transform = new Transform;
    transform->setEntity(new Entity);
    transform->name = "my cube object";
    transform->setPosition(glm::vec3(0,0,0));
    transform->setRotation(glm::vec3(0,0,0));
    transform->setScale(glm::vec3(1,1,1));


    Model* model = new Model(_materialMng.get());
    model->loadDefault(shape);
    transform->getEntity()->model.reset(model);
    LOG_TRACE("add Cube");

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

