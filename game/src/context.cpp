#include "context.h"
#include <log.h>
//#include "luaApi.h"

Context::Context(){

}

void Context::setWindowParams(uint32_t w, uint32_t h, const std::string& name){
    m_screenWidth  = w;
    m_screenHeight = h;
    m_windowName   = name;
}

void Context::init(){
    m_glContext.init(m_screenWidth,m_screenHeight,m_windowName);
    m_primitives.init();
    m_renderer.init();

    for(auto str : m_luaScriptPaths){
        lua::Script* script= new lua::Script(str);
        if(script->load()){
            m_scripts.push_back(script);
            //script.registerFunction();
        }else{
            cprint_error() << "Failed to load script " << str << std::endl;
        }
    }
}

void Context::addScripts(const std::vector<std::string>& scripts){
    for(auto str : scripts){
        m_luaScriptPaths.push_back(str);
    }
}

void Context::loopForever(){
    while(m_running){
        m_glContext.clearFrame();
        m_glContext.checkInput([this](const uint8_t* keys,double x, double y){
           if(keys[SDL_SCANCODE_Q]){
            m_running = false;
            std::cout << " Q " << std::endl; 
          }
        });
        for(const lua::LuaRef& ref : m_luaFrameFunctions){
            ref.call((void*)this);
        }
        m_renderer.render(glm::mat4(1));
        m_glContext.showFrame();
    }
}

void Context::stopLooping(){
    m_running = false;
}
