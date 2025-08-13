#include "Engine.h"
#include "Shader.h"
#include "FileUtils.h"
#include "Model.h"
#include "Renderer.h"
#include <functional>




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
    glfwSetMouseButtonCallback(_window, Engine::mouse_button_callback);
    glfwSetCursorPosCallback(_window, Engine::mouse_cursor_callback);
    glfwSetScrollCallback(_window, Engine::scroll_callback);



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

void Engine::init(){
    initWindow();
	initOpenGL();    

	_camera.init();
    _renderer.init();
	_renderer.setCamera(&_camera);
    //_renderer.enableWireframe();
    //_model.loadDefault();
    Model model;
	model.loadFromFile("models/monkey.obj");
	_models.push_back(model);


    _mouseLastX = 0; 
    _mouseLastY = 0; 
    _mouseLeftPress = false;
    _firstMouse = true;

    _UI.init(_window);
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
        } else {
            std::cout << "Failed to load model: " << modelPath << std::endl;
        }
	};

}
void Engine::mainLoop()
{
    // render loop
    // -----------
    while (!glfwWindowShouldClose(_window))
    {
        processInput(_window);
        _renderer.beginFrame();
        for(Model& model : _models) {
            _renderer.drawModel(model);
		}
        //_renderer.drawModel(_model);

		float x = sin(glfwGetTime()) * 3;
		float y = cos(glfwGetTime()) * 3;
		_renderer.getShader().setVec3("lightPos", glm::vec3(x, 2,y));
		_renderer.getShader().setVec3("viewPos",_camera.getPosition());


		_UI.draw();

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
        app->_camera.setAspectRatio(width,height);
    }
}

void Engine::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    Engine* app = static_cast<Engine*>(glfwGetWindowUserPointer(window));
    if (!app) return;

    //if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS);
    
	//std::cout << app->_UI.isHoverOnUI() << std::endl;

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && !app->_UI.isHoverOnUI())
        app->_mouseLeftPress = true;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        app->_mouseLeftPress = false;
        app->_firstMouse = true;
    }

}

void Engine::mouse_cursor_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    Engine* app = static_cast<Engine*>(glfwGetWindowUserPointer(window));
    if (app && app->_mouseLeftPress) {        
        if (app->_firstMouse) {
            app->_mouseLastX = xposIn;
            app->_mouseLastY = yposIn;
            app->_firstMouse = false; 
            return;
        }

        float xoffset = xposIn - app->_mouseLastX;
        float yoffset = app->_mouseLastY - yposIn; // reversed since y-coordinates go from bottom to top
        
        app->_mouseLastX = xposIn;
        app->_mouseLastY = yposIn;

        xoffset = glm::radians(.4f) * xoffset;
        yoffset = glm::radians(.4f) * yoffset;
        //std::cout << xoffset << "\t" << yoffset << std::endl;
        app->_camera.rotate(xoffset,yoffset);
    }
}

void Engine::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    Engine* app = static_cast<Engine*>(glfwGetWindowUserPointer(window));
    if (app) {
        app->_camera.zoom(static_cast<float>(yoffset));
    }
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void  Engine::processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

}

