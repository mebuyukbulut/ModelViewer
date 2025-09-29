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
#include "Mouse.h"

#include "EventDispatcher.h"


void Engine::initWindow()
{   // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // glfw window creation
    // --------------------
    _window = glfwCreateWindow(
        config.window.width, 
        config.window.height, 
        config.window.title.c_str(),
        config.window.fullscreen ? glfwGetPrimaryMonitor() : NULL,
        NULL);
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
    dispatcher.subscribe(EventType::onMove, [&](const Event& e) {
        _camera->move({ e.data.vec.x, e.data.vec.y, e.data.vec.z });
        });
    dispatcher.subscribe(EventType::onRotate, [&](const Event& e) {
        _camera->rotate({ e.data.vec.x, e.data.vec.y, e.data.vec.z });
        });
    dispatcher.subscribe(EventType::onZoom, [&](const Event& e) {
        _camera->zoom(e.data.vec.y);
        });
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
    _UI.init(_window, &_lightManager, _camera);
	//_UI.setWindowSize(SCR_WIDTH, SCR_HEIGHT);

    dispatcher.subscribe(EventType::ShaderSelected, [&](const Event& e) {
        _renderer.setShader(e.data.text);
        });
    dispatcher.subscribe(EventType::EngineExit, [&](const Event& e) {
        glfwSetWindowShouldClose(_window, true);
        });
    dispatcher.subscribe(EventType::ModelOpened, [&](const Event& e) {
        std::string modelPath = e.data.text;

        if (modelPath.empty())
            return;

        //for (Model& i : _models)
        //    i.terminate();
        //_models.clear();

        //Model model;
        //if (model.loadFromFile(modelPath)) {
        //    _models.push_back(model);
        //    //std::wcout << L"Success to load model: " << FileUtils::UTF8ToWString(modelPath) << std::endl;
        //    std::cout << "Success to load model: " << modelPath << std::endl;
        //}
        //else {
        //    std::cout << "Failed to load model: " << modelPath << std::endl;
        //}
        });

    _UI.ps = &ps;
}

void Engine::init(){
    config.load();

    initWindow();
	initOpenGL();    
    
	_camera->init(glm::vec2(config.window.width, config.window.height));
	_camera->setWindowSize(config.window.width, config.window.height);
    _renderer.init();
	_renderer.setCamera(_camera);
    _renderer.setShader("PBR0");
	_lightManager.init(_camera);
    //_renderer.enableWireframe();

    SM.init(&_renderer, _camera.get());


	initUI();
}
void Engine::mainLoop()
{
    Material material;


    ///_renderer.setShader("particle0");
    /////ParticleSystem ps;
    ///ps.init(_camera);
    
    time.init();
    // render loop
    // -----------
    while (!glfwWindowShouldClose(_window))
    {
        processInput(_window);
        _renderer.beginFrame();

        time.update();
        double deltaTime = time.deltaTime();

        //ps.update(deltaTime);
        //ps.draw();
        
        material.use(&_renderer.getShader()); 

        SM.draw();



		_lightManager.configShader(_renderer.getShader());
        
        _renderer.getShader().setVec3("viewPos", _camera->getPosition());
        _renderer.getShader().setVec3("ambientColor", glm::vec3(1, 1, 1));
        _renderer.getShader().setFloat("ambientIntensity", 0.1);


		_UI.draw(&material, &SM);
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(_window);
        glfwPollEvents();
    }
}
void Engine::terminate() {
    ////for (Model& model : _models) 
    ////    model.terminate();
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
		std::cout << "Window resized to: " << width << "x" << height << std::endl;
        if(width && height)
            app->_camera->setWindowSize(width,height);
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
        _camera->resetFrame();

}

