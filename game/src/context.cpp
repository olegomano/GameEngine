#include "context.h"
#include <log.h>
#include "luaApi.h"
#include <chrono>
#include <file.h>
#define LOG_TAG "Context"


Context::Context(){
  //m_glContext.setContextCallback(std::bind(&Context::onGLContextInit,this));
}

void Context::init(){
  m_luaContext.init(0);
  int fcount = sizeof(LuaApi)/sizeof(Lua_ApiCall); 
  for(int i =0 ; i < fcount ; i++){
    cprint_debug(LOG_TAG) << "Registering Function " << LuaApi[i].name << std::endl;
    m_luaContext.registerFunction(LuaApi[i].name,LuaApi[i].call);
  }
  lua::LuaVar self = m_luaContext.createVar<void*>(true,"_SCRIPT");
  lua::write_global<void*>(m_luaContext.lua(),"_SCRIPT",this);

  for(int i = 0; i < render::scene::ComponentDescriptors_Length; i++){
    const render::scene::ComponentDescriptor& d = render::scene::ComponentDescriptors[i];
    //m_luaContext.createGlobal(d.name,lua::FlatTable());
    m_luaContext.createVar<lua::FlatTable>(true,d.name);
  }

  for(const std::string& path : m_initScript){
    core::file::File f = core::file::File(path);
    f.load();
    m_luaContext.loadBuffer(f.data(),f.size());
  }

}


void Context::initRendering(uint32_t screenW, uint32_t screenH, float renderScale, SDLWindowManager::Backend b){
  cprint_debug(LOG_TAG) << "Initializing Rendering " << screenW << " " << screenH << " " << renderScale << std::endl;
  switch(b){
  case SDLWindowManager::Backend::GL:
    m_windowManager.create(SDLWindowManager::Backend::GL);
    m_glRenderContext = new render::gl::GLContext();
    break;
  default:
    return;
  }

  SDLWindow* window = m_windowManager.addWindow("Game",screenW,screenH);
  if(window == nullptr){
    cprint_debug(LOG_TAG) << "Failed init, windowManager returned null window" << std::endl;
    return;
  }
  m_glRenderContext->create();
  m_glRenderContext->addEventListener(std::bind(&Context::handleEntityCreatedEvent,this,std::placeholders::_1,std::placeholders::_2));

  render::scene::Entity camera = m_glRenderContext->createCamera(window->width()*renderScale,window->height()*renderScale);
  camera.getComponent<render::ICamera>().setFov(1,(float)screenW/(float)screenH);
  m_windowCameras[window->id()] = camera;
  m_isRenderInit = true;
}

void Context::addLuaEventHandler(const std::string& event, const lua::FunctionHandle& h){
  cprint_debug(LOG_TAG) << "Adding Handler for event " << event << std::endl;
  if(event == "frame"){
    m_luaHandlers.frameHandlers.push_back(lua::LuaVar(&m_luaContext,h));
  }
  else if(event == "key"){
    m_luaHandlers.keyboardHandlers.push_back(lua::LuaVar(&m_luaContext,h));
  }
  else if(event == "entity"){
    m_luaHandlers.entityCreationHandlers.push_back(lua::LuaVar(&m_luaContext,h));
  }else{
    cprint_debug(LOG_TAG) << "Unknown event type " << event << std::endl;
  }
}


void Context::onGLContextInit(){

}

void Context::handleEntityCreatedEvent(uint64_t eventId, void* ptr){
  render::scene::Entity entity = *((render::scene::Entity*)ptr);
  uint32_t index = -1;
  if(eventId != render::scene::Events::ENTITY_CREATED){
    return;
  }
  lua::LuaVar var;
  cprint_debug(LOG_TAG) << "Entity Created " << eventId << " " << entity.entityId() <<  " " << entity.contextId() << std::endl; 
  if(m_luaEntityContextMap.count(entity.contextId())){
    var = m_luaEntityContextMap[entity.contextId()];
    m_luaEntityContextMap.erase(entity.contextId());
  }
  else{
    var = m_luaContext.createVar<lua::FlatTable>();
  }
  
  var.write( entity.getComponent<Transform>() );
  m_luaEntities.push_back( std::make_tuple(var,entity) );
  
  entity.foreachComponent(
    [&](render::scene::Component c, void* ptr) mutable {
    switch(c){    
      
    case render::scene::Component::Drawable:{
      render::IDrawable* d = (render::IDrawable*) ptr; 
      cprint_debug(LOG_TAG) << "Creating Lua Drawable " << std::endl;
      var.write(d);
      
      m_luaContext.load(render::scene::descriptor<render::scene::Component::Drawable>().name);
      var.load();
      lua::append_list(m_luaContext.lua(),true);
    }break;
    case render::scene::Component::Camera:{
      render::ICamera* c = (render::ICamera*) ptr;
      cprint_debug(LOG_TAG) << "Creating Lua Camera " << std::endl;
      var.write(c);

      m_luaContext.load(render::scene::descriptor<render::scene::Component::Camera>().name);
      var.load();
      lua::append_list(m_luaContext.lua(),true);
    }break;
        
    default:
    break; 
    }
  });

}

void Context::loadLuaFile(const std::string& name){
  cprint_debug(LOG_TAG) << "Loading Script " << name << std::endl;
  core::file::File f = core::file::File(name);
  f.load();
  this->m_luaContext.loadBuffer(f.data(),f.size());    
}

void Context::loadLuaBuffer(const std::string& line){
  cprint_debug(LOG_TAG) << "Loading Buffer " << line << std::endl;
  this->m_luaContext.loadBuffer((const uint8_t*)line.c_str(),line.size(),cprint_debug("Lua"));    
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

/**
 *
 */
lua::LuaVar Context::createPrimitive(render::primitive::Type t){
  uint32_t contextId = m_luaEntityContextId++;
  render::scene::Entity e = m_glRenderContext->createPrimitive(t,contextId);
  lua::LuaVar luaEntity = m_luaContext.createVar<lua::FlatTable>();
  m_luaEntityContextMap[contextId] = luaEntity;
  return luaEntity;
}

void Context::loopForever(){
  while(!m_isRenderInit){
    std::this_thread::sleep_for(std::chrono::milliseconds(16));
    //cprint_debug(LOG_TAG) << "Waiting For Rendering to Init... " << std::endl;
    m_tasks.run();  
  }

  cprint_debug(LOG_TAG) << "Game Loop Started" << std::endl;


  while(m_running){
    ++m_frame;
    m_tasks.run();  
    m_glRenderContext->handleEvents(); 
    
    for(const auto& iter : m_luaHandlers.frameHandlers){
      iter.call();
    }
    m_windowManager.handleInput([&](const SDL_Event& event){
      //cprint_debug(LOG_TAG) << event.key << std::endl; 
      std::string keyname = std::string( SDL_GetKeyName( event.key.keysym.sym ) );
      for(const auto& iter : m_luaHandlers.keyboardHandlers){
        iter.call(keyname);
      }
    });
    
    for(const auto& iter : m_luaEntities){
      const lua::LuaVar& var = std::get<0>(iter);
      render::scene::Entity entity = std::get<1>(iter);
      var.read(entity.getComponent<Transform>());
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
