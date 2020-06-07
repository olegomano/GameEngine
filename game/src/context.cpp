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
  SDLWindow* window = m_windowManager.addWindow("Game",900,900);
  if(window == nullptr){
    cprint_error(LOG_TAG) << "Failed init, windowManager returned null window" << std::endl;
    return;
  }
  m_glRenderContext = new render::gl::GLContext();
  m_glRenderContext->create(); 
  m_luaContext.init(0);
  
  render::scene::Entity camera = m_glRenderContext->createCamera(320,320);

  m_windowCameras[window->id()] = camera;

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
  cprint_debug(LOG_TAG) << "Creating Window " << name << "(" << w << "," << h << ")" << std::endl; 
  SDLWindow* window =  m_windowManager.addWindow(name,w,h); 
  if(window == nullptr){
    cprint_error(LOG_TAG) << "Failed to create window " << std::endl;
    return;
  }
  render::scene::Entity cameraEntity = m_glRenderContext->createCamera(w,h);  
  //m_windowCameras[window] = cameraEntity;
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
  std::vector<render::scene::Entity> e;

  int w = 2;
  int h = 1;
  for(int x =0 ; x < w; x++){
    for(int y = 0; y < h; y++){
      render::scene::Entity k = m_glRenderContext->createPrimitive(render::primitive::Cube);
      //k.getComponent<Transform>().scale(1);
      k.getComponent<Transform>().setPosition(0,0,2);
      e.push_back(k);
    }
  }

  while(m_running){
    ++m_frame;
    for(auto& a : e){
      a.getComponent<Transform>().rotate(1,1,1,.003f);
      //a.getComponent<Transform>().setPositionZ(sin(m_frame/10.0f));
    }

    m_tasks.run();  
    for(auto& luaObjectVariant : m_mappedLuaObjects){
      std::visit([](auto&& mappedObject){
        mappedObject.read();
      },luaObjectVariant);
    }
    /**
     * Draws every camera to a texture
     */
    
    m_glRenderContext->render();
    
    for(auto& window : m_windowManager.windows()){ 
      if(!m_windowCameras.count(window->id())){
        cprint_error(LOG_TAG) << "Window did not have associated camera, skipping " << std::endl;
        continue;
      }
      window->beginDraw();
      render::scene::Entity camera = m_windowCameras[window->id()];
      render::ICamera& cameraComponent = camera.getComponent<render::ICamera>();
      cameraComponent.blitToScreen(0,0,window->width(),window->height());
      window->endDraw();
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(16));
  }
}


void Context::stopLooping(){
    m_running = false;
}
