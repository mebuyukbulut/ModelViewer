#pragma once
#include <string>



class Config
{
    struct Window {
        int width;
        int height;
        std::string title;
        bool fullscreen;
    };

    struct Graphics {    
    };
public:
    Window window;
    void load(std::string path = "config.yaml");
    void save(std::string path = "config.yaml");
};

