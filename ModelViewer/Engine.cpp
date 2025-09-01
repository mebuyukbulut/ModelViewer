#include "Engine.h"
#include "Shader.h"
#include "FileUtils.h"
#include "Model.h"
#include "Renderer.h"
#include "Material.h"
#include <functional>


#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <ImGuizmo.h>
#include "ParticleSystem.h"
#include "Mouse.h"

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


void Engine::initWindow()
{   // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // glfw window creation
    // --------------------
    _window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (_window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        //return -1;
    }
    glfwMakeContextCurrent(_window);
    glfwSetWindowUserPointer(_window, this);
    glfwSetFramebufferSizeCallback(_window, Engine::framebuffer_size_callback);

    _mouse.init(_window, &_UI);
    _mouse.onMove = [&](float dx, float dy) { _camera.move(dx, dy); };
    _mouse.onRotate = [&](float dx, float dy) { _camera.rotate(dx, dy); };
    _mouse.onZoom = [&](float dy) { _camera.zoom(dy); };



}

void Engine::initOpenGL()
{
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        //return -1;
    }

}

void Engine::initUI()
{
    _UI.init(_window, &_lightManager, &_camera);
	//_UI.setWindowSize(SCR_WIDTH, SCR_HEIGHT);

    _UI.onShaderSelected = ([&](std::string shaderName) {
        _renderer.setShader(shaderName);
        });
    _UI.onEngineExit = [&]() {
        glfwSetWindowShouldClose(_window, true);
        };
    _UI.onOpenModel = [&](std::string modelPath) {
        if (modelPath.empty())
            return;

        for (Model& i : _models)
            i.terminate();
        _models.clear();

        Model model;
        if (model.loadFromFile(modelPath)) {
            _models.push_back(model);
            //std::wcout << L"Success to load model: " << FileUtils::UTF8ToWString(modelPath) << std::endl;
            std::cout << "Success to load model: " << modelPath << std::endl;
        }
        else {
            std::cout << "Failed to load model: " << modelPath << std::endl;
        }
        };
}

void Engine::init(){
    initWindow();
	initOpenGL();    

	_camera.init();
	_camera.setWindowSize(SCR_WIDTH, SCR_HEIGHT);
    _renderer.init();
	_renderer.setCamera(&_camera);
	_lightManager.init(&_camera);
    //_renderer.enableWireframe();
    //_model.loadDefault();
    Model model;
	model.loadFromFile("models\\monkey.obj");
	//model.loadFromFile("models\\bellapais_abbey\\Bellapais Abbey.obj");
	_models.push_back(model);



	initUI();
}
void Engine::mainLoop()
{
    Material material;


    _renderer.setShader("particle0");
    ParticleSystem ps;
    ps.init(&_camera);
    
    time.init();
    // render loop
    // -----------
    while (!glfwWindowShouldClose(_window))
    {
        processInput(_window);
        _renderer.beginFrame();

        time.update();
        double deltaTime = time.deltaTime();

        ps.update(deltaTime);
        ps.draw();


        //material.use(&_renderer.getShader());
        //for(Model& model : _models) {
        //    _renderer.drawModel(model);
		//}
		//_lightManager.configShader(_renderer.getShader());
        ////_renderer.drawModel(_model);
        //_renderer.getShader().setVec3("viewPos", _camera.getPosition());
        //_renderer.getShader().setVec3("ambientColor", glm::vec3(1, 1, 1));
        //_renderer.getShader().setFloat("ambientIntensity", 0.1);


		_UI.draw(&material);
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(_window);
        glfwPollEvents();
    }
}
void Engine::terminate() {
    for (Model& model : _models) 
        model.terminate();
    //_model.terminate();
    _renderer.terminate();
    _UI.terminate();
    glfwTerminate();
}

void Engine::run()
{
    init();
    mainLoop();
	terminate();
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void Engine::framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);

    // Retrieve the instance pointer
    Engine* app = static_cast<Engine*>(glfwGetWindowUserPointer(window));
    if (app) {
		//std::cout << "Window resized to: " << width << "x" << height << std::endl;
        app->_camera.setWindowSize(width,height);
		//app->_UI.setWindowSize(width, height);
		//app->_camera.setAspectRatio(width, height);
    }
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void  Engine::processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        _camera.resetFrame();

}

