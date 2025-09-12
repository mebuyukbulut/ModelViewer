#include "ParticleSystem.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <string>

glm::vec3 lerp(const glm::vec3& x, const glm::vec3& y, float t) {
    return x * (1.f - t) + y * t;
}

glm::vec3 bezierCubic(
    const glm::vec3& a,
    const glm::vec3& b,
    const glm::vec3& c,
    const glm::vec3& d,
    float t)
{
    glm::vec3 ab = glm::mix(a, b, t);
    glm::vec3 bc = glm::mix(b, c, t);
    glm::vec3 cd = glm::mix(c, d, t);

    glm::vec3 ab_bc = glm::mix(ab, bc, t);
    glm::vec3 bc_cd = glm::mix(bc, cd, t);

    return glm::mix(ab_bc, bc_cd, t);
}
glm::vec3 bezierQuadratic(
    const glm::vec3& a,
    const glm::vec3& b,
    const glm::vec3& c,
    float t)
{
    glm::vec3 ab = glm::mix(a, b, t);
    glm::vec3 bc = glm::mix(b, c, t);

    return glm::mix(ab, bc, t);
}

void Particle::update(const ParticleUpdateContext& ctx) {
    float deltaTime = ctx.deltaTime;
    float elapsedTime = ctx.elapsedTime;


    age += deltaTime;
    float normalizedAge = age / lifetime;

    float mass = 1;
    glm::vec3 totalForce{};
    glm::vec3 acceleration{};
    std::vector<std::unique_ptr<IForce>> forces;
    WindForce wf{};
    GravityForce gf{};
    NoiseForce nf{};

    wf.direction = glm::vec3(1.0f, 0.0f, 0.0f);
    nf.strength = 3;

    forces.push_back(std::make_unique<WindForce>(wf));
    forces.push_back(std::make_unique<NoiseForce>(nf));
    //forces.push_back(std::make_unique<GravityForce>(gf));
    forces.push_back(std::make_unique<QuadraticDragForce>());

    for (auto& f : ctx.forces)
        totalForce += f->apply(*this, deltaTime);

    acceleration = totalForce / mass; 
    velocity += acceleration * deltaTime;
    position += velocity * deltaTime;




    //// https://libnoise.sourceforge.net/tutorials/tutorial2.html
    //noise::module::Perlin myModule;
    ////noise::module::random
    //
    //float a = glm::linearRand(0.0f, 10.0f);
    //
    //position.y = glm::perlin(glm::vec3((a + elapsedTime) * 2, 0, 0));
    //position.x = a;

    //color = _colorFunction->evaluate(normalizedAge);
    color = ctx.colorProvider->evaluate(normalizedAge);


    //std::cout << position.x << std::endl; 
    if (age > lifetime) isactive = false;
}



void EmitterInfo::drawUI() {
    ImGui::SeparatorText("Emitter Info");
    ImGui::DragInt("spawn rate: ", &spawnRate, 1, 0, 300);

    static const char* items[]{ "Point","Sphere","Torus" }; // box
    static int Selecteditem = 0;
    bool check = ImGui::Combo("Emitter Shape", &Selecteditem, items, IM_ARRAYSIZE(items));
    if (check)
    {
        switch (Selecteditem)
        {
        case 0:
            shape.reset(new PointShape);
            break;
        case 1:
            shape.reset(new SphereShape);
            break;
        case 2:
            shape.reset(new TorusShape);
            break;
        default:
            std::cout << "Error: line 130 ParticleSystem.cpp" << std::endl;
            break;
        }
    }
    shape->drawUI();
}
void ParticleUpdateContext::drawUI()
{
    ImGui::SeparatorText("Color");

    // COLOR
    static const char* items[]{ "Constant","LifeTime","Gradient" }; // box
    static int colorSelecteditem = 0;
    if (ImGui::Combo("Color Provider", &colorSelecteditem, items, IM_ARRAYSIZE(items)))
    {
        switch (colorSelecteditem)
        {
        case 0:
            colorProvider.reset(new ConstantColorProvider);
            break;
        case 1:
            colorProvider.reset(new ColorOverLifeProvider);
            break;
        case 2:
            colorProvider.reset(new GradientProvider);
            break;
        default:
            std::cout << "Error: line 157 ParticleSystem.cpp" << std::endl;
            break;
        }
    }
    colorProvider->drawUI();


    ImGui::SeparatorText("Force");
    // FORCES
    // Gravity
    // Wind
    // Noise
    // Drag
    // QuadraticDragForce

    static const char* forceItems[]{ "Gravity","Wind","Noise","Drag", "QuadraticDrag" }; // box
    static int forceSelecteditem = 0;
    ImGui::Combo("Force Type", &forceSelecteditem, forceItems, IM_ARRAYSIZE(forceItems));
    //std::vector<std::unique_ptr<IForce>> forces{};
    
    if (ImGui::Button("Add Force")) {
        switch (forceSelecteditem)
        {
        case 0: {// Gravity
            forces.push_back(std::make_unique<GravityForce>());
            break;
        }
        case 1: {// Wind
            forces.push_back(std::make_unique<WindForce>());
            break;
        }
        case 2: {// Noise
            forces.push_back(std::make_unique<NoiseForce>());
            break;
        }
        case 3: {// Drag
            forces.push_back(std::make_unique<DragForce>());
            break;
        }
        case 4: {// QuadraticDrag
            forces.push_back(std::make_unique<QuadraticDragForce>());
            break;
        }
        default:
            std::cout << "Error: line 160 ParticleSystem.cpp" << std::endl;
            break;
        }
    }
        

    static int selectedIndex = -1;
    if (ImGui::BeginListBox("Force"))
    {
        for (int i{}; i < forces.size(); i++) //(int n = 0; n < IM_ARRAYSIZE(items); n++)
        {
            const bool is_selected = (selectedIndex == i);
            if (ImGui::Selectable(std::to_string(i).c_str(), is_selected)) {
                //std::cout << "Selected light: " << light.name << std::endl;
                selectedIndex = i;
            }
            //if (ImGui::IsItemHovered())
            //    std::cout << "hovered light: " << light.name << std::endl;
            //// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            //if (is_selected)
            //    ImGui::SetItemDefaultFocus();
        }
        ImGui::EndListBox();
    }

    if (selectedIndex == -1) {  return; } // No force selected

    if (ImGui::Button("Delete Force")) {
        forces.erase(forces.begin() + selectedIndex);
        selectedIndex = -1;
        return;
    }
    forces[selectedIndex]->drawUI();
}
void ParticleSystem::drawUI() {
    ImGui::Begin("Particle System");

    //ImGui::SeparatorText("Particle System");
    info.drawUI();

    _particleCtx.drawUI();

    ImGui::End();
}

