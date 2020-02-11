#include <iostream>
#include <vector>
#include <GL/glew.h>

#include "renderer.h"
#include "core.h"
#include "sdlGL.h"


class ContextConfig{
public:
    void load(lua::LuaRef root){

    }
private:

};

class LuaEntity{
public:
    void load(lua::LuaRef root){
        
    }
private:

}

class Context{
public:
    void addScripts(const std::vector<std::string>& scripts){

    }

    void init(){

    }

    void loopForever(){

    }
private:
    SdlGlContext             m_glContext:
    render::gl::Renderer     m_renderer;
    std::vector<std::string> m_luaScriptPaths;
    std::vector<lua::Script> m_scripts;
};