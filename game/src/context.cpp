#include "context.h"
#include <log.h>
#include "luaApi.h"
#include <chrono>
#define LOG_TAG "Context"

Context::Context(){
  //m_glContext.setContextCallback(std::bind(&Context::onGLContextInit,this));
}

void Context::init(){
  m_windowManager.create(SDLWindowManager::Backend::GL);
  m_windowManager.addWindow("Game",800,600);
  m_glRenderContext = new render::gl::GLContext();
  m_glRenderContext->create();

  m_luaContext.init(0);
  int fcount = sizeof(LuaApi)/sizeof(Lua_ApiCall); 
  for(int i =0 ; i < fcount ; i++){
    cprint_debug(LOG_TAG) << "Registering Function " << LuaApi[i].name << std::endl;
    m_luaContext.registerFunction(LuaApi[i].name,LuaApi[i].call);
  }
  m_luaContext.createGlobal("_SCRIPT",(void*)this);
}


void Context::onGLContextInit(){

}

void Context::loadLua(const std::string& name){
  cprint_debug(LOG_TAG) << "Loading Script " << name << std::endl;
  m_tasks.push_back([=](){
    core::file::File f = core::file::File(name);
    f.load();
    this->m_luaContext.loadBuffer(f.data(),f.size());    
  });
}

void Context::loadLua(const uint8_t* data,uint32_t len){
  m_tasks.push_back([=](){
    cprint_debug(LOG_TAG) << "Loading Buffer " << (void*) data << " " << len << std::endl;
    this->m_luaContext.loadBuffer(data,len,cprint_debug("Lua"));    
  });
}

void Context::createWindow(const std::string& name, uint32_t w, uint32_t h){
  
  /**
  const std::string cname = name;
  std::cout << "push_back create window 0" << std::endl;
  cprint_debug(LOG_TAG) << "Creating Window " << cname << "(" << w << "," << h << ")" << std::endl;
  m_tasks.push_back([cname,w,h,this](){
      cprint_debug(LOG_TAG) << "Creating Window " << cname << "(" << w << "," << h << ")" << std::endl;
      this->m_windows.push_back(m_glContext.createWindow(cname,w,h));
  });
  std::cout << "push_back create window 1" << std::endl;
  **/  
}

void Context::loopForever(){
  m_glRenderContext->createCamera();
  m_glRenderContext->createPrimitive(render::primitive::Triangle);
  while(m_running){        
    m_tasks.run();  
    for(auto& luaObjectVariant : m_mappedLuaObjects){
      std::visit([](auto&& mappedObject){
        mappedObject.read();
      },luaObjectVariant);
    }

    for(auto& window : m_windowManager.windows()){ 
      window->beginDraw(); 
      m_glRenderContext->render();
      window->endDraw();
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(16));
  }
}


void Context::stopLooping(){
    m_running = false;
}
