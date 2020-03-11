#include "context.h"
#include <log.h>
#include "luaApi.h"
#define LOG_TAG "Context"

Context::Context(){
  m_glContext.setContextCallback(std::bind(&Context::onGLContextInit,this));
}

void Context::init(){
  m_glContext.init();
}


void Context::onGLContextInit(){
  m_primitives.init();
  m_renderer.init(); 
}

void Context::loadScript(const std::string& name){
  cprint_debug(LOG_TAG) << "Loading Script " << name << std::endl;
  lua::Script* script= new lua::Script(name);
  if(script->load()){
    script->createGlobal("_ID",m_scripts.size()); 
    script->createGlobal("_SCRIPT",(void*)this);
    m_scripts.push_back(script);
     
    int apiFunctions = sizeof(LuaApi) / sizeof(Lua_ApiCall);
    for(int i = 0; i < apiFunctions; i++){
      const Lua_ApiCall& apiCall = LuaApi[i];
      cprint_debug("Context") << "Registering Api call " << apiCall.name << " " << std::endl; 
      script->registerFunction(std::string(apiCall.name),*apiCall.call);
    }
    cprint_debug(LOG_TAG) << *script << std::endl; 
    EntityScript se;
    se.load(script->root());
    se.onInit();
    m_entities.push_back(se);
  }else{
    cprint_error(LOG_TAG) << "Error loading script " << name << std::endl;
  }
}

void Context::addComponent(GameComponent& gc){
  m_gameComponents.push_back(gc);
}

lua::Script* Context::getScriptById(uint32_t id){
  return m_scripts[id];
}

void Context::createWindow(const std::string& name, uint32_t w, uint32_t h){
  m_windows.push_back(m_glContext.createWindow(name,w,h));
}

void Context::loopForever(){
    while(m_running){
        
        m_glContext.checkInput([this](const uint8_t* keys,double x, double y){
           if(keys[SDL_SCANCODE_Q]){
            m_running = false;
          }
        });
        for(auto& e : m_entities){
          e.onFrame();
        }
        //for(auto s : m_scripts){
        //  lua::stackDump(s->luaState());
        //}
        for(int i = 0; i < m_gameComponents.size() ; i++){
          m_gameComponents[i].readLua();
        }

        for(int i = 0; i < m_windows.size(); i++){
          m_windows[i]->startDraw();
          m_windows[i]->clear();
          m_renderer.render(glm::mat4(1));
          m_windows[i]->show();
        }
    }
}

void Context::stopLooping(){
    m_running = false;
}
